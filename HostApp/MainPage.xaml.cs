using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Windows.ApplicationModel.Activation;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.Storage;
using Windows.Storage.Pickers;
using Windows.UI.Core;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=391641
using HostApp.ViewModel;

namespace HostApp
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        public static MainPage Current { get; private set; }


      
        public MainPage()
        {
            Current = this;
            this.InitializeComponent();
            SetFolderAttributes();
            this.NavigationCacheMode = NavigationCacheMode.Required;
            var context = new HostAppsView(@"d:\wpsystem\apps");
            var sb = new StringBuilder(1024);
            NativeAPI.GetCurrentDirectoryW(1024, sb);
            if(sb.ToString().ToUpper().StartsWith("D:\\"))
            {
                Frame.Navigate(typeof(MoveToPhone));
            } else
            if (Windows.Storage.AccessCache.StorageApplicationPermissions.FutureAccessList.ContainsItem("WPSYSTEM"))
            {
                DoStuff(context);
            }
            else
            {
                Frame.Navigate(typeof(Welcome));
                Frame.DataContext = context;
            }
            


            
        }

        private async void DoStuff(HostAppsView context)
        {
            context.AppsStorageFolder =
                await
                    (await
                        Windows.Storage.AccessCache.StorageApplicationPermissions.FutureAccessList.GetFolderAsync(
                            "WPSYSTEM")).GetFolderAsync("apps");
            Dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
            {
                Frame.Navigate(typeof (ChooseDestination));
                Frame.DataContext = context;
            });
        }

        private void SetFolderAttributes()
         {
             var attributes = (int)NativeAPI.GetFileAttributesW(@"d:\WPSystem");
             attributes &= (int)~0x6;
             NativeAPI.SetFileAttributesW(@"d:\WPSystem", (uint)attributes);
         }
 


        /// <summary>
        /// Invoked when this page is about to be displayed in a Frame.
        /// </summary>
        /// <param name="e">Event data that describes how this page was reached.
        /// This parameter is typically used to configure the page.</param>
        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            // TODO: Prepare page for display here.

            // TODO: If your application contains multiple pages, ensure that you are
            // handling the hardware Back button by registering for the
            // Windows.Phone.UI.Input.HardwareButtons.BackPressed event.
            // If you are using the NavigationHelper provided by some templates,
            // this event is handled for you.
        }

        public async void DoStuff()
        {
            var folder3 = await StorageFolder.GetFolderFromPathAsync(@"d:\");
            var folder4 = (await folder3.GetFoldersAsync()).First(pX => pX.Name.ToLower().Equals("wpsystem"));
            var folder5 = (await folder4.GetFoldersAsync()).First(pX => pX.Name.ToLower().Equals("apps"));
            var folder6 = (await folder5.GetFoldersAsync()).First(pX => pX.Name.ToLower().Equals("{178AC8A1-6519-4A0B-960C-038393741E96}".ToLower()));
            var folder7 = (await folder6.GetFoldersAsync()).First(pX => pX.Name.ToLower().Equals("install"));
            var files = (await folder7.GetFilesAsync()).ToArray();

        }
     
    }
}
