using System;
using System.Runtime.InteropServices;

namespace ChessBotConsoleWrapper
{
	public static class ChessBotAPI
	{
		public const byte BoardSize = 8;

		public enum ChessColor : byte
		{
			None,
			White,
			Black
		}

		
		public enum ChessMan : byte
		{
			None,
			King,
			Queen,
			Bishop,
			Knight,
			Rook,
			Pawn
		}

		public enum ChessState : byte
		{
			None,
			Touched,
			Untouched
		}

		public enum ChessStatus : byte
		{
			None,
			Check,
			Mate,
			Stalemate,
			Deadend
		}

		public enum ChessTimer : byte
		{
			None,
			Charged,
			Uncharged
		}

		[StructLayout(LayoutKind.Sequential, Pack = 1)]
		public struct ChessBoardA
		{
			[MarshalAs(UnmanagedType.ByValArray, SizeConst = BoardSize * BoardSize)]
			public ChessCell[] Board;
			[MarshalAs(UnmanagedType.U1)]
			public bool Rotated;
			[MarshalAs(UnmanagedType.U1)]
			public ChessColor NextMove;
			[MarshalAs(UnmanagedType.U1)]
			public ChessStatus Status;
		}

		[StructLayout(LayoutKind.Sequential, Pack = 1)]
		public struct ChessCell
		{
			[MarshalAs(UnmanagedType.U1)]
			public ChessColor Color;
			[MarshalAs(UnmanagedType.U1)]
			public ChessMan Man;
			[MarshalAs(UnmanagedType.U1)]
			public ChessState State;
			[MarshalAs(UnmanagedType.U1)]
			public ChessTimer Timer;
		}

		[Serializable()]
		[StructLayout(LayoutKind.Sequential, Pack = 1)]
		public struct ChessMove
		{
			public ChessPoint From;
			public ChessPoint To;
			[MarshalAs(UnmanagedType.U1)]
			public ChessMan ToPromote;
		}

		[Serializable()]
		[StructLayout(LayoutKind.Sequential, Pack = 1)]
		public struct ChessPoint
		{
			[MarshalAs(UnmanagedType.U1)]
			public byte X;
			[MarshalAs(UnmanagedType.U1)]
			public byte Y;
		}

		[DllImport("chess_engine.dll")]
		public static extern bool ChessBoardMakeMove(ref ChessBoardA ChessBoard, ChessMove ChessMove);

		[DllImport("chess_engine.dll")]
		public static extern ChessMove ChessBotMakeMove(ref ChessBoardA ChessBoard, byte Difficulty, bool Parallel = false);

		[DllImport("chess_engine.dll")]
		public static extern short GetChessBoardEvaluationValue(ref ChessBoardA ChessBoard);

		[DllImport("chess_engine.dll")]
		public static extern byte GetLegalChessMoves(ref ChessBoardA ChessBoard, IntPtr ChessMoves, byte ChessMovesSize);

		[DllImport("chess_engine.dll")]
		public static extern bool GetNewChessBoard(ref ChessBoardA ChessBoard);

		[DllImport("chess_engine.dll")]
		public static extern bool IsLegalChessMove(ref ChessBoardA ChessBoard, ChessMove ChessMove);

		[DllImport("chess_engine.dll")]
		public static extern bool RotateChessBoard(ref ChessBoardA ChessBoard);
	}
}