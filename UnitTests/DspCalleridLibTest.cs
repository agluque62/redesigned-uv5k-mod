using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;

using System.Runtime.InteropServices;
using System.Diagnostics;


namespace UnitTests
{
    public class DspCallerIdLib
    {
        #region Rutinas Importadas.

        [DllImport("DspCalleridLib.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi/*, ExactSpelling = true*/)]
        static extern void DCLStart();
        [DllImport("DspCalleridLib.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi, ExactSpelling = true)]
        static extern bool DCLDecode(IntPtr pfData, IntPtr pcCallerid);

        #endregion Rutinas Importadas.

        public void Start()
        {
            DCLStart();
        }

        public byte [] Decode(float [] data)
        {
            byte[] odata = new byte[128];
            odata[0] = 0;

            IntPtr pIn = Marshal.AllocCoTaskMem(sizeof(float) * data.Length);
            Marshal.Copy(data, 0, pIn, data.Length);

            IntPtr pOut = Marshal.AllocCoTaskMem(sizeof(byte) * odata.Length);
            Marshal.Copy(odata, 0, pOut, odata.Length);

            bool isDataPresent = DCLDecode(pIn, pOut);
            Marshal.Copy(pOut, odata, 0, odata.Length);
            
            Marshal.FreeHGlobal(pIn);
            Marshal.FreeHGlobal(pOut);

            return isDataPresent ? odata : null;
        }
    }


    [TestClass]
    public class DspCalleridLibTest
    {
        [TestMethod]
        public void DCLTest1()
        {
            DspCallerIdLib dcl = new DspCallerIdLib();
            dcl.Start();

            byte[] res = dcl.Decode(DspAudioQuality.tone1k);
            res = dcl.Decode(DspAudioQuality.tone1k);
            res = dcl.Decode(DspAudioQuality.tone1k);
        }
    }
}
