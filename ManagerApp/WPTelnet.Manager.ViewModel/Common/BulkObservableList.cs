using System.Collections.Generic;
using System.Collections.Specialized;

namespace WPTelnet.Manager.ViewModel.Common
{
    public class BulkObservableList<T> : List<T>, INotifyCollectionChanged

    {
        public event NotifyCollectionChangedEventHandler CollectionChanged;

        public void ReplaceAll(IEnumerable<T> pItems)
        {
            Clear();
            AddRange(pItems);
            if (CollectionChanged != null)
                CollectionChanged(this, new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Reset,null));

        }
    }
}
