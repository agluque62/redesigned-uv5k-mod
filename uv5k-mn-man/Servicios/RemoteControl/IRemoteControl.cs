using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace uv5k_mn_mod.Servicios
{
    public interface IRemoteControl
    {
        void CheckNode(Action<dynamic> response);
        void GetMainParams(Action<dynamic> response);
        void SetMainParams(Action<dynamic> response, params object[] par);
    }
}
