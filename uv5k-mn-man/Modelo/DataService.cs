using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.Threading.Tasks;

using U5ki.Infrastructure;
using Utilities;

using uv5k_mn_mod.Helpers;

namespace uv5k_mn_mod.Modelo
{
    public interface IDataService
    {
        void Configure(dynamic newConfig, Action<dynamic> resultcb);
    }

    public class DataService : IDataService
    {
        /** Configuracion de Equipos y Estado */
        private Dictionary<string, RdEquipmentDataItem> cfgRadio = new Dictionary<string, RdEquipmentDataItem>();
        private Dictionary<string, RdEquipmentStatus> stdRadio = new Dictionary<string, RdEquipmentStatus>();
        private List<FreqDataItemAssign> stdFreq = new List<FreqDataItemAssign>();

        private Object globalLocker = new object();

        void IDataService.Configure(dynamic newConfig, Action<dynamic> resultcb)
        {
            var nodes = (newConfig as List<Node>);

            // TODO. Obtener acceso exclusivo a las tablas...
            lock (globalLocker)
            {
                /** Obtengo la nueva lista de equipos */
                var lequ = nodes.Select(e => new RdEquipmentDataItem()
                {
                    Rid = e.Id,
                    MainOrStandby = e.FormaDeTrabajo,
                    Model = e.ModeloEquipo,
                    Band = e.TipoDeFrecuencia,
                    Site = e.idEmplazamiento,
                    MainFrequency = e.FrecuenciaPrincipal,
                    MainPriority = e.Prioridad,
                    MainPower = e.Potencia,
                    MainCarrierOffset = e.Offset,
                    MainChannelSpace = e.Canalizacion,
                    MainModulationMode = e.Modulacion,
                    RemoteControlBaseOid = e.Oid,
                    RemoteControlEndp = ManagementEndpoint(e),
                    SipUri = e.SipUri,
                    TxRx = e.EsTransmisor,
                    StandbyFrequeciesRange = e.Frecs
                }).ToList();

                /** Obtener las diferencias, Las eliminaciones (contienen las cambiadas) */
                var nomodificadas = lequ.Intersect(cfgRadio.Values.ToList());
                var fordel = cfgRadio.Values.ToList().Except(nomodificadas).ToList();
                var foradd = lequ.Except(nomodificadas).ToList();

                var mod = (from i1 in foradd                                       // Modificados (para salvar los estados de disable
                           join i2 in fordel on i1.Rid equals i2.Rid
                           select new { i1.Rid, stdRadio[i1.Rid].IsEnabled }).ToList();
                /** Generar la nueva configuracion y estado */
                fordel.ForEach(d =>
                {
                    cfgRadio.Remove(d.Rid);
                    stdRadio.Remove(d.Rid);
                    stdFreq.RemoveAll(f => f.DefaultRid == d.Rid || f.ActualRid == d.Rid);
                // TODO Generar el evento de borrado hacia el servicio radio.
            });
                foradd.ForEach(a =>
                {
                    cfgRadio[a.Rid] = a;
                    stdRadio[a.Rid] = new RdEquipmentStatus() { Rid = a.Rid, IsAlive = false, IsOk = false, IsEnabled = true, RemoteControlManager = null };
                    if (a.MainOrStandby)
                    {
                        stdFreq.Add(new FreqDataItemAssign()
                        {
                            Fid = a.MainFrequency,
                            Band = a.Band,
                            Priority = a.MainPriority,
                            Site = a.Site,
                            TxOrRx = a.TxRx,
                            DefaultRid = a.Rid,
                            ActualRid = a.Rid
                        });
                    // TODO Generar el evento hacia el servicio radio (si procede)
                }
                });
                /** Recuperar el estado de los modificados.... */
                mod.ForEach(m => stdRadio[m.Rid].IsEnabled = m.IsEnabled);
            }
        }


        IPEndPoint ManagementEndpoint(Node e)
        {
            try
            {
                if (GeneralHelper.IsValidIpV4(e.IpGestor))
                {
                    return new IPEndPoint(IPAddress.Parse(e.IpGestor), (int)e.Puerto);
                }
                SipUtilities.SipUriParser sipuri = new SipUtilities.SipUriParser(e.SipUri);
                if (GeneralHelper.IsValidIpV4(sipuri.Host))
                {
                    return new IPEndPoint(IPAddress.Parse(sipuri.Host), (int)e.Puerto);
                }
            }
            catch(Exception x)
            {

            }
            return null;
        }
    }
}
