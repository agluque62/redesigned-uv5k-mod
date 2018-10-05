using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;

namespace uv5k_mn_mod.Servicios.RemoteControl
{
    public enum RdEquipmentStatus
    {
        NotPresent = 0,
        Ready = 1,
        Fail = 2,
        FailLocal = 3,
        FailSessionsSip = 5,
        FailMasterConfig = 6,
        FailProtocolSNMP = 7
    }

    public interface IRemoteControl
    {
        void CheckNode(dynamic rid, Action<dynamic, dynamic> response);
        void GetMainParams(dynamic rid, Action<dynamic, dynamic> response);
        void SetMainParams(dynamic rid, dynamic par, Action<dynamic, dynamic> response);
    }

    public abstract class GenericSnmpSession
    {
        public virtual RdEquipmentStatus Status { get; }
        public virtual string Frequency { get; set; }
        public virtual int? ChannelSpacing { get; set; }
        public virtual int? Modulation { get; set; }
        public virtual int? CarrierOffset { get; set; }
        public virtual int? Power { get; set; }

        public IPEndPoint TargetEndp { get; set; }
        public List<dynamic> SessionErrors { get; set; }
        public string Rid { get; set; }
        public bool Executed
        {
            get { return SessionErrors.Count == 0; }
        }
        public RdEquipmentStatus LastStatus
        {
            get
            {
                return Executed ? RdEquipmentStatus.Ready :
                    SessionErrors.Where(i => i.res == SnmpInterfazResult.Timeout).ToList().Count == 0 ? RdEquipmentStatus.Fail :
                    RdEquipmentStatus.NotPresent;
            }
        }

        protected SnmpInterfaz snmpi = new SnmpInterfaz();
        protected object GetValue<T>(string strproc, string oid)
        {
            object retorno = null;
            snmpi.Get<T>(TargetEndp, oid, (res, val, x) =>
            {
                retorno = val;
                SessionErrorsTreatment(strproc, res, x);
            });
            return retorno;
        }
        protected void SetValue<T>(string strproc, string oid, T valor)
        {
            snmpi.Set<T>(TargetEndp, oid, valor, (res, x) =>
            {
                SessionErrorsTreatment(strproc, res, x);
            });
        }
        protected SnmpDataSet GetSet(string strproc, SnmpDataSet set)
        {
            SnmpDataSet retorno = null;
            snmpi.Get(TargetEndp, set , (res, val, x) =>                
            {
                retorno = val as SnmpDataSet;
                SessionErrorsTreatment(strproc, res, x);
            });
            return retorno;
        }
        protected void SetSet(string strproc, SnmpDataSet set)
        {
            snmpi.Set(TargetEndp, set, (res, val, x) =>
            {
                SessionErrorsTreatment(strproc, res, x);
            });
        }
        protected void SessionErrorsTreatment(String strProc, SnmpInterfazResult res, Exception ex)
        {
            if (res != SnmpInterfazResult.Ok)
            {
                if (ex != null)
                    ExceptionTreatment(strProc, res, ex);
                SessionErrors.Add(new { strProc, res, ex });
            }
        }
        protected static IDictionary<String, Type> _lastExceptions = new Dictionary<String, Type>();
        protected static SnmpInterfazResult ExceptionTreatment(String strProc, SnmpInterfazResult res, Exception ex)
        {
            if (res != SnmpInterfazResult.Ok && ex != null)
            {
                if (!_lastExceptions.ContainsKey(strProc) || _lastExceptions[strProc] != ex.GetType())
                {
                    // TODO. LOG...
                }
            }
            return res;
        }
    }
}
