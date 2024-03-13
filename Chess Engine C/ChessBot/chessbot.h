#pragma once

#include <Windows.h>
#include <omp.h>
#include <stack>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "chessboard.h"

namespace chessbot
{
	// Alpha-beta pruning minimum/maximum moves evaluation recursive function
	int16_t ab_minimax(const chessboard& board, chessmove move, int16_t alpha, int16_t beta, uint8_t level);

	// Return the best move for given boardstate
	chessmove make_move(const chessboard& board, uint8_t level, bool parallel = false);
}