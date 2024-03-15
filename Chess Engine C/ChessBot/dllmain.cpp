#include <stack>
#include <stdint.h>
#include <Windows.h>
#include "chessboard.h"
#include "chessbot.h"
#include "chessmove.h"

using namespace std;

inline bool is_valid_chess_board(const chessboard* board)
{
	return board != nullptr;
}

inline bool is_valid_chess_move(chessmove move)
{
	return move.from.x < BOARD_SIZE&& move.from.y < BOARD_SIZE&& move.to.x < BOARD_SIZE&& move.to.y < BOARD_SIZE
		&& (move.to_promote == chessman::none || move.to_promote == chessman::queen || move.to_promote == chessman::bishop
			|| move.to_promote == chessman::knight || move.to_promote == chessman::rook || move.to_promote == chessman::pawn);
}

inline bool is_valid_chess_moves(const chessmove* moves)
{
	return moves != nullptr;
}

extern "C"
{
	__declspec(dllexport) void ChessBoardMakeMove(chessboard* ChessBoard, chessmove ChessMove)
	{
		if (is_valid_chess_board(ChessBoard) && is_valid_chess_move(ChessMove))
			ChessBoard->make_move(ChessMove);
		else
			throw new exception("ChessBoardMakeMove: invalid arguments");
	}

	__declspec(dllexport) chessmove ChessBotMakeMove(const chessboard* ChessBoard, uint8_t Difficulty, bool Parallel = false)
	{
		if (is_valid_chess_board(ChessBoard))
			return chessbot::make_move(*ChessBoard, Difficulty, Parallel);
		else
			throw new exception("ChessBotMakeMove: invalid arguments");
	}

	__declspec(dllexport) int16_t GetChessBoardEvaluationValue(const chessboard* ChessBoard)
	{
		if (is_valid_chess_board(ChessBoard))
			return ChessBoard->evaluate();
		else
			throw new exception("GetChessBoardEvaluationValue: invalid arguments");
	}

	__declspec(dllexport) uint8_t GetLegalChessMoves(const chessboard* ChessBoard, chessmove* ChessMoves, uint8_t ChessMovesSize)
	{
		if (is_valid_chess_board(ChessBoard) && is_valid_chess_moves(ChessMoves))
		{
			stack<chessmove> moves = ChessBoard->get_moves();
			uint8_t moves_size = moves.size();

			if (moves_size <= ChessMovesSize)
				try
			{
				for (uint8_t i = 0; i < moves_size; ++i, moves.pop())
					ChessMoves[i] = moves.top();
			}
			catch (...)
			{
				throw new exception("GetLegalChessMoves: invalid arguments");
			}

			return moves_size;
		}
		else
			throw new exception("GetLegalChessMoves: invalid arguments");
	}

	__declspec(dllexport) void GetNewChessBoard(chessboard* ChessBoard)
	{
		if (is_valid_chess_board(ChessBoard))
			*ChessBoard = chessboard();
		else
			throw new exception("GetNewChessBoard: invalid arguments");
	}

	__declspec(dllexport) bool IsLegalChessMove(const chessboard* ChessBoard, chessmove ChessMove)
	{
		if (is_valid_chess_board(ChessBoard) && is_valid_chess_move(ChessMove))
			return ChessBoard->check_move(ChessMove);
		else
			throw new exception("IsLegalChessMove: invalid arguments");
	}

	__declspec(dllexport) void RotateChessBoard(chessboard* ChessBoard)
	{
		if (is_valid_chess_board(ChessBoard))
			ChessBoard->rotate();
		else
			throw new exception("RotateChessBoard: invalid arguments");
	}
}