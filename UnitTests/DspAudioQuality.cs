using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;

using System.Runtime.InteropServices;
using NAudio;
using NAudio.Wave;


namespace UnitTests
{
    public class DspAudioQualityLib
    {
        #region DspAudioQualityLib

        [DllImport("DspAudioQualityLib.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi/*, ExactSpelling = true*/)]
        static extern int DAQOpenInstance();
        [DllImport("DspAudioQualityLib.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi, ExactSpelling = true)]
        static extern void DAQSetData(int instance, int count, IntPtr pfData);
        [DllImport("DspAudioQualityLib.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi, ExactSpelling = true)]
        static extern int DAQGetQuality(int instance);
        [DllImport("DspAudioQualityLib.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi, ExactSpelling = true)]
        static extern void DAQCloseInstance(int instance);
        
        #endregion DspAudioQualityLib

        public int OpenInstance()
        {
            return DAQOpenInstance();
        }

        public int GetQuality(int instance)
        {
            return DAQGetQuality(instance);
        }

        public void SetData(int instance, float[] vdata)
        {
            IntPtr p = Marshal.AllocCoTaskMem(sizeof(float) * vdata.Length);
            Marshal.Copy(vdata, 0, p, vdata.Length);

            DAQSetData(instance, vdata.Length, p);

            Marshal.FreeHGlobal(p);
        }

        public void CloseInstance(int instance)
        {
            DAQCloseInstance(instance);
        }

    }

    // Convertir un array de un tipo en otro tipo....
    // double[] doubleArray = Array.ConvertAll(decimalArray, x => (double)x);


    [TestClass]
    public class DspAudioQuality
    {
        const string inPath = @"alarm.wav";

        [TestMethod]
        public void TestMethod1()
        {
            DspAudioQualityLib DAQ = new DspAudioQualityLib();
            int instance = DAQ.OpenInstance();

            using (var reader = new AudioFileReader(inPath))
            {
            }

            DAQ.CloseInstance(instance);
        }
    }
}
