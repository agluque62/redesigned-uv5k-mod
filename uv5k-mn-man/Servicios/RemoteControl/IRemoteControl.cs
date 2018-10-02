using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace uv5k_mn_mod.Servicios.RemoteControl
{
    public enum RdEquipmentSnmpStatus
    {
        NotPresent = 0,
        Ready = 1,
        Local = 2,
        Fail = 3,
        FailSessionsSip = 5,
        FailMasterConfig = 6,
        FailProtocolSNMP = 7
    }
    public interface IRemoteControl
    {
        void CheckNode(dynamic rid, Action<dynamic, dynamic> response);
        void GetMainParams(dynamic rid, Action<dynamic, dynamic> response);
        void SetMainParams(dynamic rid, Action<dynamic, dynamic> response, params object[] par);
    }
}
