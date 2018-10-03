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
    public enum SnmpInterfazResult { Ok, Timeout, Error }
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
        public SnmpInterfaz(string community="public", int timeout=1000, int reint= 2)
        {
            Timeout = timeout;
            Reint = reint;
            Community = community;
        }

        public void Get(IPEndPoint endp, IList<Variable> _data,
            Action<SnmpInterfazResult, IPEndPoint, IList<Variable>, Exception> handler)
        {
            int reint = Reint;
            do
            {
                try
                {
                    IList<Variable> results = Messenger.Get(VersionCode.V2, endp,
                                                new OctetString(Community), _data, Timeout);

                    if (results.Count != _data.Count)
                        throw new SnmpInterfazInternalErrorException($"SnmpClient.GetSet[{endp}]: Invalid result.count");

                    handler(SnmpInterfazResult.Ok, endp, results, null);
                    return;
                }

                catch (Lextm.SharpSnmpLib.Messaging.TimeoutException)
                {
                    // Si es timeout continuo reintentos...
                }

                catch (Exception x)
                {
                    handler(SnmpInterfazResult.Error, null, null, x);
                    return;
                }

            } while (--reint > 0);

            handler(SnmpInterfazResult.Timeout, null, null, new SnmpInterfazTimeoutException($"SnmpClient.Get[{endp}]: No responde..."));
        }

        public void Set(IPEndPoint endp, IList<Variable> _data,
            Action<SnmpInterfazResult, IPEndPoint, IList<Variable>, Exception> handler)
        {
            int reint = Reint;
            do
            {
                try
                {
                    IList<Variable> results = Messenger.Set(VersionCode.V2, endp,
                                                new OctetString(Community), _data, Timeout);

                    if (results.Count != _data.Count)
                        throw new SnmpInterfazInternalErrorException($"SnmpClient.Set[{endp}]: Invalid result.count");

                    handler(SnmpInterfazResult.Ok, endp, results, null);
                    return;
                }

                catch (Lextm.SharpSnmpLib.Messaging.TimeoutException)
                {
                    // Si es timeout continuo reintentos...
                }

                catch (Exception x)
                {
                    handler(SnmpInterfazResult.Error, null, null, x);
                    return;
                }

            } while (--reint > 0);

            handler(SnmpInterfazResult.Timeout, null, null, 
                new SnmpInterfazTimeoutException($"SnmpClient.Set[{endp}]: No responde..."));
        }

        public void GetInt(IPEndPoint endp, string oid, Action<SnmpInterfazResult, int, Exception> handler)
        {
            try
            {
                var lst = new List<Variable>()
                {
                    new Variable(new ObjectIdentifier(oid))
                };
                Get(endp, lst, (res, ep, val, x) =>
                {
                    if (res==SnmpInterfazResult.Ok)
                    {
                        int _ret = 0;
                        if (int.TryParse(val[0].Data.ToString(), out _ret) == false)
                            throw new SnmpInterfazInternalErrorException($"CienteSnmp.GetInt: TryParse(result[0].Data.ToString(): {ip}---{oid}");
                        handler(res, _ret, null);
                    }
                    else
                    {
                        handler(res, 0, x);
                    }
                });
            }
            catch (Exception x)
            {
                handler(SnmpInterfazResult.Error, 0, x);
            }
        }

        public void SetInt(IPEndPoint endp, string oid, int valor, Action<SnmpInterfazResult, Exception> handler)
        {
            try
            {
                var lst = new List<Variable>()
                {
                    new Variable(new ObjectIdentifier(oid), new Integer32(valor))
                };
                Set(endp, lst, (res, ep, val, x) =>
                {
                    handler(res, x);
                });
            }
            catch (Exception x)
            {
                handler(SnmpInterfazResult.Error, x);
            }
        }

        public void GetString(IPEndPoint endp, string oid, Action<SnmpInterfazResult, string, Exception> handler)
        {
            try
            {
                var lst = new List<Variable>()
                {
                    new Variable(new ObjectIdentifier(oid))
                };

                Get(endp, lst, (res, ep, val, x) =>
                {
                    handler(res, res == SnmpInterfazResult.Ok ? val[0].Data.ToString() : string.Empty, x);
                });
            }
            catch (Exception x)
            {
                handler(SnmpInterfazResult.Error, null, x);
            }
        }

        public void SetString(IPEndPoint endp, string oid, String valor, Action<SnmpInterfazResult, Exception> handler)
        {
            try
            {
                var lst = new List<Variable>()
                {
                    new Variable(new ObjectIdentifier(oid), new OctetString(valor))
                };
                Set(endp, lst, (res, ep, val, x) =>
                {
                    handler(res, x);
                });
            }
            catch (Exception x)
            {
                handler(SnmpInterfazResult.Error, x);
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
        protected string Community { get; set; }
    }
}
