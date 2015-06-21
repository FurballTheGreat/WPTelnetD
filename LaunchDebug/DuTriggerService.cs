using System;
using System.ComponentModel;
using Microsoft.SmartDevice.Connectivity;
using Microsoft.SmartDevice.MultiTargeting.Connectivity;

namespace LaunchDebug
{
    public class DuTriggerService : CcConnectionDeviceService
    {
        public DuTriggerService(
            Device pDeviceToConnect) : base(
            pDeviceToConnect,
            new Guid("769BAC05-0F89-4DE3-9D48-3280FFF185AE"),
            new Guid("5B2263FC-7CEA-44B4-B43B-1ACD5EB5D931"))
        {
            Connect(string.Empty);
            ReadVersionInfo();
        }

        public Version GetPackageVersion(string pPackageName)
        {
            var response = SendServiceCommand("getVersion", pPackageName);
            switch (response.Code)
            {
                case 0:
                    return Version.Parse(response.Lines[0]);
                case unchecked (-2147467259):
                case unchecked (-2147024809):
                    return null;
            }
            throw new Win32Exception(response.Code);
        }

        public void Trigger()
        {
            var enableResponse = SendServiceCommand("enable");
            if(enableResponse.Code!=0)
                throw new Win32Exception(enableResponse.Code);
            var scanResponse = SendServiceCommand("scan");
            if (scanResponse.Code != 0)
                throw new Win32Exception(scanResponse.Code);
        }
    }
}
