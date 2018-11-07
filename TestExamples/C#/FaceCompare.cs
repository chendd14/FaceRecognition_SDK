using System;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Imaging;
using System.Runtime.InteropServices;

namespace CSDemo.Source
{
    /// <summary>
    /// // bitmap与Byte[]互转
    /// </summary>
    public class BitmapByteConverter
    {
        /// <summary>
        /// 将bitmap转为byte[]
        /// </summary>
        /// <param name="inputBmp">Bitmap对象</param>
        /// <returns>byte[]数组</returns>
        public byte[] BitmapToByte(Bitmap inputBmp)
        {
            BitmapData bmpdata = null;
            try
            {
                bmpdata = inputBmp.LockBits(new Rectangle(0, 0, inputBmp.Width, inputBmp.Height),
                    ImageLockMode.ReadOnly, inputBmp.PixelFormat);
                int numbytes = bmpdata.Stride * inputBmp.Height;
                byte[] bytedata = new byte[numbytes];
                IntPtr ptr = bmpdata.Scan0;

                Marshal.Copy(ptr, bytedata, 0, numbytes);

                return bytedata;
            }
            finally
            {
                if (bmpdata != null)
                    inputBmp.UnlockBits(bmpdata);
            }
        }
    }


    /// <summary>
    /// 调用DLL的非安全类，主要是为了兼容C++DLL中的指针参数
    /// </summary>
    unsafe class CSCaller
    {
        // DllImport声明，引入dll中的接口函数
        [DllImport("VisionFaceSingleton.dll")]
        private static extern int VisionFaceCompare(int w1, int h1, byte* data1, int w2, int h2, byte* data2, float* score); // 确保models/face_feature.json存在

        /// <summary>
        /// 调用特征比较的函数
        /// </summary>
        /// <returns></returns>
        public float VisionFaceCompareCaller()
        {
            float result = -1;
            try
            {
                BitmapByteConverter converter = new BitmapByteConverter();

                // 打开第一个图片, 并转换参数
                Bitmap myBmp1 = new Bitmap("test1.jpg");
                int h1 = myBmp1.Height;
                int w1 = myBmp1.Width;
                byte[] data1 = converter.BitmapToByte(myBmp1);
                // 打开第二个图片, 并转换参数
                Bitmap myBmp2 = new Bitmap("test4.jpg");
                int h2 = myBmp2.Height;
                int w2 = myBmp2.Width;
                byte[] data2 = converter.BitmapToByte(myBmp2);
                Stopwatch stopWatch = new Stopwatch();

                // 循环调用
                while(true)
                {
                    // 调用DLL接口
                    stopWatch.Start();// 开始计时
                    fixed (byte* p1 = &data1[0])// 获取byte[]的指针
                    fixed (byte* p2 = &data2[0])// 获取byte[]的指针
                    {
                        int flag = VisionFaceCompare(w1, h1, p1, w2, h2, p2, &result);// 调用C++ DLL
                    }
                    stopWatch.Stop();// 结束计时
                    // 显示结果
                    Console.WriteLine("Score:" + result.ToString()+", time cost: " + stopWatch.ElapsedMilliseconds.ToString() + "ms");
                }
            }
            catch(Exception e)
            {
                Console.WriteLine(e.Message);
            }
            finally
            {
                Console.WriteLine("Done!");
            }

            return result;
        }
    }
}
