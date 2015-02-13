using System;
using System.ComponentModel;
using System.Windows;
using System.Windows.Threading;
using AppRepurposer.Common;

namespace AppRepurposer.ViewModels
{
    public class HostableAppViewModel : INotifyPropertyChanged
    {
        private readonly IHostableApp _app;


        public HostableAppViewModel(IHostableApp pApp)
        {
            _app = pApp;
            _app.PresentChanged += () => Deployment.Current.Dispatcher.BeginInvoke(() =>
            {
                NotifyPropertyChanged("Available");
                NotifyPropertyChanged("Unavailable");
            });
        }

        public string Name
        {
            get { return _app.Name; }
        }



        public bool Available
        {
            get { return _app.Present; } 
        }

        public bool Unavailable
        {
            get { return !_app.Present; }
        }

        public IHostableApp App { get { return _app; } }

        public event PropertyChangedEventHandler PropertyChanged;
        private void NotifyPropertyChanged(String propertyName)
        {
            PropertyChangedEventHandler handler = PropertyChanged;
            if (null != handler)
            {
                handler(this, new PropertyChangedEventArgs(propertyName));
            }
        }
    }
}