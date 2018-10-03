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

    public abstract class RdEquipmentMib
    {
        public virtual RdEquipmentStatus Status { get; }
        public virtual string Frequency { get; set; }


        protected SnmpInterfaz snmpi = new SnmpInterfaz();

        public IPEndPoint TargetEndp { get; set; }
        public SnmpInterfazResult LastResult { get; set; }
        public string Rid { get; set; }

        static IDictionary<String, Type> _lastExceptions = new Dictionary<String, Type>();
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
