using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;

namespace AppRepurposer
{
    public partial class Done : PhoneApplicationPage
    {
        public Done()
        {
            InitializeComponent();
        }

        private void OnClose(object pSender, RoutedEventArgs pE)
        {
            Application.Current.Terminate();
        }
    }
}