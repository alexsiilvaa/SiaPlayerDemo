using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace SiaViewer
{
    static class Program
    {
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate void FrameDecodedCallback(int FrameWidth, int FrameHeight, IntPtr FrameData);

        [DllImport("SiaPlayer.dll")]
        public static extern void StartDecoding(string url, FrameDecodedCallback callback);

        delegate void SetImageCallback(IntPtr img);

        private static Form1 form = new Form1();

        [STAThread]
        static void Main()
        {                                    
            FrameDecodedCallback callback = (width, height, data) =>
            {
                Console.WriteLine("Got frame, width: {0} height: {1}", width, height);
                SetImage(form, width, height, data);
            };
            StartDecoding("rtsp://admin:admin@169.254.0.2:8554/PSIA/Streaming/channels/0", callback);
            Application.Run(form);
        }

        private static void SetImage(Form1 form, int width, int height, IntPtr img)
        {
            if (form.InvokeRequired)
            {
                // We're on a thread other than the GUI thread
                form.Invoke(new MethodInvoker(() => SetImage(form, width, height, img)));
                return;
            }
            form.SetImage(width, height, img);
        }
    }
}
