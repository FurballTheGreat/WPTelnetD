using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace AppRepurposer.Common
{
    public interface IHostableApp
    {
        Task<bool> Replace(Action<string> pNotify);
        bool Present { get; }
        event Action PresentChanged;
        void UpdatePresent();
        string Name { get; }
    }

    public static class HostableApps
    {
        public static IHostableApp[] Apps =
        {
            new HostableXapApp
            {
                Name = "Extras & Info", 
                Guid = new Guid("2377fe1b-c10f-47da-92f3-fc517345a3c0"),
                Xap = "wptelnetd.xap"
            },
            new HostableXapApp
            {
                Name = "Preview For Developers", 
                Guid = new Guid("178ac8a1-6519-4a0b-960c-038393741e96"),
                Xap = "wptelnetd.xap"
            }, 
            new HostableXapApp
            {
                Name = "Glance Background", 
                Guid = new Guid("7c3378ff-2844-480b-b2e2-783537201b84"),
                Xap = "wptelnetd.xap"
            },
            new HostableXapApp
            {
                Name = "HTC CSDDiag", 
                Guid = new Guid("f81239cf-2e0f-4fc1-9141-a66e080b83f9"),
                Xap = "wptelnetd.xap"
            },
            new HostableXapApp
            {
                Name = "DiagnosticTool_Sprint", 
                Guid = new Guid("e69a2877-59b8-43ed-898d-554fbc4b8b2b"),
                Xap = "wptelnetd.xap"
            },
            new HostableXapApp
            {
                Name = "HTC Connection Setup", 
                Guid = new Guid("5edbdbbc-2ab2-df11-8a2f-00237de2db9e"),
                Xap = "wptelnetd.xap"
            },
            new HostableAppx
            {
                Name = "Field Medic",
                SourcePath = "new",
                AppDirName = "Microsoft.WPDiet_2.0.15086.130_arm__8wekyb3d8bbwe",
                AppFiles = new []{"etwlogger.dll", "wptd.exe"}
            },
            new HostableAppx
            {
                Name = "Phone Insider",
                AppDirName = "Microsoft.Upprv_1.1.1211.12_arm__8wekyb3d8bbwe",
                SourcePath = "new",
                AppFiles = new []{"DeviceUpdateUtils.dll",/* "wptd.exe","TestFlights.exe",*/"WindowsPhoneReservedAppInfo.xml"}
            },
            new HostableAppx
            {
                Name = "Phone Insider Drop",
                AppDirName = "Microsoft.Upprv_1.1.1211.12_arm__8wekyb3d8bbwe",
                SourcePath = "new",
                AppFiles = new []
                {
                    "TestFlights.exe"
                }
            },
        };

    };

    public class HostableAppx : IHostableApp
    {
        async Task<bool> IHostableApp.Replace(Action<string> pNotify)
        {
            return await Logic.DoAppxReplace(pNotify, AppDirName, SourcePath, AppFiles);
        }

        public bool Present { get; private set; }

        string IHostableApp.Name
        {
            get {return Name; }
        }

        public async void UpdatePresent()
        {
            Present = await Logic.AppxExists(AppDirName);
            if (PresentChanged != null)
                PresentChanged();
        }

        public event Action PresentChanged;

        public string Name { get; set; }

        public string AppDirName { get; set; }

        public string SourcePath { get; set; }

        public string[] AppFiles { get; set; }
    }

    public class HostableXapApp : IHostableApp
    {
        async Task<bool> IHostableApp.Replace(Action<string> pNotify)
        {
            return await Logic.DoXapReplace(Guid.ToString(), Xap, pNotify);
        }

        public event Action PresentChanged;


        public async void UpdatePresent()
        {
  
            Present = await Logic.XapExists(Guid);
            if (PresentChanged != null)
                PresentChanged();
        }
      
        public bool Present { get; private set; }

        public string Name { get; set; }
        public Guid Guid { get; set; }
        public string Xap { get; set; }
    }
}
