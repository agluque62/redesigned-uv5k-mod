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

    public class SnmpDataSet : Dictionary<string, object> { }

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

        public void Get<T>(IPEndPoint endp, string oid, Action<SnmpInterfazResult, Object, Exception> handler)
        {
            try
            {
                var lst = new List<Variable>()
                {
                    new Variable(new ObjectIdentifier(oid))
                };
                Get(endp, lst, (res, ep, val, x) =>
                {
                    if (res == SnmpInterfazResult.Ok && val.Count == 1)
                    {
                        if (typeof(T) == typeof(string) && (val[0].Data is OctetString))
                        {
                            handler(res, val[0].Data.ToString(), x);
                        }
                        else if (typeof(T) == typeof(int?) && (val[0].Data is Integer32))
                        {
                            handler(res, (val[0].Data as Integer32).ToInt32(), x);
                        }
                        else
                        {
                            // TODO. Error tipo no valido
                            handler(SnmpInterfazResult.Error, null, new System.IO.InvalidDataException("SNMP. El tipo solicitado no coincide con el devuelto"));
                        }
                    }
                    else
                    {
                        // TODO. Error Devuelto por SNMP
                        handler(res, null, x);
                    }
                });
            }
            catch (Exception x)
            {
                handler(SnmpInterfazResult.Error, null, x);
            }
        }

        public void Set<T>(IPEndPoint endp, string oid, T valor, Action<SnmpInterfazResult, Exception> handler)
        {
            try
            {
                ISnmpData snmpval = null;

                if (typeof(T) == typeof(string))
                    snmpval = new OctetString(Convert.ToString(valor));
                else if (typeof(T) == typeof(int?))
                    snmpval = new Integer32(Convert.ToInt32(valor));

                if (snmpval != null)
                {
                    var lst = new List<Variable>()
                    {
                        new Variable(new ObjectIdentifier(oid), snmpval)
                    };
                    Set(endp, lst, (res, ep, val, x) =>
                    {
                        handler(res, x);
                    });
                }
                else
                {
                    // TODO. Error tipo no valido
                    handler(SnmpInterfazResult.Error, new ArgumentException("SNMP. El tipo solicitado no esta implementado."));
                }
            }
            catch (Exception x)
            {
                handler(SnmpInterfazResult.Error, x);
            }

        }

        public void Get(IPEndPoint endp, SnmpDataSet set, Action<SnmpInterfazResult, Object, Exception> handler)
        {
            try
            {
                var lst = new List<Variable>();
                foreach(var item in set)
                {
                    lst.Add(new Variable(new ObjectIdentifier(item.Key)));
                }
                Get(endp, lst, (res, ep, val, x) =>
                {
                    if (res == SnmpInterfazResult.Ok)
                    {
                        if (val.Count == set.Count)
                        {
                            foreach(var ret in val)
                            {
                                string oid = ret.Id.ToString();
                                object data = null;

                                if (ret.Data is OctetString)
                                    data = ret.Data.ToString();
                                else if (ret.Data is Integer32)
                                    data = (ret.Data as Integer32).ToInt32();

                                if (set.ContainsKey(oid))
                                {
                                    set[ret.Id.ToString()] = data;
                                }
                                else
                                {
                                    // TODO. ERROR
                                }
                            }
                            handler(SnmpInterfazResult.Ok, set, null);
                        }
                        else
                        {
                            handler(SnmpInterfazResult.Error, null, new System.IO.InvalidDataException("SNMP. No se han obtenido todos los valores."));
                        }
                    }
                    else
                    {
                        handler(res, null, x);
                    }
                });
            }
            catch (Exception x)
            {
                handler(SnmpInterfazResult.Error, null, x);
            }
        }

        public void Set(IPEndPoint endp, SnmpDataSet set, Action<SnmpInterfazResult, Object, Exception> handler)
        {
            try
            {
                var lst = new List<Variable>();
                foreach (var item in set)
                {
                    ISnmpData val = (item.Value is string) ? (ISnmpData)new OctetString((item.Value as string)) : 
                                    (item.Value is int)    ? (ISnmpData)new Integer32((int)item.Value) : null;
                    if (val != null)
                    {
                        lst.Add(new Variable(new ObjectIdentifier(item.Key)));
                    }
                }
                Set(endp, lst, (res, f, val, ex) =>
                {
                    if (res == SnmpInterfazResult.Ok)
                    {
                        if (val.Count == set.Count)
                        {
                            handler(SnmpInterfazResult.Ok, set, null);
                        }
                        else
                        {
                            handler(SnmpInterfazResult.Ok, set, new Exception("SNMP. No se han Modificado todos los valores."));
                        }
                    }
                    else
                    {
                        handler(res, null, x);
                    }
                });
            }
            catch (Exception x)
            {
                handler(SnmpInterfazResult.Error, null, x);
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

        public int Timeout { get; set; }
        public int Reint { get; set; }
        public string Community { get; set; }
    }
}
