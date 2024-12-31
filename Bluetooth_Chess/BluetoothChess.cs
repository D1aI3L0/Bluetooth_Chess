using ChessBotConsoleWrapper;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace Bluetooth_Chess
{
	class BluetoothChess
	{
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Game());
        }
    }
}