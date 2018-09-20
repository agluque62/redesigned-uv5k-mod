using System;
using System.Collections.Generic;
using System.Linq;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System.Windows.Forms;

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

        void PrepareRegistry(string topic)
        {
            reg = new Registry("simul");
            reg.MasterStatusChanged += (p1, p2) =>
            {
            };
            reg.ResourceChanged += (p1, p2) =>
            {
            };
            reg.SubscribeToMasterTopic(topic);
            reg.SubscribeToTopic<string>(topic);
            reg.Join(topic);
        }

        void UnprepareRegistry()
        {
            reg.Dispose();
        }

        [TestMethod]
        public void TestMethod1()
        {
            IService service = new MNManagerService();
            IMNManagerService eventService = (IMNManagerService)service;

            PrepareRegistry(service.Name);

            service.Start();
            Assert.IsTrue(service.Status == U5ki.Infrastructure.ServiceStatus.Running, "Error en Rutina de Arranque");

            DialogResult res = DialogResult.None;

            while (res != DialogResult.Cancel)
            {
                res = MessageBox.Show("Servicio Arrancado", "Caption", MessageBoxButtons.YesNoCancel);
                if (res == DialogResult.Yes)
                {
                    reg.SetValue<string>(service.Name, null, "Hola...");
                    reg.Publish(true);
                }
            }

            service.Stop();
            Assert.IsTrue(service.Status == U5ki.Infrastructure.ServiceStatus.Stopped, "Error en Rutina de Parada");

            UnprepareRegistry();
        }
    }
}
