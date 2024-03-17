using System;
using System.Collections.Generic;
using static ChessBotConsoleWrapper.ChessBotAPI;

namespace ChessBotConsoleWrapper
{
	class Program
	{
		static void Main(string[] args)
		{
			ChessBoard chessBoard = new ChessBoard();

			bool _false = chessBoard.IsLegalChessMove(new ChessMoveU(new ChessMove()
			{
				From = new ChessPoint() { X = 0, Y = 0 },
				To = new ChessPoint() { X = 0, Y = 0 },
				ToPromote = ChessMan.Queen
			}, chessBoard));

			chessBoard.Rotate();

			chessBoard.MakeMove(chessBoard.ChessBotMakeMove(4, true));

			int eval = chessBoard.GetEvaluationValue();

			LinkedList<ChessMoveU> list = chessBoard.GetLegalChessMoves();

			Console.ReadKey();
		}
	}
}