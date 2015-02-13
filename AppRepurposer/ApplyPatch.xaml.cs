using System;
using System.Windows;
using Microsoft.Phone.Controls;

namespace AppRepurposer
{
    public partial class ApplyPatch : PhoneApplicationPage
    {
        public ApplyPatch()
        {
            InitializeComponent();
            DataContext = App.ViewModel;
        }

        private async void OnConfirm(object pSender, RoutedEventArgs pE)
        {
            Confirm.Visibility = Visibility.Collapsed;
            var result = await App.ViewModel.TargettedApp.App.Replace(pMessage => Deployment.Current.Dispatcher.BeginInvoke(
                () => StatusMessage.Text = pMessage));
            if (result)
            {
                App.RootFrame.Navigate(new Uri("/Done.xaml", UriKind.RelativeOrAbsolute));
            }
            else
            {
                Confirm.Visibility = Visibility.Visible;
            }
        }
    }
}