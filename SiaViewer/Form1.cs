using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing.Imaging;
using System.Runtime.InteropServices;

namespace SiaViewer
{
    public partial class Form1 : Form
    {
        [DllImport("SiaPlayer.dll")]
        private static extern void StartDecoding(string url, FrameDecodedCallback callback, ref IntPtr videoId);

        [DllImport("SiaPlayer.dll")]
        private static extern void StopDecoding(ref IntPtr videoId);

        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        private delegate void FrameDecodedCallback(int FrameWidth, int FrameHeight, IntPtr FrameData);

        private FrameDecodedCallback callback;
        private IntPtr videoInstanceId;

        public Form1()
        {
            callback = (width, height, data) => {
                Console.WriteLine("Got frame, width: {0} height: {1}", width, height);
                SetImage(width, height, data);
            };
            InitializeComponent();

        }

        public void SetImage(int width, int height, IntPtr img)
        {
            if (InvokeRequired)
            {
                // We're on a thread other than the GUI thread
                Invoke(new MethodInvoker(() => SetImage(width, height, img)));
                return;
            }
            pictureBox.Image = new Bitmap(width,height,3*width,PixelFormat.Format24bppRgb,img);
        }

        private void btnStart_Click(object sender, EventArgs e)
        {
            if (IntPtr.Zero == videoInstanceId)
            {
                StartDecoding(textUrl.Text, callback, ref videoInstanceId);
            }
        }

        private void btnStop_Click(object sender, EventArgs e)
        {
            if (IntPtr.Zero != videoInstanceId)
            {
                StopDecoding(ref videoInstanceId);
                pictureBox.Image = null;
            }
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            btnStop_Click(sender, e);
        }
    }
}
