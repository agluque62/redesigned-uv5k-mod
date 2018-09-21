using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace U5ki.Infrastructure
{
    public interface IMNManagerService
    {
        event Action<string, string, string> ChangeUri;         // Frecuencia, URI Actual, URI Nueva.
        event Action<string, bool> EnableUri;                   // URI del EQUIPO. Enable/Disable. 
    }
}
