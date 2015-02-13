using System;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Windows.Interop;
using AppRepurposer.Common;
using AppRepurposer.Resources;

namespace AppRepurposer.ViewModels
{
    public class MainViewModel : INotifyPropertyChanged
    {
        private ObservableCollection<HostableAppViewModel> _items;

        public MainViewModel()
        {
            this.Items = new ObservableCollection<HostableAppViewModel>();
            RebuildItems();
            TargetAppCommand = new BaseCommand(pApp=>OnTargetApp(pApp as HostableAppViewModel));
        }

        private void OnTargetApp(HostableAppViewModel pHostableAppViewModel)
        {
            TargettedApp = pHostableAppViewModel;
            if (TargetApp != null)
                TargetApp(pHostableAppViewModel);

        }

        public HostableAppViewModel TargettedApp { get; private set; }

        public event Action<HostableAppViewModel> TargetApp;

        private void RebuildItems()
        {
            Items.Clear();
            foreach (var app in HostableApps.Apps)
            {
                //if(app.Present)
                    Items.Add(new HostableAppViewModel(app));
            }
        }

        /// <summary>
        /// A collection for HostableAppViewModel objects.
        /// </summary>
        public ObservableCollection<HostableAppViewModel> Items
        {
            get { return _items; }
            private set { _items = value; }
        }


        public BaseCommand TargetAppCommand { get; private set; }

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