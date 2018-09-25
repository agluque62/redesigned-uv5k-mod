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
        Dictionary<string, FreqDataItem> cfgFreq = new Dictionary<string, FreqDataItem>();
        Dictionary<string, RdEquipmentDataItem> cfgRadio = new Dictionary<string, RdEquipmentDataItem>();

        List<RdEquipmentStatus> stdRadio = new List<RdEquipmentStatus>();
        List<RdAssignement> assings = new List<RdAssignement>();

        void IDataService.Configure(dynamic newConfig, Action<dynamic> resultcb)
        {
            var nodes = (newConfig as List<Node>);

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

            /** Obtener las diferencias */
            /** Las eliminaciones (contienen las cambiadas) */
            var actuales = cfgRadio.Values.ToList();
            var foradd = (from act in lequ
                            join old in actuales on act equals old into tmp1
                            from item in tmp1.DefaultIfEmpty()
                            select act).ToList();

            /** Obtengo la nueva lista de frecuencias */
            var lfreq = nodes.GroupBy(g => g.FrecuenciaPrincipal).Select(s => s.First()).ToList()
                .Select(f => new FreqDataItem() { Fid = f.FrecuenciaPrincipal, Priority = f.Prioridad, Band = f.TipoDeFrecuencia });
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
