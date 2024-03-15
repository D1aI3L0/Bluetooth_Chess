#include "chessboard.h"

#define BOARD_ILLEGAL_MOVE_MSG "Illegal chess move"
#define BOARD_ILLEGAL_TEST_MOVE_MSG "Illegal test chess move"
#define BOARD_INVALID_STATE_MSG "Invalid chessboard state"

inline chesscolor operator!(const chesscolor& color)
{
	switch (color)
	{
	case chesscolor::none: return chesscolor::none;
	case chesscolor::white: return chesscolor::black;
	case chesscolor::black: return chesscolor::white;
	}
}

inline chessstate operator!(const chessstate& state)
{
	switch (state)
	{
	case chessstate::none: return chessstate::none;
	case chessstate::touched: return chessstate::untouched;
	case chessstate::untouched: return chessstate::touched;
	}
}

inline chesstimer operator!(const chesstimer& timer)
{
	switch (timer)
	{
	case chesstimer::none: return chesstimer::none;
	case chesstimer::charged: return chesstimer::uncharged;
	case chesstimer::uncharged: return chesstimer::charged;
	}
}

chessboard::chessboard() {}

void chessboard::check_and_push_promotions(stack<chessmove> moves, chessmove move) const
{
	if (check_no_check(move))
	{
		moves.push(chessmove(move, chessman::pawn));
		moves.push(chessmove(move, chessman::knight));
		moves.push(chessmove(move, chessman::bishop));
		moves.push(chessmove(move, chessman::rook));
		moves.push(chessmove(move, chessman::queen));
	}
}

bool chessboard::check_move(chessmove move) const
{
	stack<chessmove> moves = get_moves();

	while (!moves.empty())
	{
		if (moves.top().from.x == move.from.x && moves.top().from.y == move.from.y
			&& moves.top().to.x == move.to.x && moves.top().to.y == move.to.y)
			return true;

		moves.pop();
	}

	return false;
}

bool chessboard::check_no_check(chessmove move) const
{
	chessboard __board = *this;
	__board.make_test_move(move);
	return !__board.is_self_check();
}

int16_t chessboard::evaluate() const
{
	const int16_t gp_max_score = 24, gp_min_score = 0, gp_max_limit = 20, gp_min_limit = 4, meg_max_score = 16, meg_min_score = 0;
	int16_t eval = 0, men_eval = 0, mg_tables_eval = 0, eg_tables_eval = 0, gp_score = 0, mg_score, eg_score;

	chesspoint cell((uint8_t)0, (uint8_t)0);
	for (cell.x = (uint8_t)0; cell.x < BOARD_SIZE; ++cell.x)
		for (cell.y = (uint8_t)0; cell.y < BOARD_SIZE; ++cell.y)
			if (board[cell.x][cell.y].man != chessman::none)
			{
				chesspoint eval_table_cell = cell;

				switch (board[cell.x][cell.y].color)
				{
				case chesscolor::white:
					if (rotated)
						eval_table_cell.rotate();

					men_eval += evaluation::men_score_table[(uint8_t)board[cell.x][cell.y].man];
					mg_tables_eval += evaluation::middle_game_tables[(uint8_t)board[cell.x][cell.y].man][eval_table_cell.x * BOARD_SIZE + eval_table_cell.y];
					eg_tables_eval += evaluation::end_game_tables[(uint8_t)board[cell.x][cell.y].man][eval_table_cell.x * BOARD_SIZE + eval_table_cell.y];

					break;

				case chesscolor::black:
					if (!rotated)
						eval_table_cell.rotate();

					men_eval -= evaluation::men_score_table[(uint8_t)board[cell.x][cell.y].man];
					mg_tables_eval -= evaluation::middle_game_tables[(uint8_t)board[cell.x][cell.y].man][eval_table_cell.x * BOARD_SIZE + eval_table_cell.y];
					eg_tables_eval -= evaluation::end_game_tables[(uint8_t)board[cell.x][cell.y].man][eval_table_cell.x * BOARD_SIZE + eval_table_cell.y];

					break;
				}

				gp_score += evaluation::game_phase_score_table[(uint8_t)board[cell.x][cell.y].man];
			}

	if (gp_score > gp_max_score)
		gp_score = gp_max_score;

	if (gp_score < gp_min_limit)
	{
		mg_score = meg_min_score;
		eg_score = meg_max_score;
	}
	else if (gp_score > gp_max_limit)
	{
		mg_score = meg_max_score;
		eg_score = meg_min_score;
	}
	else
	{
		mg_score = gp_score - gp_min_limit;
		eg_score = gp_max_limit - gp_score;
	}

	eval = men_eval + ((mg_tables_eval * mg_score + eg_tables_eval * eg_score) / meg_max_score);

	switch (status)
	{
	case chessstatus::check:
		if (next_move == chesscolor::white)
			eval -= evaluation::check;
		else
			eval += evaluation::check;
		break;

	case chessstatus::mate:
		if (next_move == chesscolor::white)
			eval -= evaluation::mate;
		else
			eval += evaluation::mate;
		break;

	case chessstatus::stalemate:
	case chessstatus::deadend:
		eval = 0;
		break;
	}

	return eval;
}

chessstatus chessboard::get_board_status() const
{
	return status;
}

stack<chessmove> chessboard::get_moves() const
{
	stack<chessmove> moves;

	chesspoint cell((uint8_t)0, (uint8_t)0);
	for (cell.x = (uint8_t)0; cell.x < BOARD_SIZE; ++cell.x)
		for (cell.y = (uint8_t)0; cell.y < BOARD_SIZE; ++cell.y)
		{
			if ((rotated && next_move == chesscolor::white) || (!rotated && next_move == chesscolor::black))
				cell.rotate();

			if (board[cell.x][cell.y].color == next_move)
				switch (board[cell.x][cell.y].man)
				{
				case chessman::king:

					// King moves 1 cell
					for (int8_t i = cell.x - 1; i <= cell.x + 1; ++i)
						for (int8_t j = cell.y - 1; j <= cell.y + 1; ++j)
							if (i >= 0 && i < BOARD_SIZE && j >= 0 && j < BOARD_SIZE && (i != cell.x || j != cell.y) && board[i][j].color != next_move
								&& check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)i, (uint8_t)j)))
								moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)i, (uint8_t)j));

					// Castling
					if (board[cell.x][cell.y].state == chessstate::untouched)
					{
						// Right castling
						if (board[cell.x][BOARD_SIZE - 1].state == chessstate::untouched && status != chessstatus::check
							&& !is_threaten(!next_move, chesspoint((uint8_t)cell.x, (uint8_t)cell.y + 1)))
						{
							bool clear = true;

							for (int8_t k = cell.y + 1; k < BOARD_SIZE - 1; ++k)
								if (board[cell.x][k].man != chessman::none)
								{
									clear = false;
									break;
								}

							if (clear && check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x, (uint8_t)cell.y + 2)))
								moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x, (uint8_t)cell.y + 2));
						}

						// Left castling
						if (board[cell.x][0].state == chessstate::untouched && status != chessstatus::check
							&& !is_threaten(!next_move, chesspoint((uint8_t)cell.x, (uint8_t)cell.y - 1)))
						{
							bool clear = true;

							for (int8_t k = cell.y - 1; k >= 1; --k)
								if (board[cell.x][k].man != chessman::none)
								{
									clear = false;
									break;
								}

							if (clear && check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x, (uint8_t)cell.y - 2)))
								moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x, (uint8_t)cell.y - 2));
						}
					}

					break;

				case chessman::queen:

					// Queen moves top right
					for (int8_t i = cell.x + 1, j = cell.y + 1; i < BOARD_SIZE && j < BOARD_SIZE; ++i, ++j)
						if (board[i][j].color == chesscolor::none)
						{
							if (check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)i, (uint8_t)j)))
								moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)i, (uint8_t)j));
						}
						else if (board[i][j].color == !next_move)
						{
							if (check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)i, (uint8_t)j)))
								moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)i, (uint8_t)j));

							break;
						}
						else
							break;

					// Queen moves top left
					for (int8_t i = cell.x + 1, j = cell.y - 1; i < BOARD_SIZE && j >= 0; ++i, --j)
						if (board[i][j].color == chesscolor::none)
						{
							if (check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)i, (uint8_t)j)))
								moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)i, (uint8_t)j));
						}
						else if (board[i][j].color == !next_move)
						{
							if (check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)i, (uint8_t)j)))
								moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)i, (uint8_t)j));

							break;
						}
						else
							break;

					// Queen moves bottom right
					for (int8_t i = cell.x - 1, j = cell.y + 1; i >= 0 && j < BOARD_SIZE; --i, ++j)
						if (board[i][j].color == chesscolor::none)
						{
							if (check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)i, (uint8_t)j)))
								moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)i, (uint8_t)j));
						}
						else if (board[i][j].color == !next_move)
						{
							if (check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)i, (uint8_t)j)))
								moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)i, (uint8_t)j));

							break;
						}
						else
							break;

					// Queen moves bottom left
					for (int8_t i = cell.x - 1, j = cell.y - 1; i >= 0 && j >= 0; --i, --j)
						if (board[i][j].color == chesscolor::none)
						{
							if (check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)i, (uint8_t)j)))
								moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)i, (uint8_t)j));
						}
						else if (board[i][j].color == !next_move)
						{
							if (check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)i, (uint8_t)j)))
								moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)i, (uint8_t)j));

							break;
						}
						else
							break;

					// Queen moves top
					for (int8_t k = cell.x + 1; k < BOARD_SIZE; ++k)
						if (board[k][cell.y].color == chesscolor::none)
						{
							if (check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)k, (uint8_t)cell.y)))
								moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)k, (uint8_t)cell.y));
						}
						else if (board[k][cell.y].color == !next_move)
						{
							if (check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)k, (uint8_t)cell.y)))
								moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)k, (uint8_t)cell.y));

							break;
						}
						else
							break;

					// Queen moves bottom
					for (int8_t k = cell.x - 1; k >= 0; --k)
						if (board[k][cell.y].color == chesscolor::none)
						{
							if (check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)k, (uint8_t)cell.y)))
								moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)k, (uint8_t)cell.y));
						}
						else if (board[k][cell.y].color == !next_move)
						{
							if (check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)k, (uint8_t)cell.y)))
								moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)k, (uint8_t)cell.y));

							break;
						}
						else
							break;

					// Queen moves right
					for (int8_t k = cell.y + 1; k < BOARD_SIZE; ++k)
						if (board[cell.x][k].color == chesscolor::none)
						{
							if (check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x, (uint8_t)k)))
								moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x, (uint8_t)k));
						}
						else if (board[cell.x][k].color == !next_move)
						{
							if (check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x, (uint8_t)k)))
								moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x, (uint8_t)k));

							break;
						}
						else
							break;

					// Queen moves left
					for (int8_t k = cell.y - 1; k >= 0; --k)
						if (board[cell.x][k].color == chesscolor::none)
						{
							if (check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x, (uint8_t)k)))
								moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x, (uint8_t)k));
						}
						else if (board[cell.x][k].color == !next_move)
						{
							if (check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x, (uint8_t)k)))
								moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x, (uint8_t)k));

							break;
						}
						else
							break;

					break;

				case chessman::bishop:

					// Bishop moves top right
					for (int8_t i = cell.x + 1, j = cell.y + 1; i < BOARD_SIZE && j < BOARD_SIZE; ++i, ++j)
						if (board[i][j].color == chesscolor::none)
						{
							if (check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)i, (uint8_t)j)))
								moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)i, (uint8_t)j));
						}
						else if (board[i][j].color == !next_move)
						{
							if (check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)i, (uint8_t)j)))
								moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)i, (uint8_t)j));

							break;
						}
						else
							break;

					// Bishop moves top left
					for (int8_t i = cell.x + 1, j = cell.y - 1; i < BOARD_SIZE && j >= 0; ++i, --j)
						if (board[i][j].color == chesscolor::none)
						{
							if (check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)i, (uint8_t)j)))
								moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)i, (uint8_t)j));
						}
						else if (board[i][j].color == !next_move)
						{
							if (check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)i, (uint8_t)j)))
								moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)i, (uint8_t)j));

							break;
						}
						else
							break;

					// Bishop moves bottom right
					for (int8_t i = cell.x - 1, j = cell.y + 1; i >= 0 && j < BOARD_SIZE; --i, ++j)
						if (board[i][j].color == chesscolor::none)
						{
							if (check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)i, (uint8_t)j)))
								moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)i, (uint8_t)j));
						}
						else if (board[i][j].color == !next_move)
						{
							if (check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)i, (uint8_t)j)))
								moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)i, (uint8_t)j));

							break;
						}
						else
							break;

					// Bishop moves bottom left
					for (int8_t i = cell.x - 1, j = cell.y - 1; i >= 0 && j >= 0; --i, --j)
						if (board[i][j].color == chesscolor::none)
						{
							if (check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)i, (uint8_t)j)))
								moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)i, (uint8_t)j));
						}
						else if (board[i][j].color == !next_move)
						{
							if (check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)i, (uint8_t)j)))
								moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)i, (uint8_t)j));

							break;
						}
						else
							break;

					break;

				case chessman::knight:

					// Knight moves [+2, +1]
					if (cell.x + 2 < BOARD_SIZE && cell.y + 1 < BOARD_SIZE && board[cell.x + 2][cell.y + 1].color != next_move
						&& check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x + 2, (uint8_t)cell.y + 1)))
						moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x + 2, (uint8_t)cell.y + 1));

					// Knight moves [+2, -1]
					if (cell.x + 2 < BOARD_SIZE && cell.y - 1 >= 0 && board[cell.x + 2][cell.y - 1].color != next_move
						&& check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x + 2, (uint8_t)cell.y - 1)))
						moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x + 2, (uint8_t)cell.y - 1));

					// Knight moves [+1, +2]
					if (cell.x + 1 < BOARD_SIZE && cell.y + 2 < BOARD_SIZE && board[cell.x + 1][cell.y + 2].color != next_move
						&& check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x + 1, (uint8_t)cell.y + 2)))
						moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x + 1, (uint8_t)cell.y + 2));

					// Knight moves [+1, -2]
					if (cell.x + 1 < BOARD_SIZE && cell.y - 2 >= 0 && board[cell.x + 1][cell.y - 2].color != next_move
						&& check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x + 1, (uint8_t)cell.y - 2)))
						moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x + 1, (uint8_t)cell.y - 2));

					// Knight moves [-1, +2]
					if (cell.x - 1 >= 0 && cell.y + 2 < BOARD_SIZE && board[cell.x - 1][cell.y + 2].color != next_move
						&& check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x - 1, (uint8_t)cell.y + 2)))
						moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x - 1, (uint8_t)cell.y + 2));

					// Knight moves [-1, -2]
					if (cell.x - 1 >= 0 && cell.y - 2 >= 0 && board[cell.x - 1][cell.y - 2].color != next_move
						&& check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x - 1, (uint8_t)cell.y - 2)))
						moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x - 1, (uint8_t)cell.y - 2));

					// Knight moves [-2, +1]
					if (cell.x - 2 >= 0 && cell.y + 1 < BOARD_SIZE && board[cell.x - 2][cell.y + 1].color != next_move
						&& check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x - 2, (uint8_t)cell.y + 1)))
						moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x - 2, (uint8_t)cell.y + 1));

					// Knight moves [-2, -1]
					if (cell.x - 2 >= 0 && cell.y - 1 >= 0 && board[cell.x - 2][cell.y - 1].color != next_move
						&& check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x - 2, (uint8_t)cell.y - 1)))
						moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x - 2, (uint8_t)cell.y - 1));

					break;

				case chessman::rook:

					// Rook moves top
					for (int8_t k = cell.x + 1; k < BOARD_SIZE; ++k)
						if (board[k][cell.y].color == chesscolor::none)
						{
							if (check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)k, (uint8_t)cell.y)))
								moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)k, (uint8_t)cell.y));
						}
						else if (board[k][cell.y].color == !next_move)
						{
							if (check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)k, (uint8_t)cell.y)))
								moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)k, (uint8_t)cell.y));

							break;
						}
						else
							break;

					// Rook moves bottom
					for (int8_t k = cell.x - 1; k >= 0; --k)
						if (board[k][cell.y].color == chesscolor::none)
						{
							if (check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)k, (uint8_t)cell.y)))
								moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)k, (uint8_t)cell.y));
						}
						else if (board[k][cell.y].color == !next_move)
						{
							if (check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)k, (uint8_t)cell.y)))
								moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)k, (uint8_t)cell.y));

							break;
						}
						else
							break;

					// Rook moves right
					for (int8_t k = cell.y + 1; k < BOARD_SIZE; ++k)
						if (board[cell.x][k].color == chesscolor::none)
						{
							if (check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x, (uint8_t)k)))
								moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x, (uint8_t)k));
						}
						else if (board[cell.x][k].color == !next_move)
						{
							if (check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x, (uint8_t)k)))
								moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x, (uint8_t)k));

							break;
						}
						else
							break;

					// Rook moves left
					for (int8_t k = cell.y - 1; k >= 0; --k)
						if (board[cell.x][k].color == chesscolor::none)
						{
							if (check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x, (uint8_t)k)))
								moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x, (uint8_t)k));
						}
						else if (board[cell.x][k].color == !next_move)
						{
							if (check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x, (uint8_t)k)))
								moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x, (uint8_t)k));

							break;
						}
						else
							break;

					break;

				case chessman::pawn:

					// Pawn moves bottom else top
					if ((rotated && next_move == chesscolor::white) || (!rotated && next_move == chesscolor::black))
					{
						// In passing
						if (board[cell.x][cell.y].state == chessstate::touched)
						{
							// In passing right
							if (cell.x - 1 >= 0 && cell.y + 1 < BOARD_SIZE && board[cell.x][cell.y + 1].color != next_move && board[cell.x][cell.y + 1].timer == chesstimer::charged
								&& check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x - 1, (uint8_t)cell.y + 1)))
								moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x - 1, (uint8_t)cell.y + 1));

							// In passing left
							if (cell.x - 1 >= 0 && cell.y - 1 >= 0 && board[cell.x][cell.y - 1].color != next_move && board[cell.x][cell.y - 1].timer == chesstimer::charged
								&& check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x - 1, (uint8_t)cell.y - 1)))
								moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x - 1, (uint8_t)cell.y - 1));
						}

						// Long move
						if (board[cell.x][cell.y].state == chessstate::untouched && board[cell.x - 1][cell.y].man == chessman::none && board[cell.x - 2][cell.y].man == chessman::none
							&& check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x - 2, (uint8_t)cell.y)))
							moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x - 2, (uint8_t)cell.y));

						// No promotion else promotion
						if (cell.x - 1 >= 1)
						{
							// Short move
							if (board[cell.x - 1][cell.y].man == chessman::none && check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x - 1, (uint8_t)cell.y)))
								moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x - 1, (uint8_t)cell.y));

							// Right capture
							if (cell.y + 1 < BOARD_SIZE && board[cell.x - 1][cell.y + 1].color == !next_move
								&& check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x - 1, (uint8_t)cell.y + 1)))
								moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x - 1, (uint8_t)cell.y + 1));

							// Left capture
							if (cell.y - 1 >= 0 && board[cell.x - 1][cell.y - 1].color == !next_move
								&& check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x - 1, (uint8_t)cell.y - 1)))
								moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x - 1, (uint8_t)cell.y - 1));
						}
						else if (cell.x - 1 >= 0)
						{
							// Short move with promotion
							if (board[cell.x - 1][cell.y].man == chessman::none)
								check_and_push_promotions(moves, chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x - 1, (uint8_t)cell.y));

							// Right capture with promotion
							if (cell.y + 1 < BOARD_SIZE && board[cell.x - 1][cell.y + 1].color == !next_move)
								check_and_push_promotions(moves, chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x - 1, (uint8_t)cell.y + 1));

							// Left capture with promotion
							if (cell.y - 1 >= 0 && board[cell.x - 1][cell.y - 1].color == !next_move)
								check_and_push_promotions(moves, chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x - 1, (uint8_t)cell.y - 1));
						}
					}
					else
					{
						// In passing
						if (board[cell.x][cell.y].state == chessstate::touched)
						{
							// In passing right
							if (cell.x + 1 < BOARD_SIZE && cell.y + 1 < BOARD_SIZE && board[cell.x][cell.y + 1].color != next_move && board[cell.x][cell.y + 1].timer == chesstimer::charged
								&& check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x + 1, (uint8_t)cell.y + 1)))
								moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x + 1, (uint8_t)cell.y + 1));

							// In passing left
							if (cell.x + 1 < BOARD_SIZE && cell.y - 1 >= 0 && board[cell.x][cell.y - 1].color != next_move && board[cell.x][cell.y - 1].timer == chesstimer::charged
								&& check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x + 1, (uint8_t)cell.y - 1)))
								moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x + 1, (uint8_t)cell.y - 1));
						}

						// Long move
						if (board[cell.x][cell.y].state == chessstate::untouched && board[cell.x + 1][cell.y].man == chessman::none && board[cell.x + 2][cell.y].man == chessman::none
							&& check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x + 2, (uint8_t)cell.y)))
							moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x + 2, (uint8_t)cell.y));

						// No promotion else promotion
						if (cell.x + 1 < BOARD_SIZE - 1)
						{
							// Short move
							if (board[cell.x + 1][cell.y].man == chessman::none && check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x + 1, (uint8_t)cell.y)))
								moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x + 1, (uint8_t)cell.y));

							// Right capture
							if (cell.y + 1 < BOARD_SIZE && board[cell.x + 1][cell.y + 1].color == !next_move
								&& check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x + 1, (uint8_t)cell.y + 1)))
								moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x + 1, (uint8_t)cell.y + 1));

							// Left capture
							if (cell.y - 1 >= 0 && board[cell.x + 1][cell.y - 1].color == !next_move
								&& check_no_check(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x + 1, (uint8_t)cell.y - 1)))
								moves.push(chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x + 1, (uint8_t)cell.y - 1));
						}
						else if (cell.x + 1 < BOARD_SIZE)
						{
							// Short move with promotion
							if (board[cell.x + 1][cell.y].man == chessman::none)
								check_and_push_promotions(moves, chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x + 1, (uint8_t)cell.y));

							// Right capture with promotion
							if (cell.y + 1 < BOARD_SIZE && board[cell.x + 1][cell.y + 1].color == !next_move)
								check_and_push_promotions(moves, chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x + 1, (uint8_t)cell.y + 1));

							// Left capture with promotion
							if (cell.y - 1 >= 0 && board[cell.x + 1][cell.y - 1].color == !next_move)
								check_and_push_promotions(moves, chessmove((uint8_t)cell.x, (uint8_t)cell.y, (uint8_t)cell.x + 1, (uint8_t)cell.y - 1));
						}
					}

					break;
				}

			if ((rotated && next_move == chesscolor::white) || (!rotated && next_move == chesscolor::black))
				cell.rotate();
		}

	return moves;
}

bool chessboard::is_check() const
{
	// Seek for king of next move color
	for (int8_t i = 0; i < BOARD_SIZE; ++i)
		for (int8_t j = 0; j < BOARD_SIZE; ++j)
			if (board[i][j].color == next_move && board[i][j].man == chessman::king)

				// Return if king treaten by last move color men
				return is_threaten(!next_move, chesspoint((uint8_t)i, (uint8_t)j));

	// No king of next move color found
	throw new exception(BOARD_INVALID_STATE_MSG);
}

bool chessboard::is_deadend() const
{
	uint8_t men_count[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

	uint8_t deadend_1[8] = { 62, 2, 0, 0, 0, 0, 0, 0 };
	uint8_t deadend_2[8] = { 61, 2, 0, 1, 0, 0, 0, 0 };
	uint8_t deadend_3[8] = { 61, 2, 0, 0, 1, 0, 0, 0 };
	uint8_t deadend_4[8] = { 60, 2, 0, 2, 0, 0, 0, 0 };

	for (int8_t i = 0; i < BOARD_SIZE; ++i)
		for (int8_t j = 0; j < BOARD_SIZE; ++j)
			++(men_count[(uint8_t)board[i][j].man]);

	if ((uint64_t)men_count == (uint64_t)deadend_1 || (uint64_t)men_count == (uint64_t)deadend_2 || (uint64_t)men_count == (uint64_t)deadend_3)
		return true;
	else if ((uint64_t)men_count == (uint64_t)deadend_4)
	{
		chesscell first_bishop;
		int8_t k = -1;

		for (int8_t i = 0; i < BOARD_SIZE; ++i)
			for (int8_t j = 0; j < BOARD_SIZE; ++j)
				if (board[i][j].man == chessman::bishop)
					if (first_bishop.man == chessman::none)
					{
						first_bishop = board[i][j];
						k = (i + j) % 2;
					}
					else
						return first_bishop.color == board[i][j].color && k == (i + j) % 2;
	}
	else
		return false;
}

bool chessboard::is_rotated() const
{
	return rotated;
}

bool chessboard::is_self_check() const
{
	// Seek for king of last move color
	for (int8_t i = 0; i < BOARD_SIZE; ++i)
		for (int8_t j = 0; j < BOARD_SIZE; ++j)
			if (board[i][j].color == !next_move && board[i][j].man == chessman::king)

				// Return if king treaten by next move color men
				return is_threaten(next_move, chesspoint((uint8_t)i, (uint8_t)j));

	// No king of last move color found
	throw new exception(BOARD_INVALID_STATE_MSG);
}

bool chessboard::is_threaten(chesscolor color, chesspoint point) const
{
	// King
	for (int8_t i = point.x - 1; i <= point.x + 1; ++i)
		for (int8_t j = point.y - 1; j <= point.y + 1; ++j)
			if (i >= 0 && i < BOARD_SIZE && j >= 0 && j < BOARD_SIZE && (i != point.x || j != point.y) && board[i][j].color == color && board[i][j].man == chessman::king)
				return true;

	// Queen/rook on top
	for (int8_t k = point.x + 1; k < BOARD_SIZE; ++k)
		if (board[k][point.y].man != chessman::none)
			if (board[k][point.y].color == color && (board[k][point.y].man == chessman::queen || board[k][point.y].man == chessman::rook))
				return true;
			else
				break;

	// Queen/rook on bottom
	for (int8_t k = point.x - 1; k >= 0; --k)
		if (board[k][point.y].man != chessman::none)
			if (board[k][point.y].color == color && (board[k][point.y].man == chessman::queen || board[k][point.y].man == chessman::rook))
				return true;
			else
				break;

	// Queen/rook on right
	for (int8_t k = point.y + 1; k < BOARD_SIZE; ++k)
		if (board[point.x][k].man != chessman::none)
			if (board[point.x][k].color == color && (board[point.x][k].man == chessman::queen || board[point.x][k].man == chessman::rook))
				return true;
			else
				break;

	// Queen/rook on left
	for (int8_t k = point.y - 1; k >= 0; --k)
		if (board[point.x][k].man != chessman::none)
			if (board[point.x][k].color == color && (board[point.x][k].man == chessman::queen || board[point.x][k].man == chessman::rook))
				return true;
			else
				break;

	// Queen/bishop on top right
	for (int8_t i = point.x + 1, j = point.y + 1; i < BOARD_SIZE && j < BOARD_SIZE; ++i, ++j)
		if (board[i][j].man != chessman::none)
			if (board[i][j].color == color && (board[i][j].man == chessman::queen || board[i][j].man == chessman::bishop))
				return true;
			else
				break;

	// Queen/bishop on top left
	for (int8_t i = point.x + 1, j = point.y - 1; i < BOARD_SIZE && j >= 0; ++i, --j)
		if (board[i][j].man != chessman::none)
			if (board[i][j].color == color && (board[i][j].man == chessman::queen || board[i][j].man == chessman::bishop))
				return true;
			else
				break;

	// Queen/bishop on bottom right
	for (int8_t i = point.x - 1, j = point.y + 1; i >= 0 && j < BOARD_SIZE; --i, ++j)
		if (board[i][j].man != chessman::none)
			if (board[i][j].color == color && (board[i][j].man == chessman::queen || board[i][j].man == chessman::bishop))
				return true;
			else
				break;

	// Queen/bishop on bottom left
	for (int8_t i = point.x - 1, j = point.y - 1; i >= 0 && j >= 0; --i, --j)
		if (board[i][j].man != chessman::none)
			if (board[i][j].color == color && (board[i][j].man == chessman::queen || board[i][j].man == chessman::bishop))
				return true;
			else
				break;

	// Knight on [+2, +1]
	if (point.x + 2 < BOARD_SIZE && point.y + 1 < BOARD_SIZE && board[point.x + 2][point.y + 1].color == color && board[point.x + 2][point.y + 1].man == chessman::knight)
		return true;

	// Knight on [+2, -1]
	if (point.x + 2 < BOARD_SIZE && point.y - 1 >= 0 && board[point.x + 2][point.y - 1].color == color && board[point.x + 2][point.y - 1].man == chessman::knight)
		return true;

	// Knight on [+1, +2]
	if (point.x + 1 < BOARD_SIZE && point.y + 2 < BOARD_SIZE && board[point.x + 1][point.y + 2].color == color && board[point.x + 1][point.y + 2].man == chessman::knight)
		return true;

	// Knight on [+1, -2]
	if (point.x + 1 < BOARD_SIZE && point.y - 2 >= 0 && board[point.x + 1][point.y - 2].color == color && board[point.x + 1][point.y - 2].man == chessman::knight)
		return true;

	// Knight on [-1, +2]
	if (point.x - 1 >= 0 && point.y + 2 < BOARD_SIZE && board[point.x - 1][point.y + 2].color == color && board[point.x - 1][point.y + 2].man == chessman::knight)
		return true;

	// Knight on [-1, -2]
	if (point.x - 1 >= 0 && point.y - 2 >= 0 && board[point.x - 1][point.y - 2].color == color && board[point.x - 1][point.y - 2].man == chessman::knight)
		return true;

	// Knight on [-2, +1]
	if (point.x - 2 >= 0 && point.y + 1 < BOARD_SIZE && board[point.x - 2][point.y + 1].color == color && board[point.x - 2][point.y + 1].man == chessman::knight)
		return true;

	// Knight on [-2, -1]
	if (point.x - 2 >= 0 && point.y - 1 >= 0 && board[point.x - 2][point.y - 1].color == color && board[point.x - 2][point.y - 1].man == chessman::knight)
		return true;

	// Pawns move bottom else top
	if ((rotated && color == chesscolor::white) || (!rotated && color == chesscolor::black))
	{
		// Pawn on right
		if (point.x + 1 < BOARD_SIZE && point.y + 1 < BOARD_SIZE && board[point.x + 1][point.y + 1].color == color && board[point.x + 1][point.y + 1].man == chessman::pawn)
			return true;

		// Pawn on left
		if (point.x + 1 < BOARD_SIZE && point.y - 1 >= 0 && board[point.x + 1][point.y - 1].color == color && board[point.x + 1][point.y - 1].man == chessman::pawn)
			return true;
	}
	else
	{
		// Pawn on right
		if (point.x - 1 >= 0 && point.y + 1 < BOARD_SIZE && board[point.x - 1][point.y + 1].color == color && board[point.x - 1][point.y + 1].man == chessman::pawn)
			return true;

		// Pawn on left
		if (point.x - 1 >= 0 && point.y - 1 >= 0 && board[point.x - 1][point.y - 1].color == color && board[point.x - 1][point.y - 1].man == chessman::pawn)
			return true;
	}

	// In passing
	if (board[point.x][point.y].man == chessman::pawn && board[point.x][point.y].timer == chesstimer::charged)
	{
		// Pawn on right
		if (point.x < BOARD_SIZE && point.y + 1 < BOARD_SIZE && board[point.x][point.y + 1].color == color && board[point.x][point.y + 1].man == chessman::pawn)
			return true;

		// Pawn on left
		if (point.x < BOARD_SIZE && point.y - 1 >= 0 && board[point.x][point.y - 1].color == color && board[point.x][point.y - 1].man == chessman::pawn)
			return true;
	}

	// No threats
	return false;
}

stack<chessmove> chessboard::make_move(chessmove move)
{
	try
	{
		// Uncharge all timers
		for (int8_t i = 0; i < BOARD_SIZE; ++i)
			for (int8_t j = 0; j < BOARD_SIZE; ++j)
				if (board[i][j].color == next_move)
					board[i][j].timer = chesstimer::uncharged;

		switch (board[move.from.x][move.from.y].man)
		{
		case chessman::king:

			// Right castling
			if (move.from.y + 2 == move.to.y)
			{
				board[move.to.x][move.to.y - 1] = board[move.from.x][BOARD_SIZE - 1];
				board[move.to.x][move.to.y - 1].state = chessstate::touched;
				board[move.from.x][BOARD_SIZE - 1].set_empty();
			}

			// Left castling
			if (move.from.y - 2 == move.to.y)
			{
				board[move.to.x][move.to.y + 1] = board[move.from.x][0];
				board[move.to.x][move.to.y + 1].state = chessstate::touched;
				board[move.from.x][0].set_empty();
			}

			break;

		case chessman::pawn:

			// Long move
			if (abs(move.from.x - move.to.x) == 2)
				board[move.from.x][move.from.y].timer = chesstimer::charged;

			// In passing
			if (move.from.y != move.to.y && board[move.to.x][move.to.y].man == chessman::none)
				board[move.from.x][move.to.y].set_empty();

			// Promotion
			if (move.to.x == BOARD_SIZE - 1 || move.to.x == 0)
				board[move.from.x][move.from.y].man = move.to_promote;

			break;
		}

		// Move man
		board[move.to.x][move.to.y] = board[move.from.x][move.from.y];
		board[move.to.x][move.to.y].state = chessstate::touched;
		board[move.from.x][move.from.y].set_empty();

		// Switch colors
		next_move = !next_move;

		// Get next possible moves
		stack<chessmove> next_moves = get_moves();

		// Set board status
		if (is_check())
			if (next_moves.empty())
				status = chessstatus::mate;
			else
				status = chessstatus::check;
		else
			if (next_moves.empty())
				status = chessstatus::stalemate;
			else if (is_deadend())
				status = chessstatus::deadend;
			else
				status = chessstatus::none;

		return next_moves;
	}
	catch (...)
	{
		throw new exception(BOARD_ILLEGAL_MOVE_MSG);
	}
}

void chessboard::make_test_move(chessmove move)
{
	try
	{
		// Uncharge all timers
		for (int8_t i = 0; i < BOARD_SIZE; ++i)
			for (int8_t j = 0; j < BOARD_SIZE; ++j)
				if (board[i][j].color == next_move)
					board[i][j].timer = chesstimer::uncharged;

		switch (board[move.from.x][move.from.y].man)
		{
		case chessman::king:

			// Right castling
			if (move.from.y + 2 == move.to.y)
			{
				board[move.to.x][move.to.y - 1] = board[move.from.x][BOARD_SIZE - 1];
				board[move.to.x][move.to.y - 1].state = chessstate::touched;
				board[move.from.x][BOARD_SIZE - 1].set_empty();
			}

			// Left castling
			if (move.from.y - 2 == move.to.y)
			{
				board[move.to.x][move.to.y + 1] = board[move.from.x][0];
				board[move.to.x][move.to.y + 1].state = chessstate::touched;
				board[move.from.x][0].set_empty();
			}

			break;

		case chessman::pawn:

			// Long move
			if (abs(move.from.x - move.to.x) == 2)
				board[move.from.x][move.from.y].timer = chesstimer::charged;

			// In passing
			if (move.from.y != move.to.y && board[move.to.x][move.to.y].man == chessman::none)
				board[move.from.x][move.to.y].set_empty();

			// Promotion
			if (move.to.x == BOARD_SIZE - 1 || move.to.x == 0)
				board[move.from.x][move.from.y].man = move.to_promote;

			break;
		}

		// Move man
		board[move.to.x][move.to.y] = board[move.from.x][move.from.y];
		board[move.to.x][move.to.y].state = chessstate::touched;
		board[move.from.x][move.from.y].set_empty();

		// Switch colors
		next_move = !next_move;

		// Set board status
		status = chessstatus::none;
	}
	catch (...)
	{
		throw new exception(BOARD_ILLEGAL_TEST_MOVE_MSG);
	}
}

chesscolor chessboard::get_next_move_color() const
{
	return next_move;
}

void chessboard::rotate()
{
	rotated = !rotated;

	for (int8_t i = 0; i < BOARD_SIZE / 2; ++i)
		for (int8_t j = 0; j < BOARD_SIZE; ++j)
			swap(board[i][j], board[BOARD_SIZE - i - 1][BOARD_SIZE - j - 1]);
}