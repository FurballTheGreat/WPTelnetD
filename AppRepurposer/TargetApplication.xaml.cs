using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;
using AppRepurposer.Common;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;

namespace AppRepurposer
{
    public partial class TargetApplication : PhoneApplicationPage
    {
        public TargetApplication()
        {
            InitializeComponent();
            // Set the data context of the listbox control to the sample data
            
        }

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            base.OnNavigatedTo(e);
            DataContext = App.ViewModel;
 
            App.ViewModel.TargetApp += pApp => App.RootFrame.Navigate(new Uri("/ApplyPatch.xaml", UriKind.RelativeOrAbsolute));
            foreach(var app in HostableApps.Apps)
                app.UpdatePresent();
        }
    }
}