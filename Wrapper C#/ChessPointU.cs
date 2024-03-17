using static ChessBotConsoleWrapper.ChessBotAPI;

namespace ChessBotConsoleWrapper
{
	public class ChessPointU
	{
		private ChessPoint chessPoint;

		public static ChessPoint RotateChessPoint(ChessPoint chessPoint)
		{
			ChessPoint newChessPoint = chessPoint;

			newChessPoint.X = (byte)(BoardSize - 1 - chessPoint.X);
			newChessPoint.Y = (byte)(BoardSize - 1 - chessPoint.Y);

			return newChessPoint;
		}

		public ChessPointU(ChessPoint chessPoint, bool rotated)
		{
			this.chessPoint = rotated ? RotateChessPoint(chessPoint) : chessPoint;
		}

		public ChessPointU(ChessPoint chessPoint, ChessBoard chessBoard) : this(chessPoint, chessBoard.Rotated) { }

		public ChessPoint this[bool rotated]
		{
			get => rotated ? RotateChessPoint(chessPoint) : chessPoint;
			set => chessPoint = rotated ? RotateChessPoint(value) : value;
		}

		public ChessPoint this[ChessBoard chessBoard]
		{
			get => this[chessBoard.Rotated];
			set => this[chessBoard.Rotated] = value;
		}
	}
}