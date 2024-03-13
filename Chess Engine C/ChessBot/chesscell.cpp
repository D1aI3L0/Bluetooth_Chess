#include "chesscell.h"

chesscell::chesscell()
{
	color = chesscolor::none;
	man = chessman::none;
	state = chessstate::none;
	timer = chesstimer::none;
}

chesscell::chesscell(chesscolor __color, chessman __man)
{
	color = __color;
	man = __man;
	state = chessstate::untouched;
	timer = chesstimer::uncharged;
}

void chesscell::set_empty()
{
	color = chesscolor::none;
	man = chessman::none;
	state = chessstate::none;
	timer = chesstimer::none;
}