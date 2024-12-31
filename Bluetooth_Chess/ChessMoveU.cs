using System;
using static ChessBotConsoleWrapper.ChessBotAPI;

namespace ChessBotConsoleWrapper
{
	[Serializable()]
	public class ChessMoveU
	{
		private ChessMove chessMove;

        private static ChessPoint RotateChessPoint(ChessPoint chessPoint)
        {
            ChessPoint newChessPoint = chessPoint;

            newChessPoint.X = (byte)(BoardSize - 1 - chessPoint.X);
            newChessPoint.Y = (byte)(BoardSize - 1 - chessPoint.Y);

            return newChessPoint;
        }

        private static ChessMove RotateChessMove(ChessMove chessMove)
		{
			ChessMove newChessMove = chessMove;

			newChessMove.From = RotateChessPoint(chessMove.From);
			newChessMove.To = RotateChessPoint(chessMove.To);

			return newChessMove;
		}

		public ChessMoveU(ChessMove chessMove, bool rotated)
		{
			this.chessMove = rotated ? RotateChessMove(chessMove) : chessMove;
		}

		public ChessMoveU(ChessMove chessMove, ChessBoard chessBoard) : this(chessMove, chessBoard.Rotated) { }

		public ChessMove this[bool rotated]
		{
			get => rotated ? RotateChessMove(chessMove) : chessMove;
			set => chessMove = rotated ? RotateChessMove(value) : value;
		}

		public ChessMove this[ChessBoard chessBoard]
		{
			get => this[chessBoard.Rotated];
			set => this[chessBoard.Rotated] = value;
		}
	}
}