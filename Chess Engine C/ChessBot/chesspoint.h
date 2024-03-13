#pragma once

#include <stdint.h>
#include "chessboardsize.h"

struct chesspoint
{
public:
	uint8_t x;
	uint8_t y;

	chesspoint();
	chesspoint(uint8_t __x, uint8_t __y);

	void rotate();
};