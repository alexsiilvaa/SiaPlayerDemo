using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing.Imaging;

namespace SiaViewer
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        public void SetImage(int width, int height, IntPtr img)
        {
            pictureBox.Image = new Bitmap(width,height,3*width,PixelFormat.Format24bppRgb,img);
        }
    }
}
