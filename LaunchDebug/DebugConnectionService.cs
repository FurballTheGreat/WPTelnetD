using System;
using Microsoft.SmartDevice.Connectivity;
using Microsoft.SmartDevice.MultiTargeting.Connectivity;

namespace LaunchDebug
{
    public class DebugConnectionService : CcConnectionDeviceService
    {
        private const string PhoneMsvsmonPath =
            @"\DATA\SharedData\PhoneTools\11.0\Debugger\bin\RemoteDebugger\msvsmon.exe";
        private const string PhoneNdtkClientPath =
           @"\DATA\SharedData\PhoneTools\11.0\Debugger\bin\RemoteDebugger\ndtkclient.dll";
        public DebugConnectionService(Device pDeviceToConnect)
            : base(pDeviceToConnect,
                new Guid("88B0B986-543F-4F04-8E5F-A80A7E189F11"),
                new Guid("8995B972-A375-4B6D-90A5-A778CD43FCB1"))
        {
            Connect(string.Empty);
        }

        private class DebugResponsePacket
        {
            public int Id { get; set; }
            public int Status { get; set; }
        }

        public int DebugLaunch(string pExeToLaunch,  Guid pProductId)
        {

            SendFile(pExeToLaunch,PhoneMsvsmonPath);

            SendFile("ndtkclient.dll", PhoneNdtkClientPath);

            // This is the only debug command ID I have seen so far, launch msvsmon. Their may be more....
            SendPacket(pPacket => pPacket.WriteInt32(1));

            SendPacket(pPacket => pPacket.WriteString("{" + pProductId + "}"));

           
            var packet = ReadPacket(pPacket=>new DebugResponsePacket
            {
                Id = pPacket.ReadInt32(), 
                Status = pPacket.ReadInt32()
            });
            return packet.Status;
        }

        public void UpdateApp(Guid pGuid, string pXap)
        {
            var app = Device.GetApplication(pGuid);
            
           app.UpdateApplication("0", null,pXap);        
        }

        public void UninstallApp(Guid pGuid)
        {
            var app = Device.GetApplication(pGuid);
            app.Uninstall();
        }

        public void InstallApp(Guid pGuid, string pXap)
        {
            Device.InstallApplication(
              pGuid,
              Guid.NewGuid(),
              "256", null, pXap);
        }

     
    }
}