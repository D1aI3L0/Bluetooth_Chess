#include <stack>
#include <stdint.h>
#include <Windows.h>
#include "chessboard.h"
#include "chessbot.h"
#include "chessmove.h"

using namespace std;

chessmove illegal_chess_move = chessmove((uint8_t)255, (uint8_t)255, (uint8_t)255, (uint8_t)255);

bool is_valid_chess_move(chessmove move) noexcept
{
	return move.from.x < BOARD_SIZE && move.from.y < BOARD_SIZE && move.to.x < BOARD_SIZE && move.to.y < BOARD_SIZE
		&& (move.to_promote == chessman::none || move.to_promote == chessman::queen || move.to_promote == chessman::bishop
			|| move.to_promote == chessman::knight || move.to_promote == chessman::rook || move.to_promote == chessman::pawn);
}

extern "C"
{
	__declspec(dllexport) bool ChessBoardMakeMove(chessboard* ChessBoard, chessmove ChessMove) noexcept
	{
		if (ChessBoard == nullptr || !is_valid_chess_move(ChessMove))
			return false;

		try
		{
			ChessBoard->make_move(ChessMove);
			return true;
		}
		catch (...)
		{
			return false;
		}
	}

	__declspec(dllexport) chessmove ChessBotMakeMove(const chessboard* ChessBoard, uint8_t Difficulty, bool Parallel = false) noexcept
	{
		if (ChessBoard == nullptr)
			return illegal_chess_move;

		try
		{
			return chessbot::make_move(*ChessBoard, Difficulty, Parallel);
		}
		catch (...)
		{
			return illegal_chess_move;
		}
	}

	__declspec(dllexport) int16_t GetChessBoardEvaluationValue(const chessboard* ChessBoard) noexcept
	{
		if (ChessBoard == nullptr)
			return (int16_t)0;

		try
		{
			return ChessBoard->evaluate();
		}
		catch (...)
		{
			return (int16_t)0;
		}
	}

	__declspec(dllexport) uint8_t GetLegalChessMoves(const chessboard* ChessBoard, chessmove* ChessMoves, uint8_t ChessMovesSize) noexcept
	{
		if (ChessBoard == nullptr || ChessMoves == nullptr)
			return (uint8_t)0;

		try
		{
			stack<chessmove> moves = ChessBoard->get_moves();
			uint8_t moves_size = moves.size();

			if (moves_size <= ChessMovesSize)
				try
				{
					for (uint8_t i = 0; i < moves_size; ++i, moves.pop())
						ChessMoves[i] = moves.top();
				}
				catch (...) {}

			return moves_size;
		}
		catch (...)
		{
			return (uint8_t)0;
		}
	}

	__declspec(dllexport) bool GetNewChessBoard(chessboard* ChessBoard) noexcept
	{
		if (ChessBoard == nullptr)
			return false;

		try
		{
			*ChessBoard = chessboard();
			return true;
		}
		catch (...)
		{
			return false;
		}
	}

	__declspec(dllexport) bool IsLegalChessMove(const chessboard* ChessBoard, chessmove ChessMove) noexcept
	{
		if (ChessBoard == nullptr || !is_valid_chess_move(ChessMove))
			return false;

		try
		{
			return ChessBoard->check_move(ChessMove);
		}
		catch (...)
		{
			return false;
		}
	}

	__declspec(dllexport) bool RotateChessBoard(chessboard* ChessBoard) noexcept
	{
		if (ChessBoard == nullptr)
			return false;

		try
		{
			ChessBoard->rotate();
			return true;
		}
		catch (...)
		{
			return false;
		}
	}
}