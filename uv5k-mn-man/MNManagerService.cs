using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Diagnostics;
using System.Reflection;
using System.IO;

using ProtoBuf;
using U5ki.Infrastructure;
using uv5k_mn_mod.Modelo;

namespace uv5k_mn_mod
{
    public class MNManagerService : IService, IMNManagerService
    {
        private const string MNManagerServiceTopic = "m+n-man";
        private string _name;
        private ServiceStatus _status;
        private bool _master;
        private readonly Semaphore accsmp;
        private readonly Helpers.ILogHelper log;

        private Registry _Registry = null;
        private string LastCfgId { get; set; }

        #region IMNManagerService

        public event Action<string, string, string> ChangeUri;
        public event Action<string, bool> EnableUri;

        #endregion

        #region IService

        public string Name { get => _name; private set => _name = value; }
        public ServiceStatus Status { get => _status; private set => _status = value; }
        public bool Master { get => _master; private set => _master = value; }

        public bool Commander(ServiceCommands cmd, string par, ref string err, List<string> resp = null)
        {
            if (accsmp.WaitOne(1000)==true)
            {
                try
                {

                }
                catch(Exception)
                {

                }
                finally
                {
                    accsmp.Release();
                }
            }
            else
            {
                log.From().Error($"Semaphore Timeout {accsmp}");
            }
            return false;
        }

        public bool DataGet(ServiceCommands cmd, ref List<object> rsp)
        {
            if (accsmp.WaitOne(1000) == true)
            {
                try
                {

                }
                catch (Exception)
                {

                }
                finally
                {
                    accsmp.Release();
                }
            }
            else
            {
                log.From().Error($"Semaphore Timeout {accsmp}");
            }
            return false;
        }

        public void Start()
        {
            log.From().Debug($"Starting Service on {Status}");
            if (accsmp.WaitOne(1000) == true)
            {
                try
                {
                    if (Status != ServiceStatus.Running)
                    {
                        _master = false;
                        LastCfgId = String.Empty;
                        Activate();
                        Status = ServiceStatus.Running;
                    }
                }
                catch (Exception x)
                {
                    log.From().TraceException(x);

                    Deactivate(false);
                    Status = ServiceStatus.Stopped;
                }
                finally
                {
                    accsmp.Release();
                }

                log.From().Debug($"Service Started {Status}");
            }
            else
            {
                log.From().Error($"Semaphore Timeout {accsmp}");
            }
            //throw new NotImplementedException();
        }

        public void Stop()
        {
            log.From().Trace($"Stoping Service on {Status}");
            if (accsmp.WaitOne(1000) == true)
            {
                try
                {
                    if (Status != ServiceStatus.Stopped)
                    {
                        Deactivate(true);
                        Status = ServiceStatus.Stopped;
                    }
                }
                catch (Exception x)
                {
                    log.From().TraceException(x);
                }
                finally
                {
                    accsmp.Release();
                }
            }
            else
            {
                log.From().Error($"Semaphore Timeout {accsmp}");
            }
            log.From().Trace($"Service Stopped {Status}");
            //throw new NotImplementedException();
        }

        #endregion

        public MNManagerService()
        {
            Name = MNManagerServiceTopic;
            Status = ServiceStatus.Stopped;
            Master = false;
            accsmp = new Semaphore(1, 1);
            _Registry = null;
            log = new Helpers.LogHelper();
        }

        #region Eventos

        private void _Registry_ResourceChanged(object sender, RsChangeInfo par)
        {
            if (par.Content != null)
            {
                MemoryStream ms = new MemoryStream(par.Content);

                Task.Factory.StartNew(() =>
                {
                    if (accsmp.WaitOne(1000) == true)
                    {
                        try
                        {
                            if (par.Type == Identifiers.TypeId(typeof(Cd40Cfg)))
                            {
                                Cd40Cfg cfg = Serializer.Deserialize<Cd40Cfg>(ms);

                                log.From().Trace($"Configuration Received. {cfg.Version}");
                                if (LastCfgId != cfg.Version)
                                {
                                    LastCfgId = cfg.Version;
                                    Configure(cfg);
                                }
                            }
                            else if (par.Type == Identifiers.TypeId(typeof(MNDisabledNodes)))
                            {
                                MNDisabledNodes NodesInfo = Serializer.Deserialize<MNDisabledNodes>(ms);

                                log.From().Trace($"MN Nodes Change Received: {NodesInfo}");
                                Update(NodesInfo);
                            }
                            else
                            {
                                string str = Serializer.Deserialize<string>(ms);
                                log.From().Trace($"String Received...");
                            }
                        }
                        catch (Exception x)
                        {
                            log.From().TraceException(x);
                        }
                        finally
                        {
                            accsmp.Release();
                        }
                    }
                    else
                    {
                        log.From().Error($"Semaphore Timeout {accsmp}");
                    }
                });
            }
            //throw new NotImplementedException();
        }

        private void _Registry_MasterStatusChanged(object sender, bool par)
        {
            if (accsmp.WaitOne(1000) == true)
            {
                try
                {
                    log.From().Trace($"MasterStatus Changed => {par}");

                    if (!par && Master)
                        SetToSlave();
                    else if (par && !Master)
                        SetToMaster();
                }
                catch (Exception x)
                {
                    log.From().TraceException(x);
                }
                finally
                {
                    accsmp.Release();
                }
            }
            else
            {
                log.From().Error($"Semaphore Timeout {accsmp}");
            }
            throw new NotImplementedException();
        }

        #endregion Eventos

        private void Activate()
        {
            log.From().Trace($"Activating Service.");

            _Registry = new Registry(MNManagerServiceTopic);

            _Registry.MasterStatusChanged += _Registry_MasterStatusChanged;
            _Registry.ResourceChanged += _Registry_ResourceChanged;

            _Registry.SubscribeToMasterTopic(MNManagerServiceTopic);
            _Registry.SubscribeToTopic<MNDisabledNodes>(MNManagerServiceTopic);
            _Registry.SubscribeToTopic<string>(MNManagerServiceTopic);
            _Registry.SubscribeToTopic<Cd40Cfg>(Identifiers.CfgTopic);

            _Registry.Join(MNManagerServiceTopic, Identifiers.CfgTopic);

            log.From().Trace($"Service Activated.");
        }

        private void Deactivate(bool throwException)
        {
            log.Trace($"Deactivating Service.");
            try
            {
                if (Master)
                {
                    SetToSlave();
                }
                if (_Registry != null)
                {
                    _Registry.Dispose();
                    _Registry = null;
                }
            }
            catch(Exception x)
            {
                log.TraceException(x);
                if (throwException) throw x;
            }
            finally
            {
                _Registry = null;
            }
            log.Trace($"Service Deactivated.");
        }

        private void SetToMaster()
        {
            log.Trace($"Setting Service to Master State.");

            Master = true;
            // TODO Start del servicio interno.

            log.Trace($"Service in Master State.");
        }

        private void SetToSlave()
        {
            log.Trace($"Setting Service to Slave State.");

            Master = false;
            // TODO Stop del servicio interno.

            log.Trace($"Service in Slave State.");
        }

        private void Configure(Cd40Cfg cfg)
        {
            log.Trace($"Configuring Service in Master={_master} State");
            // TODO... Configura el servicio interno...

            _Registry.SetValue<string>(MNManagerServiceTopic, null, "Hola Hola");
            _Registry.Publish();

            log.Trace($"Service Configured in Master={_master} State");
        }

        private void Update(MNDisabledNodes updateInfo)
        {
            if (_master==false)
            {

                log.Trace($"Updating Service in Master={_master} State");
                // TODO.. Actualiza el servicio interno...

                log.Trace($"Service Updated in Master={_master} State");
            }
        }
    }
}
