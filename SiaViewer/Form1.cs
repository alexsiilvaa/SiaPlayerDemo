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
using System.IO;
using System.Windows.Media.Imaging;

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
            IntPtr FrameData, int size, double estFps, IntPtr videoId);

        private FrameDecodedCallback callback;
        private IntPtr videoInstanceId;

        public Form1()
        {
            callback = (width, height, data, size, estFps, videoId) => {
                Console.WriteLine("Got frame, width: {0} height: {1} [est. FPS: {2}]", 
                    width, height, estFps);
                SetRuntimeData(width, height, data, size, estFps);
            };
            InitializeComponent();
            comboType.SelectedIndex = FMT_JPEG;
        }

        public void SetRuntimeData(int width, int height, IntPtr img, int size, double estFps)
        {
            if (InvokeRequired)
            {
                // We're on a thread other than the GUI thread
                Invoke(new MethodInvoker(() => SetRuntimeData(width, height, img, size, estFps)));
                return;
            }
            switch (comboType.SelectedIndex)
            {
                case FMT_BGR_24BPP:
                    pictureBox.Image = new Bitmap(width, height, 3 * width, PixelFormat.Format24bppRgb, img);
                    break;
                case FMT_JPEG:
                    byte[] img_data = new byte[size];
                    Marshal.Copy(img, img_data, 0, size);
                    using (Stream stream = new MemoryStream(img_data)) {
                        pictureBox.Image = JpegToBitmap(stream);
                    }
                    break;
            }

            labelEstFps.Text = string.Format("{0:N2}", estFps);
        }

        private Bitmap JpegToBitmap(Stream jpg)
        {
            JpegBitmapDecoder ldDecoder = new JpegBitmapDecoder(jpg, BitmapCreateOptions.None, BitmapCacheOption.None);
            BitmapFrame lfFrame = ldDecoder.Frames[0];
            Bitmap lbmpBitmap = new Bitmap(lfFrame.PixelWidth, lfFrame.PixelHeight);
            Rectangle lrRect = new Rectangle(0, 0, lbmpBitmap.Width, lbmpBitmap.Height);
            BitmapData lbdData = lbmpBitmap.LockBits(lrRect, ImageLockMode.WriteOnly, (lfFrame.Format.BitsPerPixel == 24 ? PixelFormat.Format24bppRgb : PixelFormat.Format32bppArgb));
            lfFrame.CopyPixels(System.Windows.Int32Rect.Empty, lbdData.Scan0, lbdData.Height * lbdData.Stride, lbdData.Stride);
            lbmpBitmap.UnlockBits(lbdData);
            return lbmpBitmap;
        }

        private void btnStart_Click(object sender, EventArgs e)
        {
            if (IntPtr.Zero == videoInstanceId)
            {
                StartDecoding(textUrl.Text, (double)fpsValue.Value, 
                    comboType.SelectedIndex, callback, ref videoInstanceId);
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
