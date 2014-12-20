using System;
using System.ComponentModel;
using System.Windows;

namespace WPTelnet.Manager.ViewModel.Common
{
    public class BaseView : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;

        public void OnPropertyChanged(string pName)
        {
            if (PropertyChanged != null)
                PropertyChanged(this, new PropertyChangedEventArgs(pName));
        }

        protected void Dispatch(Action pAction)
        {
            Deployment.Current.Dispatcher.BeginInvoke(pAction);
        }
    }
}