using Microsoft.Phone.Controls;

using WPTelnet.Manager.ViewModel.Daemon;

namespace WPTelnetManager
{
    public partial class MainPage : PhoneApplicationPage
    {
       
        // Constructor
        public MainPage()
        {
            InitializeComponent();
            DataContext = new ControllerView();
        }

        
    }
}