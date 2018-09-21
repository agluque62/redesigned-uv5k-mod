using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Runtime.CompilerServices;

using NLog;

namespace uv5k_mn_mod.Helpers

{
    public interface ILogHelper
    {
        ILogHelper From([CallerFilePathAttribute] string caller = null, [CallerLineNumber] int lineNumber = 0);

        void Trace(string msg, params object[] par);
        void Debug(string msg, params object[] par);
        void Info(string msg, params object[] par);
        void Warn(string msg, params object[] par);
        void Error(string msg, params object[] par);
        void Fatal(string msg, params object[] par);

        void TraceException(Exception x);
    }
    public class LogHelper : ILogHelper, IDisposable
    {
        #region ILogHelper

        ILogHelper ILogHelper.From(string caller, int lineNumber)
        {
            _file = System.IO.Path.GetFileName(caller);
            //LogManager.Configuration.Variables["file"] = caller;
            //LogManager.Configuration.Variables["line"] = lineNumber.ToString();
            _line = lineNumber.ToString();
            return this;
        }

        void ILogHelper.Trace(string msg, params object[] par)
        {
            LogEnqueue(LogLevel.Trace, msg, par);
        }

        void ILogHelper.Debug(string msg, params object[] par)
        {
            LogEnqueue(LogLevel.Debug, msg, par);
        }

        void ILogHelper.Info(string msg, params object[] par)
        {
            LogEnqueue(LogLevel.Info, msg, par);
        }

        void ILogHelper.Warn(string msg, params object[] par)
        {
            LogEnqueue(LogLevel.Warn, msg, par);
        }

        void ILogHelper.Error(string msg, params object[] par)
        {
            LogEnqueue(LogLevel.Error, msg, par);
        }

        void ILogHelper.Fatal(string msg, params object[] par)
        {
            LogEnqueue(LogLevel.Fatal, msg, par);
        }

        void ILogHelper.TraceException(Exception x)
        {
            LogEnqueue(LogLevel.Error, x.Message);
        }

        #endregion

        public LogHelper()
        {
            Task.Factory.StartNew(() =>
            {
                do
                {
                    dynamic logitem = null;

                    lock (logitems)
                    {
                        if (logitems.Count > 0)
                            logitem = logitems.Dequeue();
                    }

                    if (logitem != null)
                        LogGenerate(logitem);

                } while (taskfinalize.WaitOne(10) == false);
            });
        }

        public void Dispose()
        {
            taskfinalize.Set();
        }

        #region private / protected

        protected Logger Log = LogManager.GetLogger("RdMNService");
        string _file;
        string _line;
        ManualResetEvent taskfinalize = new ManualResetEvent(false);
        Queue<dynamic> logitems = new Queue<dynamic>();
        void LogEnqueue(LogLevel level, string msg, params object[] par)
        {
            lock (logitems)
            {
                if (logitems.Count < 100)
                {
                    logitems.Enqueue(new { level, file = _file, line = _line, msg, par });
                }
            }
        }
        void LogGenerate(dynamic item)
        {
            LogManager.Configuration.Variables["file"] = item.file;
            LogManager.Configuration.Variables["line"] = item.line;
            Log.Log(item.level, item.msg.Replace(System.Environment.NewLine, "--"), item.par);
        }

        #endregion

    }
}
