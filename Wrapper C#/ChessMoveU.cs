using static ChessBotConsoleWrapper.ChessBotAPI;

namespace ChessBotConsoleWrapper
{
	public class ChessMoveU
	{
		private ChessMove chessMove;

		public static ChessMove RotateChessMove(ChessMove chessMove)
		{
			ChessMove newChessMove = chessMove;

			newChessMove.From = ChessPointU.RotateChessPoint(chessMove.From);
			newChessMove.To = ChessPointU.RotateChessPoint(chessMove.To);

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