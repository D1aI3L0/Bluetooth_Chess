#include "chessbot.h"

int16_t chessbot::ab_minimax(const chessboard& board, chessmove move, int16_t alpha, int16_t beta, uint8_t level_left)
{
	chessboard __board = board;
	stack<chessmove> moves = __board.make_move(move);

	if (level_left == 0)
		return __board.evaluate();
	else
		if (moves.empty())
			return __board.evaluate();
		else
		{
			int16_t eval = __board.next_move_color() == chesscolor::white ? INT16_MIN : INT16_MAX;

			for (; !moves.empty(); moves.pop())
			{
				int16_t eval_next = ab_minimax(__board, moves.top(), alpha, beta, level_left - 1);

				if (__board.next_move_color() == chesscolor::white)
				{
					if (eval_next > eval)
						eval = eval_next;

					if (eval_next > alpha)
						alpha = eval_next;
				}
				else
				{
					if (eval_next < eval)
						eval = eval_next;

					if (eval_next < beta)
						beta = eval_next;
				}

				if (alpha > beta)
					break;
			}

			return eval;
		}
}

chessmove chessbot::make_move(const chessboard& board, uint8_t level, bool parallel)
{
	stack<chessmove> moves = board.get_moves();
	size_t moves_size = moves.size();

	if (moves_size == 0)
		throw new exception();
	else
	{
		int16_t eval = board.next_move_color() == chesscolor::white ? INT16_MIN : INT16_MAX;
		chessmove move;

		int16_t* evals_list = new int16_t[moves_size];
		chessmove* moves_list = new chessmove[moves_size];

		for (int i = 0; i < moves_size; ++i, moves.pop())
			moves_list[i] = moves.top();

		if (parallel)
		{
#pragma omp parallel num_threads(moves_size)
#pragma omp for
			for (int i = 0; i < moves_size; ++i)
			{
				if (level == 0)
					evals_list[i] = 0;
				else
					evals_list[i] = ab_minimax(board, moves_list[i], INT16_MIN, INT16_MAX, level - 1);

				if (board.next_move_color() == chesscolor::white && evals_list[i] > eval)
					eval = evals_list[i];

				if (board.next_move_color() == chesscolor::black && evals_list[i] < eval)
					eval = evals_list[i];
			}
		}
		else
		{
			int16_t alpha = INT16_MIN, beta = INT16_MAX;

			for (int i = 0; i < moves_size; ++i)
			{
				if (level == 0)
					evals_list[i] = 0;
				else
					evals_list[i] = ab_minimax(board, moves_list[i], alpha, beta, level - 1);

				if (board.next_move_color() == chesscolor::white)
				{
					if (evals_list[i] > eval)
						eval = evals_list[i];

					if (evals_list[i] > alpha)
						alpha = evals_list[i];
				}
				else
				{
					if (evals_list[i] < eval)
						eval = evals_list[i];

					if (evals_list[i] < beta)
						beta = evals_list[i];
				}
			}
		}

		int eval_count = 0;

		for (int i = 0; i < moves_size; ++i)
			if (evals_list[i] == eval)
				++eval_count;

		srand(time(0));
		int eval_num = rand() % eval_count;

		for (int i = 0; i < moves_size; ++i)
			if (evals_list[i] == eval)
			{
				if (eval_num == 0)
				{
					move = moves_list[i];
					break;
				}
				else
					--eval_num;
			}

		delete[] moves_list;
		delete[] evals_list;

		return move;
	}
}