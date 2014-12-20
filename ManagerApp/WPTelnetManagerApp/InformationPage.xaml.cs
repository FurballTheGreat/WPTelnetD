using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;
using WPTelnet.Manager.ViewModel.Daemon;

namespace WPTelnetManager
{
    public partial class InformationPage : PhoneApplicationPage
    {
        public InformationPage()
        {
            InitializeComponent();
            DataContext = new ControllerView();
        }
    }
}