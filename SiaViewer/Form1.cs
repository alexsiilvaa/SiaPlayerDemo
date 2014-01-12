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
        public const int FMT_BGR_24BPP = 0;
        public const int FMT_JPEG = 1;

        [DllImport("SiaPlayer.dll")]
        private static extern void StartDecoding(string url, double userFps, 
            int fmt_out_type, FrameDecodedCallback callback, ref IntPtr videoId);        

        [DllImport("SiaPlayer.dll")]
        private static extern void StopDecoding(ref IntPtr videoId);

        [DllImport("SiaPlayer.dll")]
        private static extern void ChangeFps(double fps, IntPtr videoId);

        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        private delegate void FrameDecodedCallback(int FrameWidth, int FrameHeight, 
            IntPtr FrameData, double estFps, IntPtr videoId);

        private FrameDecodedCallback callback;
        private IntPtr videoInstanceId;

        public Form1()
        {
            callback = (width, height, data, estFps, videoId) => {
                Console.WriteLine("Got frame, width: {0} height: {1} [est. FPS: {2}]", 
                    width, height, estFps);
                SetRuntimeData(width, height, data, estFps);
            };
            InitializeComponent();

        }

        public void SetRuntimeData(int width, int height, IntPtr img, double estFps)
        {
            if (InvokeRequired)
            {
                // We're on a thread other than the GUI thread
                Invoke(new MethodInvoker(() => SetRuntimeData(width, height, img, estFps)));
                return;
            }
            pictureBox.Image = new Bitmap(width,height,3*width,PixelFormat.Format24bppRgb,img);
            labelEstFps.Text = string.Format("{0:N2}", estFps);
        }

        private void btnStart_Click(object sender, EventArgs e)
        {
            if (IntPtr.Zero == videoInstanceId)
            {
                StartDecoding(textUrl.Text, (double)fpsValue.Value, FMT_BGR_24BPP, callback, ref videoInstanceId);
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

        private void fpsValue_ValueChanged(object sender, EventArgs e)
        {
            ChangeFps((double)fpsValue.Value, this.videoInstanceId); 
        }

    }
}
