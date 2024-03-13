#pragma once

#include <stdint.h>
#include "chessboardsize.h"
#include "chessman.h"
#include "chesspoint.h"

struct chessmove
{
public:
	chesspoint from;
	chesspoint to;
	chessman to_promote;

	chessmove();
	chessmove(const chessmove& move);
	chessmove(const chessmove& move, chessman man);
	chessmove(chesspoint __from, chesspoint __to);
	chessmove(chesspoint __from, chesspoint __to, chessman man);
	chessmove(uint8_t from_x, uint8_t from_y, uint8_t to_x, uint8_t to_y);
	chessmove(uint8_t from_x, uint8_t from_y, uint8_t to_x, uint8_t to_y, chessman man);

	void rotate();
	void swap();
};