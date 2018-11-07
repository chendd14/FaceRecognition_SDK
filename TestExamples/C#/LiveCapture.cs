using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Imaging;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace CSDemp_LiveCapture
{
    public partial class Form1 : Form
    {

        [DllImport("VisionFaceSingleton.dll", CharSet = CharSet.Auto, CallingConvention = CallingConvention.Cdecl)]
        private static extern int VisionLiveOpenCamera(ref int img_w, ref int img_h);

        [DllImport("VisionFaceSingleton.dll", CharSet = CharSet.Auto, CallingConvention = CallingConvention.Cdecl)]
        private static extern void VisionLiveCloseCamera();

        [DllImport("VisionFaceSingleton.dll", CharSet = CharSet.Auto, CallingConvention = CallingConvention.Cdecl)]
        private static extern int VisionLiveCapture(byte[] img_data, ref int living_status, ref int face_rect_x, ref int face_rect_y, ref int face_rect_w, ref int face_rect_h);

        int img_w, img_h;

        private void OpenButton_Click(object sender, EventArgs e)
        {
			// open camera
            VisionLiveOpenCamera(ref img_w, ref img_h);
        }

        private void RunLiveCapture()
        {
            Bitmap img = null;
            
            byte[] data = new byte[img_w * img_h * 3];
            int living_status = 0, face_rect_x = 0, face_rect_y = 0, face_rect_w = 0, face_rect_h = 0;
            BitmapByteConverter converter = new BitmapByteConverter();

            isRunning = true;
            Stopwatch stopWatch = new Stopwatch();
            Graphics img_graphic;
            Pen face_rect_pen = new Pen(Color.Red);
            SolidBrush face_text_brush = new SolidBrush(Color.Black);
            Point face_rect_point = new Point();
            Size face_rect_size = new Size();
            while (isRunning)
            {
                stopWatch.Start();
                VisionLiveCapture(data, ref living_status, ref face_rect_x, ref face_rect_y, ref face_rect_w, ref face_rect_h);

                face_rect_point.X = face_rect_x;
                face_rect_point.Y = face_rect_y;
                face_rect_size.Width = face_rect_w;
                face_rect_size.Height = face_rect_h;

                img = converter.ByteToBitmap(data, img_w, img_h);
                img_graphic = Graphics.FromImage(img);
				
                // show result
                if (living_status == 1)
                {
                    face_rect_pen.Color = Color.Green;
                    face_text_brush.Color = Color.Green;
                    img_graphic.DrawRectangle(face_rect_pen, new Rectangle(face_rect_point, face_rect_size));
                    img_graphic.DrawString("Living", new Font("Courier New", 12), face_text_brush, face_rect_point);
                }
                else if (living_status == 0)
                {
                    face_rect_pen.Color = Color.Red;
                    face_text_brush.Color = Color.Red;
                    img_graphic.DrawRectangle(face_rect_pen, new Rectangle(face_rect_point, face_rect_size));
                    img_graphic.DrawString("Fake", new Font("Courier New", 12), face_text_brush, face_rect_point);
                }
                else
                {
                    face_rect_pen.Color = Color.Blue;
                    face_text_brush.Color = Color.Blue;
                    img_graphic.DrawString("Please put your face\nat the center of the screen.", new Font("Courier New", 15), face_text_brush, face_rect_point);
                }
                liveCaptureDisp.BackgroundImage = img;
                liveCaptureDisp.Invalidate();

                stopWatch.Stop();
                if(!isTesting && testProc.IsAlive)
                    statusLabel.Text = stopWatch.ElapsedMilliseconds.ToString() + "ms";
                stopWatch.Reset();

                Thread.Sleep(10);
            }
        }

        private void CloseLiveCapture()
        {
            // close camera
            VisionLiveCloseCamera();
        }
    }
}
