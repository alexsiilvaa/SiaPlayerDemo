using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace SiaViewer
{
    static class Program
    {
        [DllImport("C:\\dev\\siavid\\SiaPlayerDemo\\Debug\\SiaPlayer.dll")]
        public static extern void StartDecoding(string url);

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            StartDecoding("rtsp://admin:admin@169.254.0.2:8554/PSIA/Streaming/channels/0");
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Form1());
        }
    }
}
