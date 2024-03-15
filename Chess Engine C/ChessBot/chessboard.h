#pragma once

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

	void check_and_push_promotions(stack<chessmove> moves, chessmove move) const;

	bool check_no_check(chessmove move) const;

	bool is_check() const;

	bool is_deadend() const;

	bool is_self_check() const;

	bool is_threaten(chesscolor color, chesspoint point) const;

	void make_test_move(chessmove move);

public:
	chessboard();

	bool check_move(chessmove move) const;

	int16_t evaluate() const;

	chessstatus get_board_status() const;

	stack<chessmove> get_moves() const;

	chesscolor get_next_move_color() const;

	bool is_rotated() const;

	stack<chessmove> make_move(chessmove move);

	void rotate();
};