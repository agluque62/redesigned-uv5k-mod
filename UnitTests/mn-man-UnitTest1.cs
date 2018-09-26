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
        class ItemData : IEquatable<ItemData>
        {
            public string Id { get; set; }
            public string variante { get; set; }

            public override bool Equals(object obj)
            {
                return Equals(obj as ItemData);
            }

            public override int GetHashCode()
            {
                return Id == null ? 0 : Id.GetHashCode();
            }

            public bool Equals(ItemData other)
            {
                return Id == other.Id && variante==other.variante;
            }
        }

        class ItemStatus
        {
            public string Id { get; set; }
            public bool Running { get; set; }
            public bool Enabled { get; set; }
        }

        Dictionary<string, ItemData> Config = new  Dictionary<string, ItemData>
        {
            {"id01", new ItemData(){Id="id01", variante = "01"} },
            {"id02", new ItemData(){Id="id02", variante = "02"} },
            {"id03", new ItemData(){Id="id03", variante = "03"} },
            {"id04", new ItemData(){Id="id04", variante = "04"} },
            {"id05", new ItemData(){Id="id05", variante = "05"} },
            {"id06", new ItemData(){Id="id06", variante = "06"} },
        };
        Dictionary<string, ItemStatus> Estado = new Dictionary<string, ItemStatus>();

        List<ItemData> NuevaConfig = new List<ItemData>()
        {
            new ItemData(){Id="id07", variante = "07"},
            new ItemData(){Id="id02", variante = "02"},
            new ItemData(){Id="id03", variante = "03"},
            new ItemData(){Id="id04", variante = "44"},
            new ItemData(){Id="id05", variante = "55"},
            new ItemData(){Id="id06", variante = "06"},
        };

        [TestMethod]
        public void LinqTest01()
        {
            /** Preparacion del Estado Inicial */
            Estado = (from i in Config.Values
                      select (new ItemStatus() { Id = i.Id, Enabled = true, Running = true }))
                      .ToDictionary(g => g.Id, g => g);
            /** No Modificado => disable */
            Estado["id02"].Enabled = false;
            /** Modificado => disable */
            Estado["id04"].Enabled = false;


            /** Recalcular la configuracion */
            var comunes = Config.Values.Intersect(NuevaConfig).ToList();
            var add = NuevaConfig.Except(comunes).ToList();                 // Para añadir (incluye los modificados)
            var del = Config.Values.Except(comunes).ToList();               // Para borrar (incluye los modificados)
            var mod = (from i1 in add                                       // Modificados (para salvar los estados de disable
                      join i2 in del on i1.Id equals i2.Id
                      select new { i1.Id, Estado[i1.Id].Enabled}).ToList();

            /** Generar la nueva configuracion y estado */
            del.ForEach(d =>
            {
                Config.Remove(d.Id);
                Estado.Remove(d.Id);
            });
            add.ForEach(a => 
            {
                Config[a.Id] = a;
                Estado[a.Id] = new ItemStatus() { Id = a.Id, Running = true, Enabled = true };
            });
            /** Recuperar el estado de los modificados.... */
            mod.ForEach(m => Estado[m.Id].Enabled = m.Enabled);

            /** Gestion de Asignaciones */
        }
    }
}
