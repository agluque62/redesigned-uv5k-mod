using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;

using Lextm.SharpSnmpLib;
using Lextm.SharpSnmpLib.Messaging;
using Lextm.SharpSnmpLib.Objects;
using Lextm.SharpSnmpLib.Pipeline;


namespace uv5k_mn_mod.Servicios.RemoteControl
{
    public class SnmpInterfazTimeoutException : ApplicationException
    {
        public SnmpInterfazTimeoutException(string msg) : base(msg)
        {
        }
    }

    public class SnmpInterfazInternalErrorException : ApplicationException
    {
        public SnmpInterfazInternalErrorException(string msg) : base(msg)
        {
        }
    }

    public class SnmpInterfaz
    {
        public SnmpInterfaz(int port=161, string community="public", int timeout=1000, int reint= 2)
        {
            Timeout = timeout;
            Reint = reint;
            Port = port;
            Community = community;
        }

        public void Get(string ip, IList<Variable> _data,
            Action<bool, IPEndPoint, IList<Variable>, Exception> handler)
        {
            int reint = Reint;
            do
            {
                try
                {
                    IList<Variable> results = Messenger.Get(VersionCode.V2, new IPEndPoint(IPAddress.Parse(ip), Port),
                                                new OctetString(Community), _data, Timeout);

                    if (results.Count != _data.Count)
                        throw new SnmpInterfazInternalErrorException($"SnmpClient.GetSet[{ip}]: Invalid result.count");

                    handler(true, new IPEndPoint(IPAddress.Parse(ip), Port), results, null);
                    return;
                }

                catch (Lextm.SharpSnmpLib.Messaging.TimeoutException)
                {
                    // Si es timeout continuo reintentos...
                }

                catch (Exception x)
                {
                    handler(false, null, null, x);
                    return;
                }

            } while (--reint > 0);

            throw new SnmpInterfazTimeoutException(String.Format("SnmpClient.GetSet[{0}]: No responde...", ip));
        }

        public void Set(string ip, IList<Variable> _data,
            Action<bool, IPEndPoint, IList<Variable>, Exception> handler)
        {
            int reint = Reint;
            do
            {
                try
                {
                    IList<Variable> results = Messenger.Set(VersionCode.V2, new IPEndPoint(IPAddress.Parse(ip), Port),
                                                new OctetString(Community), _data, Timeout);

                    if (results.Count != _data.Count)
                        throw new SnmpInterfazInternalErrorException($"SnmpClient.Set[{ip}]: Invalid result.count");

                    handler(true, new IPEndPoint(IPAddress.Parse(ip), Port), results, null);
                    return;
                }

                catch (Lextm.SharpSnmpLib.Messaging.TimeoutException)
                {
                    // Si es timeout continuo reintentos...
                }

                catch (Exception x)
                {
                    handler(false, null, null, x);
                    return;
                }

            } while (--reint > 0);

            throw new SnmpInterfazTimeoutException(String.Format("SnmpClient.Set[{0}]: No responde...", ip));
        }

        public void GetInt(string ip, string oid, Action<bool, int, Exception> handler)
        {
            try
            {
                var lst = new List<Variable>()
                {
                    new Variable(new ObjectIdentifier(oid))
                };
                Get(ip, lst, (ok, ep, res, x) =>
                {
                    if (ok)
                    {
                        int _ret = 0;
                        if (int.TryParse(res[0].Data.ToString(), out _ret) == false)
                            throw new SnmpInterfazInternalErrorException($"CienteSnmp.GetInt: TryParse(result[0].Data.ToString(): {ip}---{oid}");
                        handler(true, _ret, null);
                    }
                    else
                    {
                        handler(false, 0, x);
                    }
                });
            }
            catch (Exception x)
            {
                handler(false, 0, x);
            }
        }

        public void SetInt(string ip, string oid, int valor, Action<bool, Exception> handler)
        {
            try
            {
                var lst = new List<Variable>()
                {
                    new Variable(new ObjectIdentifier(oid), new Integer32(valor))
                };
                Set(ip, lst, (ok, ep, res, x) =>
                {
                    handler(ok, x);
                });
            }
            catch (Exception x)
            {
                handler(false, x);
            }
        }

        public void GetString(string ip, string oid, Action<bool, string, Exception> handler)
        {
            try
            {
                var lst = new List<Variable>()
                {
                    new Variable(new ObjectIdentifier(oid))
                };

                Get(ip, lst, (ok, ep, res, x) =>
                {
                    if (ok)
                    {
                        handler(true, res[0].Data.ToString(), null);
                    }
                    else
                    {
                        handler(false, null, x);
                    }
                });
            }
            catch (Exception x)
            {
                handler(false, null, x);
            }
        }

        public void SetString(string ip, string oid, String valor, Action<bool, Exception> handler)
        {
            try
            {
                var lst = new List<Variable>()
                {
                    new Variable(new ObjectIdentifier(oid), new OctetString(valor))
                };
                Set(ip, lst, (ok, ep, res, x) =>
                {
                    handler(ok, x);
                });
            }
            catch (Exception x)
            {
                handler(false, x);
            }
        }

        //public void TrapTo(string ipTo, int port, string community, string oid, string val)
        //{
        //    List<Variable> lst = new List<Variable>();
        //    Variable var = new Variable(new ObjectIdentifier(oid), new OctetString(val));
        //    lst.Add(var);

        //    Messenger.SendTrapV2(0, VersionCode.V2,
        //        new IPEndPoint(IPAddress.Parse(ipTo), port),
        //        new OctetString(community),
        //        new ObjectIdentifier(oid),
        //        0, lst);
        //}

        //public int Integer(ISnmpData data)
        //{
        //    return (data is Integer32) ? ((Integer32)data).ToInt32() : -1;
        //}

        protected int Timeout { get; set; }
        protected int Reint { get; set; }
        protected int Port { get; set; }
        protected string Community { get; set; }
    }
}
