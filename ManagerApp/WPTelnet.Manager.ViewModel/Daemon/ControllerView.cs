using System;
using System.Collections.Generic;
using System.Threading;
using Windows.Networking.Connectivity;
using WPTelnet.Manager.ViewModel.Common;

namespace WPTelnet.Manager.ViewModel.Daemon
{
    public class ControllerView : BaseView
    {
        private string _status;
        private const string DAEMON_LAUNCH = "wptd.exe";
        private bool _hasInit = false;
        private string _inProcessStatus;

        public ControllerView()
        {
            Installations = new InstallationManagerViewModel();
            SmsIntercept = new SmsInterceptViewModel();
            LaunchCommand=new BaseCommand(() =>
            {
                if (!IsRunning)
                {
                    try
                    {
                        var result = NativeMethods.Current.CreateProcess(
                            DAEMON_LAUNCH);
                        // string.Format("{0} {1} {2} {3}", DAEMON_LAUNCH, Port, UserName, Password).Trim();
                        if (result == null)
                        {
                            var error = NativeMethods.Current.GetLastError();
                            Status = "Failed " + error;
                        }
                        else
                        {
                            Status = "Success ";
                        }
                    }
                    catch (Exception e)
                    {
                        Status = "Exception:"+ e.Message + " "+e.GetType().Name;
                    }
                }
            });

            LaunchListenInProcess = new BaseCommand(OnLaunchListenInProcess);
            Port = 23422;
        }

        public SmsInterceptViewModel SmsIntercept { get; set; }

        private void OnLaunchListenInProcess(object pObj)
        {
            var thread = new Thread(() =>
            {
                try
                {
                    if (!_hasInit)
                    {
                        TelnetControlMethods.InitNetWorking();
                        _hasInit = true;
                    }
                    Dispatch(() =>
                    {
                        InProcessStatus = "Listening...";
                    });
                    var socket = TelnetControlMethods.ListenForOneConnection(Port);
                    Dispatch(() =>
                    {
                        InProcessStatus = "Got connection proceeding to process";
                    });
                    TelnetControlMethods.ProcessConnection(socket, "This instance is hosted inside the app process.");
                    Dispatch(() =>
                    {
                        InProcessStatus = "Disconnected";
                    });
                }
                catch (Exception e)
                {
                    Dispatch(() =>
                    {
                        InProcessStatus = e.Message;
                    });
                   
                }
            });
            thread.Start();

        }

        

        protected bool IsRunning
        {
            get; set;
        }

        public string UserName { get; set; }
        public string Password { get; set; }
        public int Port { get; set; }

        public BaseCommand LaunchCommand { get; private set; }

        public string Status
        {
            get { return _status; }
            set
            {
                _status = value;
                OnPropertyChanged("Status");
            }
        }

        public string InProcessStatus
        {
            get { return _inProcessStatus; }
            set
            {
                _inProcessStatus = value;
                OnPropertyChanged("InProcessStatus");
            }
        }

        public string CommandLine
        {
            get { return NativeMethods.Current.GetCommandLine(); }
        }

        public BaseCommand LaunchListenInProcess { get; private set; } 

        public string[] IpAddresses
        {
            get
            {
                var address = new List<string>();
                var hostnames = NetworkInformation.GetHostNames();
                foreach (var hn in hostnames)
                {
                    if (hn.IPInformation != null)
                    {
                        address.Add(hn.DisplayName);

                    }
                }

                return address.ToArray();
            }
        }

        public InstallationManagerViewModel Installations { get; private set; }
    }
}
