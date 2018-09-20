using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Reflection;

namespace uv5k_mn_mod
{
    public class TestingRoutines
    {
        class ItemTestReceived
        {
            public string p1 { get; set; }
            public int p2 { get; set; }
            public int p3 { get; set; }
        }
        public static void TestReflectionList(dynamic data)
        {
            Type tipo = data.GetType();

            List<ItemTestReceived> lista = new List<ItemTestReceived>();

            foreach (var item in data)
            {
                Type itipo = item.GetType();

                ItemTestReceived itemr = new ItemTestReceived();

                PropertyInfo[] properties = itipo.GetProperties();
                foreach (var prop in properties)
                {
                    switch (prop.Name)
                    {
                        case "strData":
                            itemr.p1 = (prop.GetValue(item) as string);
                            break;
                        case "intData":
                            itemr.p2 = (int)prop.GetValue(item);
                            break;
                        case "enumData":
                            {
                                var enval = prop.GetValue(item);
                                Object hack1 = Convert.ChangeType(enval, enval.GetTypeCode());
                                itemr.p3 = (int)hack1;
                            }
                            break;
                        default:
                            break;
                    }
                }

                lista.Add(itemr);
            }
        }
    }
}
