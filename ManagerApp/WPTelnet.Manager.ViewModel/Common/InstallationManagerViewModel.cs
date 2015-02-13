using System;
using System.Collections;
using System.Collections.Generic;
using Windows.ApplicationModel;
using Windows.Phone.Management.Deployment;

namespace WPTelnet.Manager.ViewModel.Common
{
    public class InstallationManagerViewModel : BaseView, IEnumerable<PackageViewModel>
    {
        private async void DoAdd()
        {
            try
            {
                await
                    PhoneInternal.CorePlat.Diagnostics.DiagnosticsController.CopyFileAsync(
                        @"c:\windows\system32\kernelbase.dll", @"c:\kernelbase.dll");
            }
            catch (Exception e)
            {
                Dispatch(() =>
                {
                    Status = e.Message;
                });
            }
        }

        public InstallationManagerViewModel()
        {
       
            InstallCommand = new BaseCommand(() =>
            {
                try
                {
                    DoAdd();
                }
                catch (Exception e)
                {
                    Status = e.Message;
                }
                
            });
            try
            {
                foreach (var package in InstallationManager.FindPackages())
                {
                    _packages.Add(new PackageViewModel((Package)package));
                }


            }
            catch (Exception e)
            {
                Status = e.Message;
            }
        }

        private List<PackageViewModel> _packages = new List<PackageViewModel>();
        private string _status;

        public IEnumerator<PackageViewModel> GetEnumerator()
        {
            return _packages.GetEnumerator();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }

        public string Status
        {
            get { return _status; }
            set
            {
                _status = value;
                OnPropertyChanged("Status");
            }
        }

        public BaseCommand InstallCommand { get; set; }
    }
}