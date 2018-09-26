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

        private Dictionary<string, FreqDataItem>  cfgFreq = new Dictionary<string, FreqDataItem>();
        private Dictionary<string, FreqDataStatus> stdFreq = new Dictionary<string, FreqDataStatus>();

        void IDataService.Configure(dynamic newConfig, Action<dynamic> resultcb)
        {
            var nodes = (newConfig as List<Node>);

            // TODO. Obtener acceso exclusivo a las tablas...

            /** Obtengo la nueva lista de equipos */
            var lequ = nodes.Select(e => new RdEquipmentDataItem()
            {
                Rid = e.Id,
                MainOrStanby = e.FormaDeTrabajo, Model = e.ModeloEquipo,
                Band = e.TipoDeFrecuencia, Site = e.idEmplazamiento, MainFrequency = e.FrecuenciaPrincipal,
                MainPower = e.Potencia, MainCarrierOffset = e.Offset, MainChannelSpace = e.Canalizacion, MainModulationMode = e.Modulacion, 
                RemoteControlBaseOid = e.Oid, RemoteControlEndp = ManagementEndpoint(e),
                SipUri = e.SipUri, TxRx = e.EsTransmisor, StandbyFrequeciesRange = e.Frecs
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
            });
            foradd.ForEach(a =>
            {
                cfgRadio[a.Rid] = a;
                stdRadio[a.Rid] = new RdEquipmentStatus() { Rid = a.Rid, IsAlive=false, IsOk=false, IsEnabled=true, RemoteControlManager=null };
            });
            /** Recuperar el estado de los modificados.... */
            mod.ForEach(m => stdRadio[m.Rid].IsEnabled = m.IsEnabled);


            /** Obtengo la nueva lista de frecuencias */
            var lfreq = (from n in nodes
                         group n by n.FrecuenciaPrincipal into g
                         select g.First());
            var dfreq = lfreq.ToDictionary(i => i.FrecuenciaPrincipal, i => new FreqDataItem()
            {
                Fid = i.FrecuenciaPrincipal, Band = i.TipoDeFrecuencia, Priority=i.Prioridad
            });
            var fnomod = dfreq.Values.Intersect(cfgFreq.Values);            
            var ffdel = cfgFreq.Values.Except(fnomod);
            var ffadd = dfreq.Values.Except(fnomod);

            /** Hay que añadir a ambas listas las frecuencias cuyos equipos actualmente asignados hayan sido modificados */
            var rids = mod.Select(m => m.Rid).ToList();
            //var fids1 = stdFreq.Values.SelectMany(f=>f);    //.SelectMany(m => m.Rid)).ToList();

            //// Gestion de Asignaciones...
            //// Borro las modificaciones (borrados de equipos)
            //var rids = fordel.SelectMany(i => i.Rid).ToList();
            //var toremove = assigns.Where(a => rids.Contains(a.Rid)).ToList();
            //toremove.ForEach(e =>
            //{
            //    // TODO. Generar la notificacion del evento al Servicio Radio General....
            //});
            //assigns.RemoveAll(a => rids.Contains(a.Rid));


            // TODO. devolver el acceso exclusivo a las tablas...
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
