using System;
using System.Collections.Generic;
using System.Linq;
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
        static extern float DAQGetSampleMax(int instance);
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

        public float GetSampleMax(int instance)
        {
            return DAQGetSampleMax(instance);
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
        const string inPath = @"FE-00001.wav";
        const int BlockSize = 48;
        int CallControl = (int )Math.Round((Decimal)(512.0 / BlockSize), MidpointRounding.AwayFromZero);

        [TestMethod]
        public void TestMethod1()
        {
            DspAudioQualityLib DAQ = new DspAudioQualityLib();
            int instance = DAQ.OpenInstance();
            float sample_in_max = 0,sample_proc_max=0;
            List<int> quality = new List<int>();

            using (var reader = new AudioFileReader(inPath))
            {
                float[] buffer = new float[BlockSize];
                int read,testq=0;
                do
                {
                    read = reader.Read(buffer, 0, buffer.Length);
                    if (read > 0)
                    {
                        for (int n = 0; n < read; n++)
                        {
                            float unnormal_sample = (buffer[n] * 0x7fff);

                            var abs = Math.Abs(unnormal_sample);
                            if (abs > sample_in_max) sample_in_max = abs;

                            buffer[n] = unnormal_sample;
                        }

                        DAQ.SetData(instance, buffer);
                        if ((++testq % CallControl) == 0)
                        {
                            quality.Add(DAQ.GetQuality(instance));
                        }
                    }
                } while (read > 0);

                sample_proc_max = DAQ.GetSampleMax(instance);
                quality.Add(DAQ.GetQuality(instance));
            }

            DAQ.CloseInstance(instance);
        }
    }
}
