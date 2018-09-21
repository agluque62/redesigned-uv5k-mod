using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using U5ki.Infrastructure;

namespace uv5k_mn_mod.Modelo
{
    public class FreqDataItem
    {
        public object Fid { get; set; }
        public object Band { get; set; }
        public object Priority { get; set; }
    }

    public class RdEquipmentDataItem
    {
        public dynamic Id { get; set; }

        /** Configuration data */
        public object Model { get; set; }
        public object Site { get; set; }
        public object Band { get; set; }
        public object TxRx { get; set; }
        public object MainOrStanby { get; set; }
        public object MainFrequency { get; set; }
        public object MainPower { get; set; }
        public object MainModulationMode { get; set; }
        public object MainCarrierOffset { get; set; }
        public object MainChannelSpace { get; set; }
        public object StandbyFrequeciesRange { get; set; }

        /** Remote control data */
        public object RemoteControlEndp { get; set; }
        public object RemoteControlBaseOid { get; set; }
        public object RemoteControlManager { get; set; }

        /** Voip Configuration */
        public object SipUri { get; set; }
    }

    public class RdEquipmentStatus
    {
        public object Id { get; set; }

        public object IsAlive { get; set; }
        public object IsOk { get; set; }
        public object IsEnabled { get; set; }
    }

}
