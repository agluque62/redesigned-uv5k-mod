using System;
using System.Collections.Generic;
using System.Linq;
using Microsoft.VisualStudio.TestTools.UnitTesting;

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

        [TestMethod]
        public void TestMethod1()
        {
            IMNManager manager = MNManagerFactory.Manager;

            manager.ManagementOperation(MNManagerManagementOperations.Configure, cfg, (res, data) =>
            {

            });
            
        }
    }
}
