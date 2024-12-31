using ChessBotConsoleWrapper;
using InTheHand.Net;
using InTheHand.Net.Sockets;
using System;
using System.Collections.Generic;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Runtime.Serialization.Formatters.Binary;
using System.Threading;
using System.Windows.Forms;
using static ChessBotConsoleWrapper.ChessBotAPI;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;

enum GameType
{
    OfflineWithBot,
    OfflineWithoutBot,
    Online
}

namespace Bluetooth_Chess
{
    public partial class Game : Form
    {
        private delegate void Delegate();
        private readonly List<Cell> cells = new List<Cell>();
        private readonly int size;
        private bool rotated = false;
        private ChessBoard board;
        private bool pressed = false;
        private List<ChessMoveU> moves;

        private readonly object cellLock = new object();

        private GameType gameType;

        private ChessColor myColor;

        private readonly List<Image> Figures = new List<Image>
        {
          Bluetooth_Chess.Properties.Resources.WK,
          Bluetooth_Chess.Properties.Resources.WQ,
          Bluetooth_Chess.Properties.Resources.WB,
          Bluetooth_Chess.Properties.Resources.WN,
          Bluetooth_Chess.Properties.Resources.WR,
          Bluetooth_Chess.Properties.Resources.WP,
          Bluetooth_Chess.Properties.Resources.BK,
          Bluetooth_Chess.Properties.Resources.BQ,
          Bluetooth_Chess.Properties.Resources.BB,
          Bluetooth_Chess.Properties.Resources.BN,
          Bluetooth_Chess.Properties.Resources.BR,
          Bluetooth_Chess.Properties.Resources.BP,
          Bluetooth_Chess.Properties.Resources.None,
        };

        public Game()
        {
            InitializeComponent();
            size = desk.Size.Height / 8;
            PrintCells();
            desk.Visible = false;
        }

        private void PrintCells()
        {
            ClearCells();
            board = new ChessBoard();

            for (byte x = 0; x < 8; x++)
                for (byte y = 0; y < 8; y++)
                {
                    cells.Add(new Cell(x, y)
                    {
                        Location = new Point(y * size, (7 - x) * size),
                        Size = new Size(size, size),
                        FlatStyle = FlatStyle.Flat,
                        BackgroundImageLayout = ImageLayout.Stretch,
                        TabStop = false,
                        BackColor = Color.Transparent,
                    }
                    );
                    cells[x * 8 + y].FlatAppearance.BorderSize = 0;
                    cells[x * 8 + y].FlatAppearance.BorderColor = Color.Black;
                    cells[x * 8 + y].Click += CellClick;

                    desk.Controls.Add(cells[x * 8 + y]);
                }
            PrintFigures();
        }

        private void ClearCells()
        {
            for(int i = 0; i < cells.Count; i++)
                cells[i].Dispose();
            cells.Clear();
        }

        private void PrintFigures()
        {
            for (int x = 0; x < 8; x++)
                for (int y = 0; y < 8; y++)
                {
                    ChessCell cell = board[new ChessPointU(new ChessPoint() { X = (byte)x, Y = (byte)y }, board)];
                    cells[x * 8 + y].BackgroundImage = Figures[cell.Man == ChessMan.None ? 12 : ((int)cell.Color - 1) * 6 + (int)cell.Man-1];
                }
        }

        public void RotateDesk()
        {
            Image Desk = Bluetooth_Chess.Properties.Resources.ChessDesk;
            if (!rotated)
                Desk.RotateFlip(RotateFlipType.Rotate180FlipNone);
            deskFull.BackgroundImage = Desk;

            if (!rotated)  rotated = true; 
            else  rotated = false; 

            board.Rotate();
        }

        private void CellClick(object sender, EventArgs e)
        {
            new Thread(() => { PressCell(sender); }).Start();
        }

        private void PressCell(object sender)
        {
            lock (cellLock)
            {
                if (gameType == GameType.Online)
                    if (board.NextMove != myColor)
                        return;

                if (board.Status == ChessStatus.Mate || board.Status == ChessStatus.Stalemate)
                    return;

                Cell cell = (Cell)sender;

                if (!pressed)
                {
                    if (board[new ChessPointU(new ChessPoint() { X = cell.X, Y = cell.Y }, board)].Color != board.NextMove ||
                    board[new ChessPointU(new ChessPoint() { X = cell.X, Y = cell.Y }, board)].Man == ChessMan.None)
                        return;

                    if (!GetMoves(cell))
                        return;

                    pressed = true;

                    ShowAvailableMoves();
                }
                else
                {
                    if (board[new ChessPointU(new ChessPoint() { X = cell.X, Y = cell.Y }, board)].Color == board.NextMove)
                    {
                        if (!GetMoves(cell))
                        {
                            pressed = false;
                            ShowAvailableMoves(false);
                        }
                        else
                            ShowAvailableMoves();
                        return;
                    }

                    foreach (var move in moves)
                    {
                        if (move[board].To.X == cell.X && move[board].To.Y == cell.Y)
                        {
                            ChessMoveU movePr;
                            if (CheckPromotion(move))
                            {
                                Promotion promotion = new Promotion((int)board.NextMove);
                                promotion.ShowDialog();
                                byte figureNumber = promotion.figureNumber;
                                movePr = new ChessMoveU(new ChessMove { From = move[board].From, To = move[board].To, ToPromote = (ChessMan)(figureNumber + 2) }, board);
                            }
                            else
                                movePr = move;

                            if (gameType == GameType.Online)
                                new Thread(()=>MakeOnlineMove(movePr)).Start();

                            board.MakeMove(movePr);

                            ShowAvailableMoves(false);
                            PrintFigures();
                            pressed = false;

                            if (gameType == GameType.OfflineWithBot)
                                BotMakeMove();

                            if (board.Status == ChessStatus.Mate || board.Status == ChessStatus.Stalemate)
                            {
                                return;
                            }

                            break;
                        }
                    }

                }
            }
        }

        private bool GetMoves(Cell cell)
        {
            moves = board.GetLegalChessMoves().Where((ChessMoveU move) => { return move[board].From.X == (byte)cell.X && move[board].From.Y == (byte)cell.Y; }).ToList();

            if (moves.Count <= 0)
                return false;

            return true;
        }

        private bool CheckPromotion(ChessMoveU move)
        {
            if (board[new ChessPointU(new ChessPoint() { X = move[board].From.X, Y = move[board].From.Y }, board)].Man == ChessMan.Pawn)
            {
                if (!rotated)
                {
                    if (board.NextMove == ChessColor.White)
                    {
                        if (move[board].From.X == 6 && move[board].To.X == 7) return true;
                    }
                    else
                    {
                        if (move[board].From.X == 1 && move[board].To.X == 0) return true;
                    }
                }
                else
                {
                    if (board.NextMove == ChessColor.Black)
                    {
                        if (move[board].From.X == 6 && move[board].To.X == 7) return true;
                    }
                    else
                    {
                        if (move[board].From.X == 1 && move[board].To.X == 0) return true;
                    }
                }
            }

            return false;
        }

        private void BotMakeMove()
        {
            ChessMoveU move = board.ChessBotMakeMove(4, true);
            board.MakeMove(move);
            PrintFigures();
        }


        private void MakeOnlineMove(ChessMoveU move)
        {
            BluetoothTransaction.SendMove(ToByteArray(move));
            WaitMove();
        }

        private void WaitMove()
        {
            ChessMoveU move = ToChessMoveU(BluetoothTransaction.ReceiveMove());
            board.MakeMove(move);
            PrintFigures();
        }

        private void ShowAvailableMoves(bool showNew = true)
        {
            BeginInvoke(new Delegate(() =>
            {
                foreach (Cell cell in cells)
                    cell.FlatAppearance.BorderSize = 0;

                if (showNew)
                    foreach (var move in moves)
                        cells[move[board].To.X * 8 + move[board].To.Y].FlatAppearance.BorderSize = 2;
            }));
        }


        private void ExitButton_click(object sender, EventArgs e)
        {
            Application.Exit();
        }
             
        private void RefreshDesk()
        {
            board = new ChessBoard();
            ShowAvailableMoves(false);
            PrintFigures();
        }


        private byte[] ToByteArray(object source)
        {
            var formatter = new BinaryFormatter();
            using (var stream = new MemoryStream())
            {
                formatter.Serialize(stream, source);
                return stream.ToArray();
            }
        }

        private ChessMoveU ToChessMoveU(byte[] bytes)
        {
            BinaryFormatter formatter = new BinaryFormatter();
            using (MemoryStream stream = new MemoryStream(bytes))
            {
                return (ChessMoveU)formatter.Deserialize(stream);
            }
        }


        private void OnlineGame_click(object sender, EventArgs e)
        {
            gameType = GameType.Online;

            GameTypePanel.Visible = false;
            ConnectTypePanel.Visible = true;
        }

        private void OfflineWithBot_Click(object sender, EventArgs e)
        {
            gameType = GameType.OfflineWithBot;

            desk.Visible = true;
            GamePanel.Visible = true;
            GameTypePanel.Visible = false;
        }

        private void OfflineWithoutBot_Click(object sender, EventArgs e)
        {
            gameType = GameType.OfflineWithoutBot;

            desk.Visible = true;
            GamePanel.Visible = true;
            GameTypePanel.Visible = false;
        }


        private void HostRoom_Click(object sender, EventArgs e)
        {
            if (!BluetoothTransaction.StartServer())
                return;
            ConnectTypePanel.Visible = false;
            myColor = ChessColor.Black;
            GamePanel.Visible = true;
            desk.Visible = true;
            new Thread(() => WaitMove()).Start();
        }

        private void ConnectToOther_Click(object sender, EventArgs e)
        {
            ConnectTypePanel.Visible = false;
            ConnetToOtherPanel.Visible = true;
            ShowDevs(ConnectDevsList, ConnetToOtherPanel);

            myColor = ChessColor.White;
        }

        private void ShowDevs(System.Windows.Forms.ListView listView, Panel panel)
        {
            BluetoothTransaction.devsScanThread = new Thread(() => { ShowDevsList(listView, panel); });
            BluetoothTransaction.devsScanThread.Start();
        }

        private void Back_Click(object sender, EventArgs e)
        {
            ConnectTypePanel.Visible = false;
            GameTypePanel.Visible = true;
        }

        private void ConnectToOtherConfirm_Click(object sender, EventArgs e)
        {
            if (ConnectDevsList.SelectedItems.Count == 1)
            {
                BluetoothAddress selectedDevAddress = BluetoothAddress.Parse(ConnectDevsList.SelectedItems[0].SubItems[1].Text);

                lock (BluetoothTransaction.devsSync)
                    BluetoothTransaction.selectedDev = BluetoothTransaction.devs.First(dev => dev.DeviceAddress == selectedDevAddress);

                if(BluetoothTransaction.StartClient(BluetoothTransaction.selectedDev))
                {
                    BluetoothTransaction.devsScanThread.Abort();
                    ConnetToOtherPanel.Visible = false;
                    GamePanel.Visible = true;
                    desk.Visible = true;
                }
            }
        }

        private void CancelConnectToOther_Click(object sender, EventArgs e)
        {
            BluetoothTransaction.Stop();
            BluetoothTransaction.devsScanThread.Abort();
            ConnectDevsList.Items.Clear();
            ConnectTypePanel.Visible = true;
            ConnetToOtherPanel.Visible = false;
            myColor = ChessColor.None;
        }

        private void ShowDevsList(System.Windows.Forms.ListView listView, Panel panel)
        {
            ConnectDevsList.BeginInvoke(new Delegate(() => { panel.Visible = true; }));

            BluetoothTransaction.LocalClientRefresh();

            while (true)
            {
                IReadOnlyCollection<BluetoothDeviceInfo> list;

                lock (BluetoothTransaction.localClientSync)
                    list = BluetoothTransaction.localClient.DiscoverDevices();

                lock (BluetoothTransaction.devsSync)
                {
                    BluetoothTransaction.devs.Clear();
                    foreach (BluetoothDeviceInfo dev in list)
                        BluetoothTransaction.devs.Add(dev);
                }

                listView.BeginInvoke(new Delegate(() =>
                {
                    listView.Items.Clear();
                    foreach (BluetoothDeviceInfo dev in BluetoothTransaction.devs)
                        listView.Items.Add(new ListViewItem(new[] { dev.DeviceName, dev.DeviceAddress.ToString() }, dev.Authenticated ? ConnectDevsList.Groups[0] : ConnectDevsList.Groups[1]));
                }));

                Thread.Sleep(5000);
            }
        }

        private void Game_FormClosed(object sender, FormClosedEventArgs e)
        {
            BluetoothTransaction.Stop();
            BluetoothTransaction.devsScanThread.Abort();
        }

        private void DisconnectB_Click(object sender, EventArgs e)
        {
            myColor = ChessColor.None;
            GamePanel.Visible = false;
            GameTypePanel.Visible = true;

            BluetoothTransaction.Stop();
            RefreshDesk();
            desk.Visible = false; 
            rotated = false;
        }

        private void RotateDeskB_Click(object sender, EventArgs e)
        {
            RotateDesk();
            PrintFigures();
        }
        protected override void OnFormClosing(FormClosingEventArgs e)
        {
            base.OnFormClosing(e);

            BluetoothTransaction.Stop();
            BluetoothTransaction.devsScanThread?.Abort();
        }
    }
}
