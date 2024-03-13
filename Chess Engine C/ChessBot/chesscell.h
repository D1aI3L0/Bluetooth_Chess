#pragma once

#include "chesscolor.h"
#include "chessman.h"
#include "chessstate.h"
#include "chesstimer.h"

struct chesscell
{
public:
	chesscolor color;
	chessman man;
	chessstate state;
	chesstimer timer;

	chesscell();
	chesscell(chesscolor __color, chessman __man);

	void set_empty();
};