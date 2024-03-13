#pragma once

#include <stdint.h>

enum class chessstatus : uint8_t
{
	none,
	check,
	mate,
	stalemate,
	deadend
};