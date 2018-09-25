using System;
using System.Collections.Generic;
using System.Linq;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System.Windows.Forms;
using System.Threading.Tasks;

using U5ki.Infrastructure;
using uv5k_mn_mod;

namespace UnitTests
{
    enum  enumDataValues { Cero, Uno, Dos, Tres }
    class DataItem
    {
        public string strData { get; set; }
        public int intData { get; set; }
        public enumDataValues enumData { get; set; }
        public DataItem()
        {
            strData = (Guid.NewGuid()).ToString();
            intData = strData.GetHashCode();
            enumData = enumDataValues.Dos;
        }
    }


    [TestClass]
    public class MNMan_UnitTest
    {
        readonly List<DataItem> cfg = new List<DataItem>() { new DataItem(), new DataItem(), new DataItem() };
        readonly List<Node> nodes = new List<Node>() { new Node(), new Node() };

        Registry reg = null;

        void PrepareRegistry(string testing_topic)
        {
            reg = new Registry("utest");
            reg.MasterStatusChanged += (p1, p2) =>
            {
            };
            reg.ResourceChanged += (p1, p2) =>
            {
            };
            reg.SubscribeToMasterTopic(testing_topic);
            reg.SubscribeToTopic<string>(testing_topic);
            reg.Join(testing_topic);
        }

        void UnprepareRegistry()
        {
            reg.Dispose();
        }

        [TestMethod]
        public void TestMethod1()
        {
            using (IDisposable service_dispose = new MNManagerService())
            {
                IService service = (service_dispose as IService);

                PrepareRegistry(service.Name);

                service.Start();
                Assert.IsTrue(service.Status == U5ki.Infrastructure.ServiceStatus.Running, "Error en Rutina de Arranque");

                DialogResult res = DialogResult.None;

                while (res != DialogResult.Cancel)
                {
                    res = MessageBox.Show("Servicio Arrancado", "Caption", MessageBoxButtons.YesNoCancel);
                    if (res == DialogResult.Yes)
                    {
                        reg.SetValue<string>(service.Name, "fromsimul", "Hola...");
                        reg.Publish(true);
                    }
                    else if (res == DialogResult.No)
                    {
                        UnprepareRegistry();
                        Task.Delay(2000).Wait();
                        PrepareRegistry(service.Name);
                    }
                }

                service.Stop();
                Assert.IsTrue(service.Status == U5ki.Infrastructure.ServiceStatus.Stopped, "Error en Rutina de Parada");

                UnprepareRegistry();
            }
        }
    }


    [TestClass]
    public class LinqTest
    {
        class ItemData
        {
            public string invariante { get; set; }
            public string variante { get; set; }
        }

        List<ItemData> actual = new List<ItemData>()
        {
            new ItemData(){invariante="id01", variante = "01"},
            new ItemData(){invariante="id02", variante = "02"},
            new ItemData(){invariante="id03", variante = "03"},
            new ItemData(){invariante="id04", variante = "04"},
            new ItemData(){invariante="id05", variante = "05"},
            new ItemData(){invariante="id06", variante = "06"},
        };
        List<ItemData> nueva = new List<ItemData>()
        {
            new ItemData(){invariante="id07", variante = "07"},
            new ItemData(){invariante="id02", variante = "02"},
            new ItemData(){invariante="id03", variante = "03"},
            new ItemData(){invariante="id04", variante = "44"},
            new ItemData(){invariante="id05", variante = "55"},
            new ItemData(){invariante="id06", variante = "06"},
        };

        [TestMethod]
        public void linqTest01()
        {
            var foradd = (from act in nueva
                          join old in actual on act.invariante equals old.invariante into tmp1
                          from item in tmp1.DefaultIfEmpty()
                          select item).ToList();

        }
    }
}
