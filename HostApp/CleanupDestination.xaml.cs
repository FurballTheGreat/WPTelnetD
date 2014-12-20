using System.Text;
using Windows.ApplicationModel;
using Windows.Storage;
using Windows.UI.Core;
using CSUnvsAppDownloadExtractZip;
using HostApp.Common;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.Graphics.Display;
using Windows.UI.ViewManagement;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// The Basic Page item template is documented at http://go.microsoft.com/fwlink/?LinkID=390556
using HostApp.ViewModel;

namespace HostApp
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class CleanupDestination : Page
    {
        private NavigationHelper navigationHelper;
        private ObservableDictionary defaultViewModel = new ObservableDictionary();

        public CleanupDestination()
        {
            this.InitializeComponent();

            this.navigationHelper = new NavigationHelper(this);
            this.navigationHelper.LoadState += this.NavigationHelper_LoadState;
            this.navigationHelper.SaveState += this.NavigationHelper_SaveState;
        }

        /// <summary>
        /// Gets the <see cref="NavigationHelper"/> associated with this <see cref="Page"/>.
        /// </summary>
        public NavigationHelper NavigationHelper
        {
            get { return this.navigationHelper; }
        }

        /// <summary>
        /// Gets the view model for this <see cref="Page"/>.
        /// This can be changed to a strongly typed view model.
        /// </summary>
        public ObservableDictionary DefaultViewModel
        {
            get { return this.defaultViewModel; }
        }

        /// <summary>
        /// Populates the page with content passed during navigation.  Any saved state is also
        /// provided when recreating a page from a prior session.
        /// </summary>
        /// <param name="sender">
        /// The source of the event; typically <see cref="NavigationHelper"/>
        /// </param>
        /// <param name="e">Event data that provides both the navigation parameter passed to
        /// <see cref="Frame.Navigate(Type, Object)"/> when this page was initially requested and
        /// a dictionary of state preserved by this page during an earlier
        /// session.  The state will be null the first time a page is visited.</param>
        private void NavigationHelper_LoadState(object sender, LoadStateEventArgs e)
        {
        }

        /// <summary>
        /// Preserves state associated with this page in case the application is suspended or the
        /// page is discarded from the navigation cache.  Values must conform to the serialization
        /// requirements of <see cref="SuspensionManager.SessionState"/>.
        /// </summary>
        /// <param name="sender">The source of the event; typically <see cref="NavigationHelper"/></param>
        /// <param name="e">Event data that provides an empty dictionary to be populated with
        /// serializable state.</param>
        private void NavigationHelper_SaveState(object sender, SaveStateEventArgs e)
        {
        }

        #region NavigationHelper registration

        /// <summary>
        /// The methods provided in this section are simply used to allow
        /// NavigationHelper to respond to the page's navigation methods.
        /// <para>
        /// Page specific logic should be placed in event handlers for the  
        /// <see cref="NavigationHelper.LoadState"/>
        /// and <see cref="NavigationHelper.SaveState"/>.
        /// The navigation parameter is available in the LoadState method 
        /// in addition to page state preserved during an earlier session.
        /// </para>
        /// </summary>
        /// <param name="e">Provides data for navigation methods and event
        /// handlers that cannot cancel the navigation request.</param>
        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            this.navigationHelper.OnNavigatedTo(e);
        }

        protected override void OnNavigatedFrom(NavigationEventArgs e)
        {
            this.navigationHelper.OnNavigatedFrom(e);
        }

        #endregion

        private async void DoReplace()
        {
            NativeAPI.MoveFileW(@"d:\wpsystem\apps", @"d:\wpsystem\appss");
            var dsttree = @"d:\wpsystem\appss\{" + (DataContext as HostAppsView).SelectedDestinationApp +
                             "}";
            try
            {

                PathUtils.DeleteTree(dsttree);
            }
            finally
            {
                NativeAPI.MoveFileW(@"d:\wpsystem\appss", @"d:\wpsystem\Apps");
            }

            Dispatcher.RunAsync(CoreDispatcherPriority.Normal, () => Status.Text = "Locating xap...");
           
            var storeFile = await Package.Current.InstalledLocation.GetFileAsync("wptelnetd.xap");
            var cont = DataContext as HostAppsView;
            try
            {
                Dispatcher.RunAsync(CoreDispatcherPriority.Normal, () => Status.Text = "Locating destination...");
               
                var rootDestFolder = await cont.SelectedDestinationApp.GetFolder();
                
                Dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>Status.Text = "Creating folders...");
               
                await rootDestFolder.CreateFolderAsync("ni");
                var installFolder = await rootDestFolder.CreateFolderAsync("install");
                Dispatcher.RunAsync(CoreDispatcherPriority.Normal, () => Status.Text = "Installing XAP...");
               
                await ZipHelper.UnZipFileAsync(storeFile, installFolder);
                MainPage.Current.Frame.Navigate(typeof(DoneScreen));
                MainPage.Current.Frame.DataContext = DataContext;
            }
            catch (Exception e)
            {
                Dispatcher.RunAsync(CoreDispatcherPriority.Normal, () => Status.Text = "ERROR: " + e.Message);
                
            }
            Dispatcher.RunAsync(CoreDispatcherPriority.Normal, () => Nuke.IsEnabled = true);
        }
        private void OnNukeClick(object pSender, RoutedEventArgs pE)
        {
            Nuke.IsEnabled = false;
            Status.Text = "Cleaning destination app...";
            DoReplace();
            
        }
    }
}
