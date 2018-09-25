using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace uv5k_mn_mod.Helpers
{
    public class GeneralHelper
    {
        const string ipv4_regexp = "^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?))$";
        public static bool IsValidIpV4(string ip)
        {
            return Regex.Match(ip, ipv4_regexp).Success;
        }
    }
}
