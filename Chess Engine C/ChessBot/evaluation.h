#pragma once

#include <stdint.h>
#include "chessboardsize.h"

namespace evaluation
{
	extern const int16_t middle_game_king_table[BOARD_SIZE][BOARD_SIZE];
	extern const int16_t end_game_king_table[BOARD_SIZE][BOARD_SIZE];

	extern const int16_t middle_game_queen_table[BOARD_SIZE][BOARD_SIZE];
	extern const int16_t end_game_queen_table[BOARD_SIZE][BOARD_SIZE];

	extern const int16_t middle_game_bishop_table[BOARD_SIZE][BOARD_SIZE];
	extern const int16_t end_game_bishop_table[BOARD_SIZE][BOARD_SIZE];

	extern const int16_t middle_game_knight_table[BOARD_SIZE][BOARD_SIZE];
	extern const int16_t end_game_knight_table[BOARD_SIZE][BOARD_SIZE];

	extern const int16_t middle_game_rook_table[BOARD_SIZE][BOARD_SIZE];
	extern const int16_t end_game_rook_table[BOARD_SIZE][BOARD_SIZE];

	extern const int16_t middle_game_pawn_table[BOARD_SIZE][BOARD_SIZE];
	extern const int16_t end_game_pawn_table[BOARD_SIZE][BOARD_SIZE];

	extern const int16_t* middle_game_tables[7];
	extern const int16_t* end_game_tables[7];

	extern const int16_t game_phase_score_table[7];
	extern const int16_t men_score_table[7];

	extern const int16_t check;

	extern const int16_t mate;
}