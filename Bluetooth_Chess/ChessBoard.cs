using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using static ChessBotConsoleWrapper.ChessBotAPI;

namespace ChessBotConsoleWrapper
{
	public class ChessBoard
	{
		private ChessBotAPI.ChessBoardA chessBoard;

		public ChessCell this[ChessPointU chessPoint] => chessBoard.Board[chessPoint[this].X * BoardSize + chessPoint[this].Y];

		public bool Rotated => chessBoard.Rotated;

		public ChessColor NextMove => chessBoard.NextMove;

		public ChessStatus Status => chessBoard.Status;

		public ChessMoveU ChessBotMakeMove(byte difficulty = 0, bool parallel = false)
		{
			return new ChessMoveU(ChessBotAPI.ChessBotMakeMove(ref chessBoard, difficulty, parallel), this);
		}

		public short GetEvaluationValue()
		{
			return GetChessBoardEvaluationValue(ref chessBoard);
		}

		public unsafe LinkedList<ChessMoveU> GetLegalChessMoves()
		{
			byte movesCount = byte.MaxValue;
			LinkedList<ChessMoveU> movesList = new LinkedList<ChessMoveU>();

			IntPtr chessMoves = Marshal.AllocHGlobal(sizeof(ChessMove) * movesCount);
			movesCount = ChessBotAPI.GetLegalChessMoves(ref chessBoard, chessMoves, movesCount);

			for (byte i = 0; i < movesCount; ++i)
				movesList.AddLast(new ChessMoveU(Marshal.PtrToStructure<ChessMove>(chessMoves + i * sizeof(ChessMove)), this));

			Marshal.FreeHGlobal(chessMoves);
			return movesList;
		}

		public bool IsLegalChessMove(ChessMoveU chessMove)
		{
			return ChessBotAPI.IsLegalChessMove(ref chessBoard, chessMove[this]);
		}

		public void MakeMove(ChessMoveU chessMove)
		{
			ChessBoardMakeMove(ref chessBoard, chessMove[this]);
		}

		public void Rotate()
		{
			RotateChessBoard(ref chessBoard);
		}

		public ChessBoard()
		{
			GetNewChessBoard(ref chessBoard);
		}

		public ChessBoard(ChessBoard chessBoard)
		{
			this.chessBoard = chessBoard.chessBoard;
		}
	}
}