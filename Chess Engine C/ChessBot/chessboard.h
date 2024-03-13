#pragma once

#include <Windows.h>
#include <stack>
#include "chessboardsize.h"
#include "chesscell.h"
#include "chessmove.h"
#include "chesspoint.h"
#include "chessstatus.h"
#include "evaluation.h"

using namespace std;

inline chesscolor operator!(const chesscolor& color);

inline chessstate operator!(const chessstate& state);

inline chesstimer operator!(const chesstimer& timer);

struct chessboard
{
private:
	chesscell board[BOARD_SIZE][BOARD_SIZE] =
	{
		{
		chesscell(chesscolor::white, chessman::rook), chesscell(chesscolor::white, chessman::knight),
		chesscell(chesscolor::white, chessman::bishop), chesscell(chesscolor::white, chessman::queen),
		chesscell(chesscolor::white, chessman::king), chesscell(chesscolor::white, chessman::bishop),
		chesscell(chesscolor::white, chessman::knight), chesscell(chesscolor::white, chessman::rook)
		},
		{
		chesscell(chesscolor::white, chessman::pawn), chesscell(chesscolor::white, chessman::pawn),
		chesscell(chesscolor::white, chessman::pawn), chesscell(chesscolor::white, chessman::pawn),
		chesscell(chesscolor::white, chessman::pawn), chesscell(chesscolor::white, chessman::pawn),
		chesscell(chesscolor::white, chessman::pawn), chesscell(chesscolor::white, chessman::pawn)
		},
		{
		chesscell(), chesscell(), chesscell(), chesscell(), chesscell(), chesscell(), chesscell(), chesscell()
		},
		{
		chesscell(), chesscell(), chesscell(), chesscell(), chesscell(), chesscell(), chesscell(), chesscell()
		},
		{
		chesscell(), chesscell(), chesscell(), chesscell(), chesscell(), chesscell(), chesscell(), chesscell()
		},
		{
		chesscell(), chesscell(), chesscell(), chesscell(), chesscell(), chesscell(), chesscell(), chesscell()
		},
		{
		chesscell(chesscolor::black, chessman::pawn), chesscell(chesscolor::black, chessman::pawn),
		chesscell(chesscolor::black, chessman::pawn), chesscell(chesscolor::black, chessman::pawn),
		chesscell(chesscolor::black, chessman::pawn), chesscell(chesscolor::black, chessman::pawn),
		chesscell(chesscolor::black, chessman::pawn), chesscell(chesscolor::black, chessman::pawn)
		},
		{
		chesscell(chesscolor::black, chessman::rook), chesscell(chesscolor::black, chessman::knight),
		chesscell(chesscolor::black, chessman::bishop), chesscell(chesscolor::black, chessman::queen),
		chesscell(chesscolor::black, chessman::king), chesscell(chesscolor::black, chessman::bishop),
		chesscell(chesscolor::black, chessman::knight), chesscell(chesscolor::black, chessman::rook)
		}
	};

	bool rotated = false;
	chesscolor next_move = chesscolor::white;
	chessstatus status = chessstatus::none;

	// Push all legal pawn promotions to stack
	void check_and_push_promotions(stack<chessmove> moves,chessmove move) const;

	// True if no self check after making given move, else false
	bool check_no_check(chessmove move) const;

	// True if next move king is treaten, else false
	bool is_check() const;

	// True if deadend, else false
	bool is_deadend() const;

	// True if prev move king is treaten, else false
	bool is_self_check() const;

	// True if given cell is treaten by any men of given color
	bool is_threaten(chesscolor color,chesspoint point) const;

	// Make move without calculating board status
	void make_test_move(chessmove move);

public:
	chessboard();

	// True if given move is legal, else false
	bool check_move(chessmove move) const;

	// Signed 16-bit evaluation value, positive value means white advantage and vice versa
	int16_t evaluate() const;

	// All legal moves
	stack<chessmove> get_moves() const;

	// Make a move on the board, return all legal moves
	stack<chessmove> make_move(chessmove move);

	// Next move color
	chesscolor next_move_color() const;

	// Rotate board 180 deg
	void rotate();
};