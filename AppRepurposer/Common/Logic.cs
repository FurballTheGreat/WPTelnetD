using System;
using System.Collections.Generic;
using System.Threading;
using System.Threading.Tasks;
using Windows.ApplicationModel;
using Windows.Data.Xml.Dom;
using Windows.Storage;
using TelnetDS81;

namespace AppRepurposer.Common
{
    public static class Logic
    {
        public static void Execute(string pCommand)
        {
            DevProgramReg.TelnetExecuteCommand(pCommand);
        }

        private static  async Task<StorageFolder> GetFolder()
        {
            var folder = await Windows.Storage.AccessCache.StorageApplicationPermissions.FutureAccessList.GetFolderAsync("WPSYSTEM");

            return folder;

        }
        public static StorageFolder WpSystem
        {
            get
            {
                if (!Windows.Storage.AccessCache.StorageApplicationPermissions.FutureAccessList.ContainsItem("WPSYSTEM"))
                    return null;

                try
                {
                    var task = GetFolder();
                    Task.WaitAny(new Task[] {task});
                    return task.Result;
                }
                catch (Exception)
                {
                    return null;
                }
            }

            set
            {
                Windows.Storage.AccessCache.StorageApplicationPermissions.FutureAccessList.AddOrReplace(
               "WPSYSTEM", value);
            }
        }

        public static async Task<bool> DoXapReplace(string pDestGuid, string pSourceXap, Action<string> pNotify )
        {
            pNotify("Removing original files...");
            Execute(@"move d:\wpsystem\apps d:\wpsystem\appss");

            Execute(string.Format(@"del D:\wpsystem\appss\{0}\*.*", "{" + pDestGuid + "}"));

            Execute(@"move d:\wpsystem\appss d:\wpsystem\Apps");
           pNotify("Locating xap...");

           var storeFile = await Package.Current.InstalledLocation.GetFileAsync(pSourceXap);
           
            try
            {
                pNotify("Locating destination...");

                var rootDestFolder = await (await GetFolder()).GetFolderAsync(@"Apps\{" + pDestGuid+"}");

                pNotify("Creating folders...");

                await rootDestFolder.CreateFolderAsync("ni");
                var installFolder = await rootDestFolder.CreateFolderAsync("install");
              pNotify("Installing XAP...");

                await ZipHelper.UnzipFromStorage(storeFile, installFolder, new []{"wmappmnifest.xml"});
                return true;
            }
            catch (Exception e)
            {
                pNotify( "ERROR: " + e.Message);

            }
            return false;
        }

        

        public static async Task<bool> DoAppxReplace(Action<string> pNotify,string pDestFolder, string pSourceFolder, string[] pFiles)
        {
            try
            {
                pNotify("Removing original files...");
                Execute(@"move d:\wpsystem\apps d:\wpsystem\appss");
                foreach (var file in pFiles)
                    Execute(string.Format(@"del D:\wpsystem\appss\WindowsApps\{0}\{1}", pDestFolder, file));

                Execute(@"move d:\wpsystem\appss d:\wpsystem\Apps");

                pNotify("Preparing to install replacement files...");
                var apps = await (await GetFolder()).GetFolderAsync("apps");
                var winapps = await apps.GetFolderAsync("WindowsApps");
                var appFolder =
                    await winapps.GetFolderAsync(pDestFolder);
                pNotify("Installing replacement files...");
                foreach (var file in pFiles)
                {
                    var newFile = await Package.Current.InstalledLocation.GetFileAsync(pSourceFolder + @"\" + file);
                    await newFile.CopyAsync(appFolder);
                }
                pNotify("Done...");
            }
            catch (Exception e)
            {
                pNotify("ERROR: " + e.Message);
                return false;
            }
            return true;
        }

        public async static Task<bool> AppxExists(string pAppxDirName)
        {
            var apps = await (await GetFolder()).GetFolderAsync(@"apps\WindowsApps");
            try
            {
                var folder = await apps.GetFolderAsync(pAppxDirName);
                if (folder != null)
                    return true;
            }
            // ReSharper disable once EmptyGeneralCatchClause
            catch
            {

            }
            return false;
        }

        public async static Task<bool> XapExists(Guid pXap)
        {
            var apps = await (await GetFolder()).GetFolderAsync("apps");
            try
            {
                var folder =await apps.GetFolderAsync("{"+pXap.ToString()+"}");
                if (folder != null)
                    return true;
            }
// ReSharper disable once EmptyGeneralCatchClause
            catch
            {
                
            }
            return false;
        }
    }
}
