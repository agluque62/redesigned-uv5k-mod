using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.Text.RegularExpressions;

using uv5k_mn_mod.Modelo;

namespace uv5k_mn_mod.Servicios.RemoteControl
{
    class RCNucSimulado : IRemoteControl
    {
        /** */
        class SimulSnmpSession : GenericSnmpSession
        {
            const Int32 DelaySetFrequencyMs = 1000;

            const string ControlWriteOid = ".1.3.6.1.4.1.7916.8.4.1.1.0";
            const string ControlReadOid = ".1.3.6.1.4.1.7916.8.4.1.2.0";

            const string DeviceStatusOid = ".1.3.6.1.4.1.7916.8.4.1.3.1.11";       //	"grDevStat
            const string FrequencyOid = ".1.3.6.1.4.1.7916.8.4.1.3.1.6";           //	"ffFreq
            const string CarrierOffStatusOid = ".1.3.6.1.4.1.7916.8.4.1.3.1.9";    //	"ffCarrOffst
            const string ChannelSpacingOid = ".1.3.6.1.4.1.7916.8.4.1.3.1.7";      //	"ffChSpc
            const string ModulationOid = ".1.3.6.1.4.1.7916.8.4.1.3.1.8";          //  ffMode
            const string TxPowerOid = ".1.3.6.1.4.1.7916.8.4.1.3.1.10";            //	"rcTxPwr

            #region Override

            private RdEquipmentStatus _status = RdEquipmentStatus.NotPresent;
            public override RdEquipmentStatus Status
            {
                get
                {
                    snmpi.Get<int>(TargetEndp, DeviceStatusOid + OidBase, (res, val, x) =>
                    {
                        SessionErrorsTreatment("Status Get", res, x);
                        if (SessionErrors.Count==0)
                        {
                            int ival = (int)val;
                            switch (ival)
                            {
                                case 0:
                                    _status = RdEquipmentStatus.FailLocal;
                                    break;
                                case 1:
                                    _status = RdEquipmentStatus.Ready;
                                    break;
                                case 2:
                                    _status = RdEquipmentStatus.Fail;
                                    break;
                                case 3:
                                    System.Threading.Thread.Sleep(1000);
                                    _status = RdEquipmentStatus.NotPresent;
                                    break;
                                default:
                                    break;
                            }
                        }
                    });
                    return _status;
                }
            }

            public override string Frequency
            {
                get
                {
                    return GetValue<string>("Frequency get", FrequencyOid + OidBase) as string;
                }
                set
                {
                    SetValue<string>("Frequency set", FrequencyOid + OidBase, value);
                }
            }

            public override int? CarrierOffset
            {
                get
                {
                    return GetValue<int?>("CarrierOffset get", CarrierOffStatusOid + OidBase) as int?;
                }
                set
                {
                    SetValue<int?>("CarrierOffset set", CarrierOffStatusOid + OidBase, value);
                }
            }

            public override int? ChannelSpacing
            {
                get
                {
                    return GetValue<int?>("ChannelSpacingOid get", ChannelSpacingOid + OidBase) as int?;
                }
                set
                {
                    SetValue<int?>("ChannelSpacingOid set", ChannelSpacingOid + OidBase, value);
                }
            }

            public override int? Modulation
            {
                get
                {
                    return GetValue<int?>("ModulationOid get", ModulationOid + OidBase) as int?;
                }
                set
                {
                    SetValue<int?>("ModulationOid set", ModulationOid + OidBase, value);
                }
            }

            public override int? Power
            {
                get
                {
                    return GetValue<int?>("TxPowerOid get", TxPowerOid + OidBase) as int?;
                }
                set
                {
                    SetValue<int?>("TxPowerOid set", TxPowerOid + OidBase, value);
                }
            }

            #endregion

            public SimulSnmpSession(IPEndPoint endp, string rid) : base()
            {
                TargetEndp = endp;
                Rid = rid;
                SessionErrors = new List<dynamic>();
            }

            private string OidBase
            {
                get
                {
                    lock (locker)
                    {
                        string oid = string.Empty;
                        if (EquipmentsOid.Keys.Contains(Rid)==false || EquipmentsOid[Rid]==string.Empty)
                        {
                            snmpi.Set<string>(TargetEndp, ControlWriteOid, Rid, (res, x) =>
                            {
                                SessionErrorsTreatment("OidBase ControlWritie", res, x);
                            });
                            if (SessionErrors.Count == 0)
                            {
                                System.Threading.Thread.Sleep(200);
                                snmpi.Get<string>(TargetEndp, ControlReadOid, (res, val, x) =>
                                {
                                    SessionErrorsTreatment("OidBase ControlRead", res, x);
                                    EquipmentsOid[Rid] = SessionErrors.Count == 0 ? val as string : string.Empty;
                                });
                            }
                        }
                        return EquipmentsOid[Rid];
                    }
                }
            }

            static object locker = new Object();
            static Dictionary<string, string> EquipmentsOid = new Dictionary<string, string>();
            public static bool isEquipmentOid(string value)
            {
                return (new Regex(@"^\.[0-9]*$")).Match(value).Success;
            }
        }

        #region Internal

        RdEquipmentStatus SNMPMasterControlConfig(SimulSnmpSession snmpSession, string sFrecuencia)
        {
            RdEquipmentStatus result = RdEquipmentStatus.NotPresent;

            string ExpectedFrequency = sFrecuencia.Replace(".", "").Replace(",", "");
            string freq = snmpSession.Frequency;
            if (snmpSession.SessionErrors.Count == 0)
            {
                string ReadFrequency = freq.Replace(".", "").Replace(",", "");
                result = ReadFrequency == ExpectedFrequency ? RdEquipmentStatus.Ready : RdEquipmentStatus.FailMasterConfig;
            }
            else
            {
                result = snmpSession.LastStatus;
            }
            return result;
        }

        #endregion Internal

        #region IRemoteControl

        public void CheckNode(dynamic rid, Action<dynamic, dynamic> response)
        {
            if (!(rid is RdEquipmentDataItem))
                response?.Invoke(false, new InvalidCastException(rid));
            else
            {
                RdEquipmentDataItem item = (rid as RdEquipmentDataItem);
                SimulSnmpSession snmpSession = new SimulSnmpSession(item.RemoteControlEndp, item.Rid);
                if (item.MainOrStandby == true)
                {
                    RdEquipmentStatus res = SNMPMasterControlConfig(snmpSession, item.MainFrequency);
                    if (res == RdEquipmentStatus.FailMasterConfig)
                    {
                        response?.Invoke(true, RdEquipmentStatus.FailMasterConfig);
                        return;
                    }
                }
                response?.Invoke(true, snmpSession.Status);
            }
        }

        public void GetMainParams(dynamic rid, Action<dynamic, dynamic> response)
        {
            throw new NotImplementedException();
        }

        public void SetMainParams(dynamic rid, dynamic par, Action<dynamic, dynamic> response)
        {
            if (!(rid is RdEquipmentDataItem))
                response?.Invoke(false, new InvalidCastException(rid));
            else if (!(par is RdEquipmentDataItem))
                response?.Invoke(false, new InvalidCastException(par));
            else
            {
                RdEquipmentDataItem itemTo = (rid as RdEquipmentDataItem);
                RdEquipmentDataItem itemFrom = (rid as RdEquipmentDataItem);
                SimulSnmpSession snmpSession = new SimulSnmpSession(itemTo.RemoteControlEndp, itemTo.Rid);

                snmpSession.Frequency = itemFrom.MainFrequency;
                snmpSession.Modulation = itemFrom.MainModulationMode;
                snmpSession.ChannelSpacing = itemFrom.MainChannelSpace;
                if (itemTo.TxRx)
                {
                    snmpSession.CarrierOffset = itemFrom.MainCarrierOffset;
                    snmpSession.Power = itemFrom.MainPower;
                }


                response?.Invoke(snmpSession.Executed, snmpSession.LastStatus);
            }
        }

        #endregion IRemoteControl
    }
}
