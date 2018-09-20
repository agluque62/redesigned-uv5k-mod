using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.CompilerServices;

using NLog;

namespace uv5k_mn_mod.Helpers

{
    public interface ILogHelper
    {
        ILogHelper From([CallerFilePathAttribute ] string caller = null, [CallerLineNumber] int lineNumber = 0);

        void Trace(string msg, params object[] par);
        void Debug(string msg, params object[] par);
        void Info(string msg, params object[] par);
        void Warn(string msg, params object[] par);
        void Error(string msg, params object[] par);
        void Fatal(string msg, params object[] par);

        void TraceException(Exception x);
    }
    public class LogHelper : ILogHelper
    {
        #region private / protected
            protected Logger Log = LogManager.GetLogger("RdMNService");
        #endregion

        ILogHelper ILogHelper.From(string caller, int lineNumber)
        {
            caller = System.IO.Path.GetFileName(caller);
            LogManager.Configuration.Variables["file"] = caller;
            LogManager.Configuration.Variables["line"] = lineNumber.ToString();
            return this;
        }

        void ILogHelper.Trace(string msg, params object[] par)
        {
            Log.Trace(msg.Replace(System.Environment.NewLine, "--"), par);
        }

        void ILogHelper.Debug(string msg, params object[] par)
        {
            Log.Debug(msg.Replace(System.Environment.NewLine, "--"), par);
        }

        void ILogHelper.Info(string msg, params object[] par)
        {
            Log.Info(msg.Replace(System.Environment.NewLine, "--"), par);
        }

        void ILogHelper.Warn(string msg, params object[] par)
        {
            Log.Warn(msg.Replace(System.Environment.NewLine, "--"), par);
        }

        void ILogHelper.Error(string msg, params object[] par)
        {
            Log.Error(msg.Replace(System.Environment.NewLine, "--"), par);
        }

        void ILogHelper.Fatal(string msg, params object[] par)
        {
            Log.Fatal(msg.Replace(System.Environment.NewLine, "--"), par);
        }

        void ILogHelper.TraceException(Exception x)
        {
            Log.Log<Exception>(LogLevel.Trace, x);
        }
    }
}
