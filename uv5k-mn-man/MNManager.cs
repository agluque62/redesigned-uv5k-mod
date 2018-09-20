using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Reflection;

using uv5k_mn_mod.Modelo;

namespace uv5k_mn_mod
{
    public enum MNManagerStates { Stopped, Running, Error }
    public enum MNManagerManagementOperations { Start, Stop, Reset, Configure, UpdateForbiddens }
    public enum MNManagerUserOperations { Info, Disable, Enable, Sintonize, EquipmentData }
    public enum MNManagerResultCodes { Ok }
    /// <summary>
    /// 
    /// </summary>
    public interface IMNManager
    {
        event Action<string, string, string> ChangeUri;         // Frecuencia, URI Actual, URI Nueva.
        event Action<string, bool> EnableUri;                   // URI del EQUIPO. Enable/Disable. 

        MNManagerStates Status { get; set; }

        void ManagementOperation(MNManagerManagementOperations operation, object data, Action<MNManagerResultCodes, object> ResultManager);
        void UserOperation(MNManagerUserOperations operation, object data, Action<MNManagerResultCodes, object> ResultManager);
    }
    /// <summary>
    /// 
    /// </summary>
    public class MNManagerFactory
    {
        private static IMNManager s_manager = new MNManager();

        public static IMNManager Manager { get => s_manager; set => s_manager = value; }
    }
    /// <summary>
    /// 
    /// </summary>
    class MNManager : IMNManager, IDisposable
    {
        public MNManagerStates Status { get => throw new NotImplementedException(); set => throw new NotImplementedException(); }

        public event Action<string, string, string> ChangeUri;
        public event Action<string, bool> EnableUri;

        public void ManagementOperation(MNManagerManagementOperations operation, object data, Action<MNManagerResultCodes, object> ResultManager)
        {
            switch(operation)
            {
                case MNManagerManagementOperations.Configure:
                    TestReflectionList(data);
                    break;

                default:
                   throw new NotImplementedException();
            }
            ResultManager?.Invoke(MNManagerResultCodes.Ok, null);
        }

        public void UserOperation(MNManagerUserOperations operation, object data, Action<MNManagerResultCodes, object> ResultManager)
        {
            throw new NotImplementedException();
        }

        #region IDisposable Support
        private bool disposedValue = false; // Para detectar llamadas redundantes

        public MNManager()
        {
        }

        protected virtual void Dispose(bool disposing)
        {
            if (!disposedValue)
            {
                if (disposing)
                {
                    // TODO: elimine el estado administrado (objetos administrados).
                }

                // TODO: libere los recursos no administrados (objetos no administrados) y reemplace el siguiente finalizador.
                // TODO: configure los campos grandes en nulos.

                disposedValue = true;
            }
        }

        // TODO: reemplace un finalizador solo si el anterior Dispose(bool disposing) tiene código para liberar los recursos no administrados.
        // ~MNManager() {
        //   // No cambie este código. Coloque el código de limpieza en el anterior Dispose(colocación de bool).
        //   Dispose(false);
        // }

        // Este código se agrega para implementar correctamente el patrón descartable.
        public void Dispose()
        {
            // No cambie este código. Coloque el código de limpieza en el anterior Dispose(colocación de bool).
            Dispose(true);
            // TODO: quite la marca de comentario de la siguiente línea si el finalizador se ha reemplazado antes.
            // GC.SuppressFinalize(this);
        }
        #endregion

        #region TESTING
#if DEBUG
        class ItemTestReceived
        {
            public string p1 { get; set; }
            public int p2 { get; set; }
            public int p3 { get; set; }
        }
        void TestReflectionList(dynamic data)
        {
            Type tipo = data.GetType();

            List<ItemTestReceived> lista = new List<ItemTestReceived>();

            foreach (var item in data)
            {
                Type itipo = item.GetType();

                ItemTestReceived itemr = new ItemTestReceived();

                PropertyInfo[] properties = itipo.GetProperties();
                foreach(var prop in properties)
                {
                    switch(prop.Name)
                    {
                        case "strData":
                            itemr.p1 = (prop.GetValue(item) as string);
                            break;
                        case "intData":
                            itemr.p2 = (int)prop.GetValue(item);
                            break;
                        case "enumData":
                            itemr.p3 = (int)prop.GetValue(item);
                            break;
                        default:
                            break;
                    }
                }

                lista.Add(itemr);
            }
        }
#endif
        #endregion
    }

}
