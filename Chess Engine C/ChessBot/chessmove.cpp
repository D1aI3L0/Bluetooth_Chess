#include "chessmove.h"

chessmove::chessmove()
{
	from = chesspoint();
	to = chesspoint();
	to_promote = chessman::none;
}

chessmove::chessmove(const chessmove& move)
{
	*this = move;
}

chessmove::chessmove(const chessmove& move, chessman man)
{
	*this = move;
	to_promote = man;
}

chessmove::chessmove(chesspoint __from, chesspoint __to)
{
	from = __from;
	to = __to;
	to_promote = chessman::none;
}

chessmove::chessmove(chesspoint __from, chesspoint __to, chessman man)
{
	from = __from;
	to = __to;
	to_promote = man;
}

chessmove::chessmove(uint8_t from_x, uint8_t from_y, uint8_t to_x, uint8_t to_y)
{
	from = chesspoint(from_x, from_y);
	to = chesspoint(to_x, to_y);
	to_promote = chessman::none;
}

chessmove::chessmove(uint8_t from_x, uint8_t from_y, uint8_t to_x, uint8_t to_y, chessman man)
{
	from = chesspoint(from_x, from_y);
	to = chesspoint(to_x, to_y);
	to_promote = man;
}

void chessmove::rotate()
{
	from.rotate();
	to.rotate();
}

void chessmove::swap()
{
	chesspoint point = from;
	from = to;
	to = point;
}