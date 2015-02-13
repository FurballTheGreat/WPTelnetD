using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using System.Xml;
using Windows.Data.Xml.Dom;
using Windows.Storage;
using Windows.UI.Core;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using HostApp.Annotations;
using HostApp.Common;

namespace HostApp.ViewModel
{
    public class HostableApp
    {
        public string Name { get; set; }
        public Guid Guid { get; set; }

        public static HostableApp[] Apps = new HostableApp[]
        {
            new HostableApp
            {
                Name = "Extras & Info", 
                Guid = new Guid("2377fe1b-c10f-47da-92f3-fc517345a3c0")
            },
            new HostableApp
            {
                Name = "Preview For Developers", 
                Guid = new Guid("178ac8a1-6519-4a0b-960c-038393741e96")
            }, 
             new HostableApp
            {
                Name = "Glance Background", 
                Guid = new Guid("7c3378ff-2844-480b-b2e2-783537201b84")
            },
            new HostableApp
            {
                Name = "HTC CSDDiag", 
                Guid = new Guid("f81239cf-2e0f-4fc1-9141-a66e080b83f9")
            },
            new HostableApp
            {
                Name = "DiagnosticTool_Sprint", 
                Guid = new Guid("e69a2877-59b8-43ed-898d-554fbc4b8b2b")
            },
            new HostableApp
            {
                Name = "HTC Connection Setup",
                Guid = new Guid("5edbdbbc-2ab2-df11-8a2f-00237de2db9e")
            },
            new HostableApp
            {
                Name = "Samsung Diagnosis",
                Guid = new Guid("07a20ad9-a4f9-4de3-855f-dcda8c8cab39")
            },
        };

        public static string GetAppName(Guid pApp)
        {
            var item = Apps.FirstOrDefault(pX => pX.Guid == pApp);
            return item ==null? pApp.ToString() : item.Name;
        }

        public static bool GetIsSupported(Guid pApp)
        {
            var item = Apps.FirstOrDefault(pX => pX.Guid == pApp);
            return item != null;
        }
    }

    public class HostApp : INotifyPropertyChanged 
    {
        private readonly HostAppsView _view;
        private readonly Guid _guid;
        private readonly string _path;
        private XmlDocument _manifest;
        private string _name;


        

        public HostApp(HostAppsView pView,  Guid pGuid, string pPath)
        {
            _view = pView;
            _guid = pGuid;
            _path = pPath;

      //      pView.AppsStorageFolderChanged += LoadInfo;
            Name = HostableApp.GetAppName(pGuid);
            IsSupported = HostableApp.GetIsSupported(pGuid);
        }

        public bool IsSupported { get; set; }

        private async void LoadInfo()
        {
            GetManifest();
        }

        public override string ToString()
        {
            return _guid.ToString();
        }

        public async Task<StorageFolder> GetFolder()
        {
            return await _view.AppsStorageFolder.GetFolderAsync("{" + ToString() + "}");
        }

        public async void GetManifest()
        {
            try
            {
                var folder = await GetFolder();
                var installFolder = await folder.GetFolderAsync("install");
                Manifest = await XmlDocument.LoadFromFileAsync(await installFolder.GetFileAsync("WMAppManifest.xml"));
                


            }
            catch (Exception e)
            {
                
            }

        }

        public XmlDocument Manifest
        {
            get { return _manifest; }
            set
            {
                _manifest = value;
               
             //   LoadInfoFromManifest();
            }
        }

        private void LoadInfoFromManifest()
        {
            Name =Manifest.DocumentElement.SelectSingleNodeNS("//App","xmlns='http://schemas.microsoft.com/windowsphone/2012/deployment'").SelectSingleNode("@Title").NodeValue.ToString();
           
        }

        public string Name
        {
            get { return _name; }
            set
            {
                _name = value;
                OnPropertyChanged("Name");
            }
        }

        public event PropertyChangedEventHandler PropertyChanged;

        [NotifyPropertyChangedInvocator]
        protected virtual void OnPropertyChanged([CallerMemberName] string pPropertyName = null)
        {
            PropertyChangedEventHandler handler = PropertyChanged;
            if (handler != null) handler(this, new PropertyChangedEventArgs(pPropertyName));
        }
    }

    public class PathUtils
    {
        public static void DeleteTree(string pBasePath)
        {
            IntPtr INVALID_HANDLE_VALUE = new IntPtr(-1);
            NativeAPI.WIN32_FIND_DATA find;
            IntPtr result = NativeAPI.FindFirstFileW(pBasePath + @"\*.*", out find);
            var cont = result != INVALID_HANDLE_VALUE;
            while (cont)
            {
                if ((find.dwFileAttributes & 0x00000010) != 0)
                {
                    if (find.cFileName == "." || find.cFileName == "..")
                    {
                        cont = NativeAPI.FindNextFileW(result, out find);
                        continue;
                    }
                    DeleteTree(pBasePath + "\\" + find.cFileName);
                    NativeAPI.RemoveDirectoryW(pBasePath + "\\" + find.cFileName);

                }
                else
                {
                    NativeAPI.DeleteFileW(pBasePath + "\\" + find.cFileName);
                }
                
                cont = NativeAPI.FindNextFileW(result, out find);
            }
            if (result != INVALID_HANDLE_VALUE)
                NativeAPI.FindClose(result);
        }
    }

    public class HostAppsView
    {
        private ObservableCollection<HostApp> _apps;
        private HostApp _selectedApp;
        private HostApp _selectedDestination;
        private HostApp _selectedHostApp;
        private StorageFolder _appsStorageFolder;

        public HostAppsView(string pBasePath)
        {
            ShowManifestCommand = new RelayCommand(pHostApp=>ShowManifest(pHostApp as HostApp));
            DestinationApps = new ObservableCollection<HostApp>();
            _apps = new ObservableCollection<HostApp>();
            IntPtr INVALID_HANDLE_VALUE = new IntPtr(-1);
            NativeAPI.WIN32_FIND_DATA find;
            IntPtr result = NativeAPI.FindFirstFileW(pBasePath + @"\*.*", out find);
            var cont = result != INVALID_HANDLE_VALUE;
            while (cont)
            {
                var guid = Guid.Empty;
                if (Guid.TryParse(find.cFileName, out guid))
                {
                    _apps.Add(new HostApp(this,guid, pBasePath+"\\"+find.cFileName));

                }
             
               cont = NativeAPI.FindNextFileW(result, out find);
            }
           foreach(var apps in _apps)
               if(apps.IsSupported)
               DestinationApps.Add(apps);
            if (result != INVALID_HANDLE_VALUE)
                NativeAPI.FindClose(result);

        }

        private void ShowManifest(HostApp pHostApp)
        {
            SelectedHostApp = pHostApp;
            MainPage.Current.Frame.Navigate(typeof(ShowManifest));
            MainPage.Current.Frame.DataContext = this;
            pHostApp.GetManifest();

        }

        public HostApp SelectedHostApp
        {
            get { return _selectedHostApp; }
            set
            {
                _selectedHostApp = value;
                
            }
        }


        public ICommand ShowManifestCommand { get; set; }
     


        public ObservableCollection<HostApp> Apps
        {
            get { return _apps; }
            private set { _apps = value; }
        }

        public ObservableCollection<HostApp> DestinationApps { get; set; }

        public HostApp SelectedApp
        {
            get { return _selectedApp; }
            set
            {
                DestinationApps.Clear();
                foreach(var app in Apps)
                    if(app!=value)
                        DestinationApps.Add(app);
                _selectedApp = value;
            }
        }

        public HostApp SelectedDestinationApp
        {
            get { return _selectedDestination; }
            set { _selectedDestination = value; }
        }

        public StorageFolder AppsStorageFolder
        {
            get { return _appsStorageFolder; }
            set
            {
                _appsStorageFolder = value;
                if (AppsStorageFolderChanged != null)
                    AppsStorageFolderChanged();
            }
        }

        public event Action AppsStorageFolderChanged;

        public async Task<string> CopyFiles(StorageFolder pFolder)
        {
            var source= await pFolder.GetFolderAsync("{" + SelectedApp.ToString() + "}");
            var destination= await pFolder.GetFolderAsync("{" + SelectedDestinationApp.ToString() + "}");;

            if (source == null || destination == null)
                return "Source or destination not found";

            if ((await destination.GetFilesAsync()).Any() || (await destination.GetFoldersAsync()).Any())
                return "Destination not empty";

            await CopyFiles(source, destination);

            return null;
        }

        private async Task CopyFiles(StorageFolder pSource, StorageFolder pDestination)
        {
            foreach (var file in await pSource.GetFilesAsync())
                file.CopyAsync(pDestination);

            foreach (var folder in await pSource.GetFoldersAsync())
            {
                var newFolder = await pDestination.CreateFolderAsync(folder.Name);
                await CopyFiles(folder, newFolder);
            }
        }
    }
}
