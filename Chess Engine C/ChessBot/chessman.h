#pragma once

#include <stdint.h>

enum class chessman : uint8_t
{
	none,
	king,
	queen,
	bishop,
	knight,
	rook,
	pawn
};