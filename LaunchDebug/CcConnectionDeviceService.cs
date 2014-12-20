using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Runtime.Remoting.Messaging;
using System.Threading;
using Microsoft.SmartDevice.Connectivity;
using Microsoft.SmartDevice.Connectivity.Interface;
using Microsoft.SmartDevice.Connectivity.Wrapper;
using Microsoft.SmartDevice.MultiTargeting.Connectivity;
using Microsoft.VisualStudio.DeviceConnectivity.Interop;

namespace LaunchDebug
{
    public class ServiceCommandResponse
    {
        public int Code { get; set; }
        public string[] Lines { get; set; }
    }
    public class CcConnectionDeviceService : IDisposable
    {
        private static readonly MultiTargetingConnectivity Targetting;

        static CcConnectionDeviceService()
        {
            Targetting = new MultiTargetingConnectivity(6153);
            
        }

        public static IEnumerable<ConnectableDevice> Devices
        {
            get { return Targetting.GetConnectableDevices(); }
        } 

        private readonly ConnectableDevice _deviceToConnect;
        private readonly Guid _remoteAgentId;
        private readonly Guid _packetStreamId;
        private IRemoteAgent _agent;
        private IDevicePacketStream _packetStream;

        private IDevice _device;
        private IFileDeployer _fileDeployer;
        private ConManServer _conmanServer;

        protected CcConnectionDeviceService(ConnectableDevice pDeviceToConnect, Guid pRemoteAgentId, Guid pPacketStreamId)
        {
            _deviceToConnect = pDeviceToConnect;
            _remoteAgentId = pRemoteAgentId;
            _packetStreamId = pPacketStreamId;

            
           
        }

        public ICcConnection CcConnection
        {
            get { return _conmanServer ; }
        }

        public ICcConnection2 CcConnection2
        {
            get { return _conmanServer as ICcConnection2; }
        }

        public ICcConnection3 CcConnection3
        {
            get { return _conmanServer as ICcConnection3; }
        }

        public ICcConnection4 CcConnection4
        {
            get { return _conmanServer as ICcConnection4; }
        }

        public IDevice Device
        {
            get { return _device; }
        }

        protected void Connect(string pArgs)
        {
            _device = _deviceToConnect.Connect(true);
           
            SetupCcConManServer();

            _device.Activate();
            _fileDeployer = _device.GetFileDeployer();
            _agent = _device.GetRemoteAgent(_remoteAgentId.ToString());
            _agent.Start(pArgs);
            
            _packetStream = _agent.CreatePacketStream(_packetStreamId.ToString());
        }

        private void SetupCcConManServer()
        {
            var fld1 = typeof(DeviceObject).GetFields(BindingFlags.NonPublic |
                         BindingFlags.Instance).First();
            var dev = fld1.GetValue(_device) as Device;
            var conManServerProp = dev.GetType().GetProperty("ConmanServer", BindingFlags.NonPublic |
                                                                             BindingFlags.Instance);
            _conmanServer = conManServerProp.GetValue(dev) as ConManServer;
        }

        protected void SendPacket(Action<IPacket> pFillPacket)
        {
            var packet = _packetStream.CreatePacket();
            pFillPacket(packet);
            _packetStream.Write(packet);
          
        }

        protected IPacket ReadPacket()
        {
            return _packetStream.Read();
        }

        protected T ReadPacket<T>(Func<IPacket, T> pParsePacket )
        {
            var packet = ReadPacket();
            return pParsePacket(packet);

        }
        public void SendFile(string pLocalFile, string pRemoteFile)
        {
            _fileDeployer.SendFile(pLocalFile,pRemoteFile,true,false);
        }

        public void RetrieveFile(string pRemoteFile,string pLocalFile)
        {
            _fileDeployer.ReceiveFile(pRemoteFile, pLocalFile,true);
        }


        public void Dispose()
        {
            _packetStream.Close();
        }

        public bool IsConnected
        {
            get { return _packetStream != null && _packetStream.IsConnected(); }
        }

        public bool IsPacketAvailable
        {
            get
            {
                return _packetStream != null && _packetStream.IsConnected() && _packetStream.IsPacketAvailable();
            }
        }

        protected ServiceCommandResponse SendServiceCommand(string pCmd, params string[] pArgs)
        {
            SendPacket(pPacket =>
            {
                pPacket.WriteString(pCmd);
                foreach (var arg in pArgs)
                    pPacket.WriteString(arg);
            });
            return ReadPacket(pPacket =>
            {
                var code = pPacket.ReadInt32();
                var num = pPacket.ReadInt32();
                var list = new List<string>();
                for (int i = 0; i < num; i++)
                    list.Add(pPacket.ReadString());
                return new ServiceCommandResponse{Code = code,Lines= list.ToArray()};
            });
        }


        protected void ReadVersionInfo()
        {
            while (IsConnected)
            {
                if (IsPacketAvailable)
                {
                    Version = null;
                    StatusString = ReadPacket(pPacket =>
                    {
                        Version = Version.Parse(pPacket.ReadString());
                        return pPacket.ReadString();
                    });

                    break;
                }
            }
        }

        public string StatusString { get; private set; }

        public Version Version { get; private set; }
    }
}