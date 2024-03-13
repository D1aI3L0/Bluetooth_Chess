#pragma once

#include <stdint.h>

enum class chessstate : uint8_t
{
	none,
	touched,
	untouched
};