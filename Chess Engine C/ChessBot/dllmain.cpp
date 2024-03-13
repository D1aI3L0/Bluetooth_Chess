#include <Windows.h>
#include "chessbot.h"
#include "chessboard.h"
#include "chesscell.h"
#include "chessmove.h"
#include "chesspoint.h"

using namespace std;

extern "C"
{
	__declspec(dllexport) int counter()
	{
		static int i = 0;
		return i++;
	}
}