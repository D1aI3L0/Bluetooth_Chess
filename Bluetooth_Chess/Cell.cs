using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Bluetooth_Chess
{
    public class Cell: Button
    {
        public byte X;
        public byte Y;
        public Cell(byte _x, byte _y) { X = _x; Y = _y; }
    }
}
