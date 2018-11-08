using System;
using System.Collections.Generic;
using System.Linq;
using Microsoft.VisualStudio.TestTools.UnitTesting;

using System.Runtime.InteropServices;
using System.Diagnostics;

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
        [DllImport("DspAudioQualityLib.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi, ExactSpelling = true)]
        static extern void DAQRoutineTest(int routine);
        [DllImport("DspAudioQualityLib.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi, ExactSpelling = true)]
        public static extern void DAQTwiddleGeneratorTest();

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

        public static void Test(int routine)
        {
            DAQRoutineTest(routine);
        }

    }

    // Convertir un array de un tipo en otro tipo....
    // double[] doubleArray = Array.ConvertAll(decimalArray, x => (double)x);

    [TestClass]
    public class DspAudioQuality
    {
        const string inPath = @"FE-00001.wav";
        const string inPath1 = @"Tono440.wav";

        const int BlockSize = 48;
        int CallControl = (int)Math.Round((Decimal)(512.0 / BlockSize), MidpointRounding.AwayFromZero);

        [TestMethod]
        public void AudioFileTest()
        {
            DspAudioQualityLib DAQ = new DspAudioQualityLib();
            int instance = DAQ.OpenInstance();
            float sample_in_max = 0, sample_proc_max = 0;
            List<int> quality = new List<int>();

            using (var reader = new AudioFileReader(inPath1))
            {
                float[] buffer = new float[BlockSize];
                int read, testq = 0;
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

        [TestMethod]
        public void DspRoutinesTest()
        {
            TestResult SignalPower = Test(0, 1000);
            TestResult Windowing = Test(1, 1000);
            TestResult FFT = Test(2, 1000);
            TestResult LOGComplex = Test(3, 1000);
            TestResult IFFT = Test(4, 1000);
            TestResult ABSComplex = Test(5, 1000);
            TestResult AudioFile = AudioFileAnalyze();

            double PartialsSumation = SignalPower.time + Windowing.time + FFT.time + LOGComplex.time + IFFT.time + ABSComplex.time + ABSComplex.time;
            double Total = AudioFile.maxtime;
        }

        [TestMethod]
        public void TwiddleGeneratorTest()
        {
            gen_twiddle(512);

            Test(6, 1000);
            DspAudioQualityLib.DAQTwiddleGeneratorTest();
        }

        class TestResult
        {
            public double ticks { get; set; }
            public double time { get; set; }
            public double maxticks { get; set; }
            public double maxtime { get; set; }
            public double inonemsec { get; set; }

            public TestResult()
            {
                ticks = maxticks = time = maxtime = 0;
            }
        }

        TestResult Test(int routine, int count)
        {
            TestResult res = new TestResult();
            Stopwatch stopWatch = new Stopwatch();

            for (int i = 0; i < count; i++)
            {
                stopWatch.Restart();

                DspAudioQualityLib.Test(routine);

                res.ticks = (res.ticks * 0.9 + stopWatch.ElapsedTicks * 0.1);
                res.time = res.ticks / Stopwatch.Frequency;

                res.maxticks = res.maxticks < stopWatch.ElapsedTicks ? stopWatch.ElapsedTicks : res.maxticks;
                res.maxtime = res.maxticks / Stopwatch.Frequency;
                res.inonemsec = (1 / res.time) / 1000;
            }

            return res;
        }

        TestResult AudioFileAnalyze()
        {
            TestResult res = new TestResult();
            Stopwatch stopWatch = new Stopwatch();

            DspAudioQualityLib DAQ = new DspAudioQualityLib();
            int instance = DAQ.OpenInstance();
            float sample_in_max = 0, sample_proc_max = 0;
            List<int> quality = new List<int>();

            using (var reader = new AudioFileReader(inPath))
            {
                float[] buffer = new float[BlockSize];
                int read, testq = 0;
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

                        stopWatch.Restart();

                        DAQ.SetData(instance, buffer);

                        res.ticks = (res.ticks * 0.9 + stopWatch.ElapsedTicks * 0.1);
                        res.time = res.ticks / Stopwatch.Frequency;

                        res.maxticks = res.maxticks < stopWatch.ElapsedTicks ? stopWatch.ElapsedTicks : res.maxticks;
                        res.maxtime = res.maxticks / Stopwatch.Frequency;

                        res.inonemsec = (1 / res.time) / 1000;

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

            return res;
        }

        void gen_twiddle(int n)
        {
            float[] w = new float[n * 2];

            int i, j, k;
            const float PI = 3.14159265358979323846F;

            for (j = 1, k = 0; j < n >> 2; j = j << 2)
            {
                for (i = 0; i < n >> 2; i += j)
                {
                    w[k + 5] = (float)Math.Sin(6.0 * PI * i / n);
                    w[k + 4] = (float)Math.Cos(6.0 * PI * i / n);

                    w[k + 3] = (float)Math.Sin(4.0 * PI * i / n);
                    w[k + 2] = (float)Math.Cos(4.0 * PI * i / n);

                    w[k + 1] = (float)Math.Sin(2.0 * PI * i / n);
                    w[k + 0] = (float)Math.Cos(2.0 * PI * i / n);

                    k += 6;
                }
            }
        }


        float[] tone1k = {
    0.000000F, 23205.352051F, 32766.845188F, 23062.687376F, -201.447980F, -23347.139634F, -32765.606702F, -22919.151003F,
    402.888347F, 23488.044767F, 32763.129776F, 22774.748355F, -604.313485F, -23628.062124F, -32759.414504F, -22629.484893F,
    805.715782F, 23767.186413F, 32754.461027F, 22483.366105F, -1007.087626F, -23905.412375F, -32748.269532F, -22336.397514F,
    1208.421405F, 24042.734785F, 32740.840252F, 22188.584677F, -1409.709509F, -24179.148454F, -32732.173470F, -22039.933179F,
    1610.944330F, 24314.648226F, 32722.269511F, 21890.448639F, -1812.118263F, -24449.228978F, -32711.128751F, -21740.136707F,
    2013.223703F, 24582.885624F, 32698.751610F, 21589.003064F, -2214.253050F, -24715.613112F, -32685.138557F, -21437.053424F,
    2415.198704F, 24847.406426F, 32670.290106F, 21284.293529F, -2616.053072F, -24978.260585F, -32654.206818F, -21130.729152F,
    2816.808560F, 25108.170641F, 32636.889301F, 20976.366099F, -3017.457582F, -25237.131686F, -32618.338210F, -20821.210203F,
    3217.992553F, 25365.138845F, 32598.554245F, 20665.267330F, -3418.405894F, -25492.187279F, -32577.538155F, -20508.543373F,
    3618.690030F, 25618.272186F, 32555.290734F, 20351.044255F, -3818.837390F, -25743.388801F, -32531.812823F, -20192.775931F,
    4018.840410F, 25867.532396F, 32507.105309F, 20033.744381F, -4218.691531F, -25990.698276F, -32481.169126F, -19873.955618F,
    4418.383197F, 26112.881788F, 32454.005255F, 19713.415680F, -4617.907862F, -26234.078313F, -32425.614721F, -19552.130635F,
    4817.257985F, 26354.283271F, 32395.998599F, 19390.106579F, -5016.426030F, -26473.492117F, -32365.158007F, -19227.349637F,
    5215.404469F, 26591.700346F, 32333.094112F, 19063.865961F, -5414.185782F, -26708.903491F, -32299.808125F, -18899.661728F,
    5612.762455F, 26825.097121F, 32265.301304F, 18734.743146F, -5811.126984F, -26940.276845F, -32229.574953F, -18569.116448F,
    6009.271869F, 27054.438308F, 32192.630424F, 18402.787895F, -6207.189622F, -27167.577197F, -32154.469111F, -18235.763773F,
    6404.872763F, 27279.689235F, 32115.092459F, 18068.050394F, -6602.313819F, -27390.770184F, -32074.501954F, -17899.654099F,
    6799.505328F, 27500.815846F, 32032.699131F, 17730.581252F, -6996.439836F, -27609.822061F, -31989.685571F, -17560.838243F,
    7193.109901F, 27717.784710F, 31945.462898F, 17390.431488F, -7389.508088F, -27824.699712F, -31900.032785F, -17219.367428F,
    7585.626974F, 27930.563025F, 31853.396949F, 17047.652529F, -7781.459146F, -28035.370648F, -31805.557152F, -16875.293281F,
    7976.997204F, 28139.118621F, 31756.515202F, 16702.296199F, -8172.233755F, -28241.803021F, -31706.272953F, -16528.667821F,
    8367.161421F, 28343.419967F, 31654.832305F, 16354.414710F, -8561.772834F, -28443.965619F, -31602.195200F, -16179.543452F,
    8756.060638F, 28543.436177F, 31548.363630F, 16004.060657F, -8950.017490F, -28641.827880F, -31493.339628F, -15827.972958F,
    9143.636059F, 28739.137009F, 31437.125275F, 15651.287010F, -9336.909026F, -28835.359888F, -31379.722694F, -15474.009491F,
    9529.829087F, 28930.492878F, 31321.134056F, 15296.147102F, -9722.388949F, -29024.532384F, -31261.361576F, -15117.706566F,
    9914.581335F, 29117.474852F, 31200.407512F, 14938.694626F, -10106.398980F, -29209.316768F, -31138.274168F, -14759.118050F,
    10297.834634F, 29300.054662F, 31074.963892F, 14578.983624F, -10488.881062F, -29389.685104F, -31010.479079F, -14398.298158F,
    10679.531042F, 29478.204706F, 30944.822164F, 14217.068480F, -10869.777369F, -29565.610122F, -30877.995630F, -14035.301440F,
    11059.612851F, 29651.898048F, 30810.002002F, 13853.003909F, -11249.030314F, -29737.065224F, -30740.843851F, -13670.182777F,
    11438.022598F, 29821.108430F, 30670.523790F, 13486.844954F, -11626.582560F, -29904.024489F, -30599.044477F, -13302.997369F,
    11814.703072F, 29985.810268F, 30526.408614F, 13118.646972F, -12002.377025F, -30066.462675F, -30452.618946F, -12933.800730F,
    12189.597325F, 30145.978662F, 30377.678263F, 12748.465631F, -12376.356896F, -30224.355224F, -30301.589397F, -12562.648678F,
    12562.648678F, 30301.589397F, 30224.355224F, 12376.356896F, -12748.465631F, -30377.678263F, -30145.978662F, -12189.597325F,
    12933.800730F, 30452.618946F, 30066.462675F, 12002.377025F, -13118.646972F, -30526.408614F, -29985.810268F, -11814.703072F,
    13302.997369F, 30599.044477F, 29904.024489F, 11626.582560F, -13486.844954F, -30670.523790F, -29821.108430F, -11438.022598F,
    13670.182777F, 30740.843851F, 29737.065224F, 11249.030314F, -13853.003909F, -30810.002002F, -29651.898048F, -11059.612851F,
    14035.301440F, 30877.995630F, 29565.610122F, 10869.777369F, -14217.068480F, -30944.822164F, -29478.204706F, -10679.531042F,
    14398.298158F, 31010.479079F, 29389.685104F, 10488.881062F, -14578.983624F, -31074.963892F, -29300.054662F, -10297.834634F,
    14759.118050F, 31138.274168F, 29209.316768F, 10106.398980F, -14938.694626F, -31200.407512F, -29117.474852F, -9914.581335F,
    15117.706566F, 31261.361576F, 29024.532384F, 9722.388949F, -15296.147102F, -31321.134056F, -28930.492878F, -9529.829087F,
    15474.009491F, 31379.722694F, 28835.359888F, 9336.909026F, -15651.287010F, -31437.125275F, -28739.137009F, -9143.636059F,
    15827.972958F, 31493.339628F, 28641.827880F, 8950.017490F, -16004.060657F, -31548.363630F, -28543.436177F, -8756.060638F,
    16179.543452F, 31602.195200F, 28443.965619F, 8561.772834F, -16354.414710F, -31654.832305F, -28343.419967F, -8367.161421F,
    16528.667821F, 31706.272953F, 28241.803021F, 8172.233755F, -16702.296199F, -31756.515202F, -28139.118621F, -7976.997204F,
    16875.293281F, 31805.557152F, 28035.370648F, 7781.459146F, -17047.652529F, -31853.396949F, -27930.563025F, -7585.626974F,
    17219.367428F, 31900.032785F, 27824.699712F, 7389.508088F, -17390.431488F, -31945.462898F, -27717.784710F, -7193.109901F,
    17560.838243F, 31989.685571F, 27609.822061F, 6996.439836F, -17730.581252F, -32032.699131F, -27500.815846F, -6799.505328F,
    17899.654099F, 32074.501954F, 27390.770184F, 6602.313819F, -18068.050394F, -32115.092459F, -27279.689235F, -6404.872763F,
    18235.763773F, 32154.469111F, 27167.577197F, 6207.189622F, -18402.787895F, -32192.630424F, -27054.438308F, -6009.271869F,
    18569.116448F, 32229.574953F, 26940.276845F, 5811.126984F, -18734.743146F, -32265.301304F, -26825.097121F, -5612.762455F,
    18899.661728F, 32299.808125F, 26708.903491F, 5414.185782F, -19063.865961F, -32333.094112F, -26591.700346F, -5215.404469F,
    19227.349637F, 32365.158007F, 26473.492117F, 5016.426030F, -19390.106579F, -32395.998599F, -26354.283271F, -4817.257985F,
    19552.130635F, 32425.614721F, 26234.078313F, 4617.907862F, -19713.415680F, -32454.005255F, -26112.881788F, -4418.383197F,
    19873.955618F, 32481.169126F, 25990.698276F, 4218.691531F, -20033.744381F, -32507.105309F, -25867.532396F, -4018.840410F,
    20192.775931F, 32531.812823F, 25743.388801F, 3818.837390F, -20351.044255F, -32555.290734F, -25618.272186F, -3618.690030F,
    20508.543373F, 32577.538155F, 25492.187279F, 3418.405894F, -20665.267330F, -32598.554245F, -25365.138845F, -3217.992553F,
    20821.210203F, 32618.338210F, 25237.131686F, 3017.457582F, -20976.366099F, -32636.889301F, -25108.170641F, -2816.808560F,
    21130.729152F, 32654.206818F, 24978.260585F, 2616.053072F, -21284.293529F, -32670.290106F, -24847.406426F, -2415.198704F,
    21437.053424F, 32685.138557F, 24715.613112F, 2214.253050F, -21589.003064F, -32698.751610F, -24582.885624F, -2013.223703F,
    21740.136707F, 32711.128751F, 24449.228978F, 1812.118263F, -21890.448639F, -32722.269511F, -24314.648226F, -1610.944330F,
    22039.933179F, 32732.173470F, 24179.148454F, 1409.709509F, -22188.584677F, -32740.840252F, -24042.734785F, -1208.421405F,
    22336.397514F, 32748.269532F, 23905.412375F, 1007.087626F, -22483.366105F, -32754.461027F, -23767.186413F, -805.715782F,
    22629.484893F, 32759.414504F, 23628.062124F, 604.313485F, -22774.748355F, -32763.129776F, -23488.044767F, -402.888347F,
    22919.151003F, 32765.606702F, 23347.139634F, 201.447980F, -23062.687376F, -32766.845188F, -23205.352051F, -0.000000F
    };
        float[] onems_tone1k =
        {
            0.000000F, 23205.352051F, 32766.845188F, 23062.687376F, -201.447980F, -23347.139634F, -32765.606702F, -22919.151003F
        };

        [TestMethod]
        public void SineTest()
        {
            DspAudioQualityLib DAQ = new DspAudioQualityLib();
            int instance = DAQ.OpenInstance();
            float sample_in_max = 0, sample_proc_max = 0;
            List<int> quality = new List<int>();
            int testq = 0;

            for (int j = 0; j < 1000; j++)
            {
                for (int i = 0; i < (512 / 64); i++)
                {
                    float[] buffer = new float[64];
                    Array.Copy(tone1k, 64 * i, buffer, 0, 64);

                    DAQ.SetData(instance, buffer);

                    if ((++testq % CallControl) == 0)
                    {
                        quality.Add(DAQ.GetQuality(instance));
                    }
                }
            }

            sample_proc_max = DAQ.GetSampleMax(instance);
            quality.Add(DAQ.GetQuality(instance));

            DAQ.CloseInstance(instance);

        }

        [TestMethod]
        public void SineTest_1k()
        {
            DspAudioQualityLib DAQ = new DspAudioQualityLib();
            int instance = DAQ.OpenInstance();
            float sample_proc_max = 0;
            List<int> quality = new List<int>();
            int testq = 0;

            int duration = 10000;

            do
            {
                DAQ.SetData(instance, onems_tone1k);

                if ((++testq % 100) == 0)
                {
                    int aqua = DAQ.GetQuality(instance);
                    quality.Add(aqua);
                    Debug.WriteLine($"Quality = {aqua}");
                }

            } while (--duration > 0);

            sample_proc_max = DAQ.GetSampleMax(instance);
            quality.Add(DAQ.GetQuality(instance));

            DAQ.CloseInstance(instance);

        }
    }
}
