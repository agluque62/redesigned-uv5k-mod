using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.Threading.Tasks;

using U5ki.Infrastructure;

using uv5k_mn_mod.Servicios.RemoteControl;

namespace uv5k_mn_mod.Modelo
{

    public class RdEquipmentDataItem : IEquatable<RdEquipmentDataItem>, IEquatable<RdEquipmentStatus>
    {
        public string Rid { get; set; }

        /** Configuration data */
        public dynamic Model { get; set; }
        public string Site { get; set; }
        public dynamic Band { get; set; }
        public bool TxRx { get; set; }
        public dynamic MainOrStandby { get; set; }
        public string MainFrequency { get; set; }
        public dynamic MainPriority { get; set; }
        public dynamic MainPower { get; set; }
        public dynamic MainModulationMode { get; set; }
        public dynamic MainCarrierOffset { get; set; }
        public dynamic MainChannelSpace { get; set; }
        public dynamic StandbyFrequeciesRange { get; set; }

        /** Remote control data */
        public IPEndPoint RemoteControlEndp { get; set; }
        public string RemoteControlBaseOid { get; set; }

        /** Voip Configuration */
        public string SipUri { get; set; }

        public RdEquipmentDataItem() { }
        public RdEquipmentDataItem(RdEquipmentDataItem o)
        {
            Rid = o.Rid;
            Model = o.Model;
            Site = o.Site;
            Band = o.Band;
            TxRx = o.TxRx;
            MainOrStandby = o.MainOrStandby;
            MainFrequency = o.MainFrequency;
            MainPriority = o.MainPriority;
            MainPower = o.MainPower;
            MainModulationMode = o.MainModulationMode;
            MainCarrierOffset = o.MainCarrierOffset;
            MainChannelSpace = o.MainChannelSpace;
            StandbyFrequeciesRange = o.StandbyFrequeciesRange;
            RemoteControlEndp = o.RemoteControlEndp;
            RemoteControlBaseOid = o.RemoteControlBaseOid;
            SipUri = o.SipUri;
        }

        /** Para las comparaciones al recargar */
        public override bool Equals(object obj)
        {
            return Equals(obj as RdEquipmentDataItem);
        }

        public override int GetHashCode()
        {
            return Rid == null ? 0 : (Rid + Site).GetHashCode();
        }

        public bool Equals(RdEquipmentDataItem other) => Rid == other.Rid &&
                Site == other.Site &&
                Band == other.Band &&
                TxRx == other.TxRx &&
                MainOrStandby == other.MainOrStandby &&
                MainFrequency == other.MainFrequency &&
                MainPower == other.MainPower &&
                MainModulationMode == other.MainModulationMode &&
                MainCarrierOffset == other.MainCarrierOffset &&
                MainChannelSpace == other.MainChannelSpace &&
                StandbyFrequeciesRange == other.StandbyFrequeciesRange &&
                RemoteControlEndp == RemoteControlEndp &&
                RemoteControlBaseOid == other.RemoteControlBaseOid &&
                SipUri == other.SipUri;

        public bool Equals(RdEquipmentStatus other) => other.Rid == Rid;
    }

    public class RdEquipmentStatus
    {
        public dynamic Rid { get; set; }

        public IRemoteControl RemoteControlManager { get; set; }
        public string RemoteControlBaseOid { get; set; }

        public dynamic IsAlive { get; set; }
        public dynamic IsOk { get; set; }
        public dynamic IsEnabled { get; set; }
        public dynamic IsOperative { get; set; }
    }

    public class FreqDataItemAssign
    {
        public string Fid { get; set; }
        public dynamic Band { get; set; }
        public dynamic Priority { get; set; }
        public string Site { get; set; }
        public bool TxOrRx { get; set; }
        public string DefaultRid { get; set; }
        public string ActualRid { get; set; }
    }

    //public class FreqDataAssign
    //{
    //    public string Rid;
    //    public string SipUri;
    //}

    //public class FreqDataStatus
    //{
    //    public string Fid { get; set; }
    //    public Dictionary<string, FreqDataAssign> Rx { get; set; }
    //    public Dictionary<string, FreqDataAssign> Tx { get; set; }
    //}

}
