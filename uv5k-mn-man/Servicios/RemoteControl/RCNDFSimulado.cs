#define DEBUG_VERBOSE_NO
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.Text.RegularExpressions;

//using U5ki.Enums;
//using U5ki.Infrastructure;
//using U5ki.Infrastructure.Code;
//using U5ki.Infrastructure.Resources;
using Utilities;

using Lextm.SharpSnmpLib;
//using Lextm.SharpSnmpLib.Messaging;
//using Lextm.SharpSnmpLib.Objects;
//using Lextm.SharpSnmpLib.Pipeline;
using NLog;

using uv5k_mn_mod.Modelo;

namespace uv5k_mn_mod.Servicios.RemoteControl
{
    class RCNDFSimulado : IRemoteControl
    {
        /** */
        class SimulMib : RdEquipmentMib
        {
            const Int32 DelaySetFrequencyMs = 1000;

            const string ControlWriteOid = ".1.3.6.1.4.1.7916.8.4.1.1.0";
            const string ControlReadOid = ".1.3.6.1.4.1.7916.8.4.1.2.0";

            const string CarrierOffStatusOid = ".1.3.6.1.4.1.7916.8.4.1.3.1.9";    //	"ffCarrOffst
            const string ChannelSpacingOid = ".1.3.6.1.4.1.7916.8.4.1.3.1.7";      //	"ffChSpc
            const string DeviceStatusOid = ".1.3.6.1.4.1.7916.8.4.1.3.1.11";       //	"grDevStat
            const string FrequencyOid = ".1.3.6.1.4.1.7916.8.4.1.3.1.6";           //	"ffFreq
            const string ModulationOid = ".1.3.6.1.4.1.7916.8.4.1.3.1.8";          //  ffMode
            const string TxPowerOid = ".1.3.6.1.4.1.7916.8.4.1.3.1.10";            //	"rcTxPwr

            #region Override

            private RdEquipmentStatus _status = RdEquipmentStatus.NotPresent;
            public override RdEquipmentStatus Status
            {
                get
                {
                    snmpi.GetInt(TargetEndp, DeviceStatusOid + OidBase, (res, val, x) =>
                    {
                        if (res == SnmpInterfazResult.Ok)
                        {
                            switch (val)
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
                        LastResult = res;
                    });
                    return _status;
                }
            }

            public override string Frequency
            {
                get
                {
                    string freq = string.Empty;
                    snmpi.GetString(TargetEndp, FrequencyOid + OidBase, (res, val, x) =>
                    {
                        freq = res == SnmpInterfazResult.Ok ? val : "Error";
                        LastResult = ExceptionTreatment("Frequency get", res, x);                        
                    });
                    return freq;
                }
                set
                {
                    snmpi.SetString(TargetEndp, FrequencyOid + OidBase, value, (res, x) =>
                    {
                        LastResult = ExceptionTreatment("Frequency get", res, x);
                    });
                }
            }

            #endregion

            public SimulMib(IPEndPoint endp, string rid)
            {
                TargetEndp = endp;
                Rid = rid;
                LastResult = RdEquipmentStatus.NotPresent;
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
                            snmpi.SetString(TargetEndp, ControlWriteOid, Rid, (res, x) =>
                            {
                                LastResult = res ? RdEquipmentStatus.Ready : ExceptionTreatment("Frequency get", x);
                            });
                            if (LastResult == RdEquipmentStatus.Ready)
                            {
                                System.Threading.Thread.Sleep(200);
                                snmpi.GetString(TargetEndp, ControlReadOid, (res, val, x) =>
                                {
                                    LastResult = res ? RdEquipmentStatus.Ready : ExceptionTreatment("Frequency get", x);
                                    EquipmentsOid[Rid] = LastResult == RdEquipmentStatus.Ready ? val : string.Empty;
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

        #region IRemoteControl_old
        /// <summary>
        /// 
        /// </summary>
		
        // JOI. CONTROL_SET_SIP
        private int delaySetFrequencyMs = Convert.ToInt32(/*u5ki.RemoteControlService.Properties.Settings.Default.*/DelaySetFrequencyMs);
        // JOI. CONTROL_SET_SIP FIN.
		
        /// <summary>
        /// 
        /// </summary>
        /// <param name="action"></param>
        /// <param name="response"></param>
        /// <param name="node"></param>
        /// <param name="isEmitter"></param>
        public void ConfigureNode(RCConfigurationAction action, Action<GearOperationStatus> response, BaseNode node, Boolean isEmitter, Boolean isMaster)
        {
            if (action == RCConfigurationAction.Unassing)
            {
                Invoke(response, "ConfigureNode.Unassing", GearOperationStatus.OK, false); 
                return;
            }
            LogInfo<RCNDFSimulado>(String.Format("RCNDFSimulado.ConfigureNode.Assign({0})", Id));
            //Task.Factory.StartNew(() =>
            //{
            //    System.Threading.Thread.CurrentThread.Name = "CfgNode_" + (Id.Contains("_N") == true ? Id.ToLower() : Id);
                try
                {
					// JOI. CONTROL_SET_SIP
					System.Threading.Thread.Sleep(delaySetFrequencyMs);
#if LOCKER
                    lock (locker)
#endif
                    {
                        ip_equipo = node.IP;
                        if (String.IsNullOrEmpty(OidEquipo))
                        {
                            Invoke(response, "ConfigureNode.Assing", GearOperationStatus.Fail, false);
                            _lastExceptions["ConfigureNode"] = null;
                            return;
                        }
                        // Fecuency
                        SnmpClient.SetString(ip_equipo, Community,
                            u5ki.RemoteControlService.OIDs.NDFSimulado.Frecuency + OidEquipo,
                            node.Frecuency, SNMPCallTimeout, Port, SNMPVersion);

                        // Channel Spacing.
                        SnmpClient.SetInt(ip_equipo, Community,
                            u5ki.RemoteControlService.OIDs.NDFSimulado.ChannelSpacing + OidEquipo,
                            (int)node.Channeling, SNMPCallTimeout, Port, SNMPVersion);

                        // Modulation
                        SnmpClient.SetInt(ip_equipo, Community,
                            u5ki.RemoteControlService.OIDs.NDFSimulado.Modulation + OidEquipo,
                            (int)node.Modulation, SNMPCallTimeout, Port, SNMPVersion);

                        if (isEmitter)
                        {
                            // Carrier Offset
                            SnmpClient.SetInt(ip_equipo, Community,
                                u5ki.RemoteControlService.OIDs.NDFSimulado.CarrierOffStatus + OidEquipo,
                                (int)node.Offset, SNMPCallTimeout, Port, SNMPVersion);

                            // Power
                            SnmpClient.SetInt(ip_equipo, Community,
                                u5ki.RemoteControlService.OIDs.NDFSimulado.TxPower + OidEquipo,
                                (int)node.PowerLevel, SNMPCallTimeout, Port, SNMPVersion);
                        }

                        Invoke(response, "ConfigureNode.Assing", GearOperationStatus.OK, false);
                        _lastExceptions["ConfigureNode"] = null;
                    }
                }
                catch (Lextm.SharpSnmpLib.Messaging.TimeoutException x)
                {
                    ExceptionTreatment("ConfigureNode", x);
                    Invoke(response, "ConfigureNode.Assing", GearOperationStatus.Timeout, false);
                }
                catch (Exception x)
                {
                    ExceptionTreatment("ConfigureNode", x);
                    Invoke(response, "ConfigureNode.Assing", GearOperationStatus.Fail, false);
                }
            //});
        }

        #endregion IRemoteControl_old

        #region protected



        #endregion protected

        #region static
        #endregion static

        #region Internal



        RdEquipmentStatus SNMPMasterControlConfig(SimulMib simulMib, string sFrecuencia)
        {
            RdEquipmentStatus result = RdEquipmentStatus.NotPresent;

            string ExpectedFrequency = sFrecuencia.Replace(".", "").Replace(",", "");
            string freq = simulMib.Frequency;
            if (simulMib.LastResult == SnmpInterfazResult.Ok)
            {
                string ReadFrequency = freq.Replace(".", "").Replace(",", "");
                result = ReadFrequency == ExpectedFrequency ? RdEquipmentStatus.Ready : RdEquipmentStatus.FailMasterConfig;
            }
            else
            {
                result = simulMib.LastResult== SnmpInterfazResult.Timeout ? RdEquipmentStatus.NotPresent : RdEquipmentStatus.Fail;
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
                SimulMib simulEq = new SimulMib(item.RemoteControlEndp, item.Rid);
                if (item.MainOrStandby == true)
                {
                    RdEquipmentStatus res = SNMPMasterControlConfig(simulEq, item.MainFrequency);
                    if (res == RdEquipmentStatus.FailMasterConfig)
                    {
                        response?.Invoke(true, RdEquipmentStatus.FailMasterConfig);
                        return;
                    }
                }
                response?.Invoke(true, simulEq.Status);
            }
        }

        public void GetMainParams(dynamic rid, Action<dynamic, dynamic> response)
        {
            if (!(rid is RdEquipmentDataItem))
                response?.Invoke(false, new InvalidCastException(rid));
            else
            {
                RdEquipmentDataItem item = (rid as RdEquipmentDataItem);
                try
                {

                }
                catch (Exception x)
                {
                    response?.Invoke(false, x);
                }
            }
        }

        public void SetMainParams(dynamic rid, dynamic par, Action<dynamic, dynamic> response)
        {
            if (!(rid is RdEquipmentDataItem))
                response?.Invoke(false, new InvalidCastException(rid));
            else
            {
                RdEquipmentDataItem item = (rid as RdEquipmentDataItem);
                try
                {

                }
                catch (Exception x)
                {
                    response?.Invoke(false, x);
                }
            }
        }

        #endregion IRemoteControl
    }
}
