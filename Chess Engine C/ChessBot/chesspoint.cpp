#include "chesspoint.h"

chesspoint::chesspoint()
{
	x = (uint8_t)0;
	y = (uint8_t)0;
}

chesspoint::chesspoint(uint8_t __x, uint8_t __y)
{
	x = __x;
	y = __y;
}

void chesspoint::rotate()
{
	x = BOARD_SIZE - 1 - x;
	y = BOARD_SIZE - 1 - y;
}