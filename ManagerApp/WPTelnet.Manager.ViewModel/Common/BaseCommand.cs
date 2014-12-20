using System;
using System.Windows;
using System.Windows.Input;
using System.Windows.Threading;

namespace WPTelnet.Manager.ViewModel.Common
{
    public class BaseCommand : ICommand
    {
        private readonly Dispatcher _dispatcher;
        private readonly Func<object, bool> _canExecute;
        private readonly Action<object> _execute;
        

        public BaseCommand(Action<object> pExecute, Func<object, bool> pCanExecute)
        {
            _execute = pExecute;
            _canExecute = pCanExecute;
            _dispatcher = Deployment.Current.Dispatcher;
        }


        public BaseCommand(Action pExecute, Func<bool> pCanExecute)
        {
            _execute = pObj=> pExecute();
            _canExecute = pObj=> pCanExecute();
            _dispatcher = Deployment.Current.Dispatcher;
        }

        public BaseCommand(Action<object> pExecute)
        {
            _execute = pExecute;
            _canExecute = pObj => true;
        }


        public BaseCommand(Action pExecute)
        {
            _execute = pObj => pExecute();
            _canExecute = pObj => true;
        }

        void ICommand.Execute(object pParameter)
        {
            _execute(pParameter);
        }

        bool ICommand.CanExecute(object pParameter)
        {
            return _canExecute(pParameter);
        }

        event EventHandler ICommand.CanExecuteChanged
        {
            add { CanExecuteChanged += value; }

            remove { CanExecuteChanged -= value; }
        }

        public void IndicateChange()
        {
            if (_dispatcher != Deployment.Current.Dispatcher)
                _dispatcher.BeginInvoke(IndicateChange);
            else
            {
                if (CanExecuteChanged != null)
                    CanExecuteChanged(this, new EventArgs());
            }
        }

        private event EventHandler CanExecuteChanged;
    }
}