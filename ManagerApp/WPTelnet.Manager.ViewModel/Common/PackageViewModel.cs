using Windows.ApplicationModel;

namespace WPTelnet.Manager.ViewModel.Common
{
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