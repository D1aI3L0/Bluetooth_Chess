using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using static ChessBotConsoleWrapper.ChessBotAPI;

namespace Bluetooth_Chess
{

    public partial class Promotion : Form
    {
        private readonly List<Image> Figures = new List<Image>
        {
          Bluetooth_Chess.Properties.Resources.WQ,
          Bluetooth_Chess.Properties.Resources.WB,
          Bluetooth_Chess.Properties.Resources.WN,
          Bluetooth_Chess.Properties.Resources.WR,
          Bluetooth_Chess.Properties.Resources.WP,
          Bluetooth_Chess.Properties.Resources.BQ,
          Bluetooth_Chess.Properties.Resources.BB,
          Bluetooth_Chess.Properties.Resources.BN,
          Bluetooth_Chess.Properties.Resources.BR,
          Bluetooth_Chess.Properties.Resources.BP,
        };
        public byte figureNumber;
        private readonly Cell[] cells;
        public Promotion(int color)
        {
            InitializeComponent();
            cells = new Cell[5];
            for(byte x = 1; x <= 5; x++)
            {
                cells[x-1] = new Cell((byte)(x-1), 0)
                {
                    Location = new Point(67 * (x-1), 0),
                    Size = new Size(67, 67),
                    FlatStyle = FlatStyle.Flat,
                    BackgroundImageLayout = ImageLayout.Stretch,
                    TabStop = false,
                    BackColor = Color.Transparent,
                    BackgroundImage = Figures[(color-1)*5+x-1]
                };
                cells[x-1].FlatAppearance.BorderSize = 0;
                cells[x-1].FlatAppearance.BorderColor = Color.Black;
                cells[x-1].Click += PressCell;

                Controls.Add(cells[x-1]);
            }
        }

        private void PressCell(object sender, EventArgs e)
        {
            Cell cell = (Cell)sender;
            figureNumber = cell.X;

            DialogResult = DialogResult.OK;
            Close();
        }

        protected override void OnFormClosing(FormClosingEventArgs e)
        {
            base.OnFormClosing(e);

            if (e.CloseReason == CloseReason.WindowsShutDown) return;

            if(DialogResult != DialogResult.OK) return;
        }
    }
}
