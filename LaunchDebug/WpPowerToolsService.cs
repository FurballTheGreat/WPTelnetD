using System;
using System.ComponentModel;
using System.IO;
using Microsoft.SmartDevice.MultiTargeting.Connectivity;

namespace LaunchDebug
{
    public class WpPowerToolsService : CcConnectionDeviceService
    {
        public WpPowerToolsService(
            ConnectableDevice pDeviceToConnect) : base(
            pDeviceToConnect,
            new Guid("280D7035-D88D-47CF-A90C-2B19717B5C34"), 
            new Guid("C48B1B43-A833-41F7-9E76-47AC1513E7F6"))
        {
            Connect(string.Empty);
            ReadVersionInfo();
        }

        /// <summary>
        /// Deletes a file from the file system.
        /// </summary>
        /// <param name="pPathToDelete">Path to file that WpToolsSvc should delete.</param>
        public void SendCleanUpCommand(string pPathToDelete)
        {
            var res = SendServiceCommand("cleanup", new[] {pPathToDelete});
            if(res.Code!=0)
                throw new Win32Exception(res.Code);
        }

        public Version GetFileVersion(string pPathToGetVersion)
        {
            var res = SendServiceCommand("getVersion", new[] { pPathToGetVersion });
            switch (res.Code)
            {
                case 0:
                    break;
                case -2147024809:
                    throw new FileNotFoundException("File not found", pPathToGetVersion);
                default:
                    throw new Win32Exception(res.Code);
            }
               

            Version version;
            if (Version.TryParse(res.Lines[0], out version))
                return version;
            return null;
        }

        /// <summary>
        /// Executes wpwpr.exe in \DATA\SharedData\PhoneTools\PwTools\bin\ with the parameters passed.
        /// </summary>
        /// <param name="pArgumentsToWpWprExe">Parameters to pass to wpwpr.exe</param>
        public void StartWindowsPerformanceRecorder(string pArgumentsToWpWprExe)
        {
            SendServiceCommand("startWpr", new[] {pArgumentsToWpWprExe});
        }

        /// <summary>
        /// Not properly understood
        /// </summary>
        /// <param name="pArgumentsToExe"></param>
        public string StartWAppVerifier(string pArgumentsToExe)
        {
            var res=    SendServiceCommand("startAppVerifier", new[] { pArgumentsToExe });
            if(res.Code!=0)
                throw new Win32Exception(res.Code);

            var tmpFile = System.IO.Path.GetTempFileName();
            
            RetrieveFile(res.Lines[0], tmpFile);
            if (!File.Exists(tmpFile))
                throw new FileNotFoundException(res.Lines[0]);
            
            var text = File.ReadAllText(tmpFile);
            File.Delete(tmpFile);
            return text;

        }
        /// <summary>
        /// Not properly understood
        /// </summary>
        /// <param name="pArgumentsToExe"></param>
        public void AddMetaData()
        {
           var res =  SendServiceCommand("appMetadata", new string[] {  });
        }
    }
}
