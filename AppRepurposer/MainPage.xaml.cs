using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;
using Windows.ApplicationModel;
using Windows.ApplicationModel.Activation;
using Windows.Storage;
using Windows.Storage.Pickers;
using AppRepurposer.Common;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;
using AppRepurposer.Resources;

namespace AppRepurposer
{
    public partial class MainPage : PhoneApplicationPage
    {
        // Constructor
        public MainPage()
        {
            Current = this;
            InitializeComponent();


           
            // Sample code to localize the ApplicationBar
            //BuildLocalizedApplicationBar();

          //  var et = new ETWLogger.WPDIETLogger();
          //  et.LogError("kk",0);
        }

        public static MainPage Current { get; private set; }

        // Load data for the ViewModel Items
        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            var dir = Directory.GetCurrentDirectory().ToLower();
       
          
            if (dir.StartsWith(@"d:\")) 
                App.RootFrame.Navigate(new Uri("/InstalledOnSD.xaml", UriKind.RelativeOrAbsolute));
            else if(Logic.WpSystem!=null)
                App.RootFrame.Navigate(new Uri("/TargetApplication.xaml", UriKind.RelativeOrAbsolute));
   
        }

        // Sample code for building a localized ApplicationBar
        //private void BuildLocalizedApplicationBar()
        //{
        //    // Set the page's ApplicationBar to a new instance of ApplicationBar.
        //    ApplicationBar = new ApplicationBar();

        //    // Create a new button and set the text value to the localized string from AppResources.
        //    ApplicationBarIconButton appBarButton = new ApplicationBarIconButton(new Uri("/Assets/AppBar/appbar.add.rest.png", UriKind.Relative));
        //    appBarButton.Text = AppResources.AppBarButtonText;
        //    ApplicationBar.Buttons.Add(appBarButton);

        //    // Create a new menu item with the localized string from AppResources.
        //    ApplicationBarMenuItem appBarMenuItem = new ApplicationBarMenuItem(AppResources.AppBarMenuItemText);
        //    ApplicationBar.MenuItems.Add(appBarMenuItem);
        //}
 

       

        public async void ContinueFromPickFolder(FolderPickerContinuationEventArgs pPick)
        {
            if (pPick.Folder == null)
                return;

            if (pPick.Folder.Path.ToLower() != @"d:\wpsystem")
                return;

            Logic.WpSystem = pPick.Folder;
            App.RootFrame.Navigate(new Uri("/TargetApplication.xaml", UriKind.RelativeOrAbsolute));
            //DoReplace(
            //    folder,
            //    "Microsoft.Upprv_1.1.1211.12_arm__8wekyb3d8bbwe",
            //    "new",
            //    new[] { "DeviceUpdateUtils.dll", "wptd.exe" });
            //DoReplace(
            //                folder,
            //                "Microsoft.WPDiet_2.0.15086.130_arm__8wekyb3d8bbwe",
            //                "new",
            //                new[] { "etwlogger.dll", "wptd.exe" });


   //         Execute(@"move d:\wpsystem\apps d:\wpsystem\appss");
   //         Execute(@"del D:\wpsystem\appss\WindowsApps\Microsoft.WPDiet_2.0.15086.130_arm__8wekyb3d8bbwe\etwlogger.dll");
   //         Execute(@"move d:\wpsystem\appss d:\wpsystem\Apps");
   //       //  Execute(@"del D:\wpsystem\appss\WindowsApps\Microsoft.WPDiet_2.0.15086.130_arm__8wekyb3d8bbwe\wpdiet.exe");
   //  //       Execute(@"del D:\wpsystem\appss\WindowsApps\68944423-66aa-41f2-ba9f-a95aa4b58f5a_1.0.0.6_arm__cxkkyw345tmjc\etwlogger.winmd");
   //  //       Execute(@"del D:\wpsystem\appss\WindowsApps\68944423-66aa-41f2-ba9f-a95aa4b58f5a_1.0.0.6_arm__cxkkyw345tmjc\etwlogger.dll");
      
   //   //     Execute(@"del D:\wpsystem\appss\WindowsApps\Microsoft.WPDiet_2.0.15086.130_arm__8wekyb3d8bbwe\etwlogger.winmd");
            
            
   //       var apps = await folder.GetFolderAsync("apps");
           
   //         var winapps = await apps.GetFolderAsync("WindowsApps");
   //         var appnames = new List<string>();
   //         foreach (StorageFolder folder2 in await winapps.GetFoldersAsync())
   //         {
   //             appnames.Add(folder2.Name);
   //         }
   //         var appFolder =
   //             await winapps.GetFolderAsync("Microsoft.WPDiet_2.0.15086.130_arm__8wekyb3d8bbwe"
   //         /*"68944423-66aa-41f2-ba9f-a95aa4b58f5a_1.0.0.6_arm__cxkkyw345tmjc"*/);
   //         var dll = await Package.Current.InstalledLocation.GetFileAsync(@"new\etwlogger.dll");
   //         var wptd = await Package.Current.InstalledLocation.GetFileAsync(@"wptd.exe");
   //   //     var winmd = await Package.Current.InstalledLocation.GetFileAsync(@"existing\etwlogger.winmd");
   //         await wptd.CopyAsync(appFolder);
   //         await dll.CopyAsync(appFolder);
   //    //   await winmd.CopyAsync(appFolder);
   ////         var exe = await Package.Current.InstalledLocation.GetFileAsync(@"existing\wpdiet.exe");
   //  //       await exe.CopyAsync(appFolder);
        }

        private void OnLocateWPSystem(object pSender, RoutedEventArgs pE)
        {
            
            var pick = new FolderPicker();
            pick.SettingsIdentifier = "wpsystem";
            pick.FileTypeFilter.Add("*");
            pick.PickFolderAndContinue();
        }
    }
}