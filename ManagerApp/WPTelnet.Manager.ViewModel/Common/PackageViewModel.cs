using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using Windows.ApplicationModel;
using Windows.Phone.Networking.NetworkOperators;

namespace WPTelnet.Manager.ViewModel.Common
{
    public class SmsInterceptViewModel : BaseView
    {
        private string _status;

        public SmsInterceptViewModel()
        {
            
            Messages = new ObservableCollection<SmsMessageViewModel>();
            try
            {
             
                Windows.Phone.PersonalInformation.Provisioning.MessagePartnerProvisioningManager.ImportSmsToSystemAsync(
                    
                    false, true, "Hi cunt", "+353343234432",new List<string>{"0877777777"}.AsReadOnly(),
                    DateTimeOffset.Now.AddDays(-5));
                foreach (var message in SmsInterceptor.GetQueuedMessages())
                {
                    Messages.Add(new SmsMessageViewModel(message as InterceptedSmsMessage));
                }
            }
            catch (Exception e)
            {
                Status = e.Message;
            }

            try
            {
                SmsInterceptor.SmsReceived += SmsInterceptor_SmsReceived;

            }
            catch (Exception e)
            {
                Status = e.Message;
            }
        }

        void SmsInterceptor_SmsReceived(object sender, object e)
        {
            Dispatch(() =>
            {
                Status = "Message Received";
                try
                {
                    var message = e as InterceptedSmsMessage;
                    Messages.Add(new SmsMessageViewModel(message));
                }
                catch (Exception ee)
                {
                    Status = ee.Message;
                }
            });
            
           
        }

        public ObservableCollection<SmsMessageViewModel> Messages { get; private set; }

        public string Status
        {
            get { return _status; }
            set
            {
                _status = value;
                OnPropertyChanged("Status");
            }
        }
    }

    public class SmsMessageViewModel
    {
        private readonly InterceptedSmsMessage _message;

        public SmsMessageViewModel(InterceptedSmsMessage pMessage)
        {
            _message = pMessage;
        }

        public string Body
        {
            get { return _message.Body; }
        }

        public string ProductId
        {
            get { return _message.ProductId; }
        }

        public string SenderPhoneNumber
        {
            get { return _message.SenderPhoneNumber; }
        }

        public string SmsPrefix
        {
            get { return _message.SmsPrefix; }
        }

        public string TimeStamp
        {
            get { return _message.TimeStamp.ToString(); }
        }
    }

    public class PackageViewModel : BaseView
    {
        private readonly Package _package;

        public PackageViewModel(Package pPackage)
        {
            _package = pPackage;
       
        }

        public string Name
        {
            get { return _package.Id.Name; }
        }

        public string InstallDate
        {
            get { return _package.InstallDate.ToString(); }
        }

        public string Path
        {
            get { return _package.InstalledLocation.FolderRelativeId+" -"; }
        }
    }
}