using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Collections.Specialized;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Data;
using System.Windows.Threading;

namespace UXX.Windows
{
    public class CustomCollectionView<T> : ListCollectionView
    {
        private readonly DispatcherTimer _timerRefreshCalculate = new DispatcherTimer();
        private readonly DispatcherTimer _timerRefreshUI = new DispatcherTimer();
        private readonly DispatcherTimer _timerRefreshCurrentItem = new DispatcherTimer();
        private bool _isRefreshingCalculate = false;
        private object _oldSelectedItem = null;

        public CustomCollectionView(IList list)
            : base(list)
        {
            _timerRefreshUI.Interval = new TimeSpan(0, 0, 0, 0, 300);
            _timerRefreshCurrentItem.Interval = new TimeSpan(0, 0, 0, 0, 500);
            _timerRefreshCalculate.Interval = new TimeSpan(0, 0, 0, 0, 200);
        }

        #region Override Method

        protected override void OnPropertyChanged(PropertyChangedEventArgs e)
        {
            if (_isRefreshingCalculate)
            {
                return;
            }

            base.OnPropertyChanged(e);
        }

        protected override void OnCollectionChanged(NotifyCollectionChangedEventArgs args)
        {
            if (_isRefreshingCalculate)
            {
                return;
            }

            base.OnCollectionChanged(args);
        }

        protected override void OnCurrentChanged()
        {
            if (_isRefreshingCalculate)
            {
                return;
            }

            base.OnCurrentChanged();
        }

        protected override void RefreshOverride()
        {
            CancelAllRefreshRequest();

            StartRefresh();
        }

        #endregion


        #region Public Method

        public void CancelAllRefreshRequest()
        {
            _timerRefreshCurrentItem.Stop();
            _timerRefreshCurrentItem.Tick -= TimerCurrentItem;

            _timerRefreshUI.Stop();
            _timerRefreshUI.Tick -= TimerUI;

            _timerRefreshCalculate.Stop();
            _timerRefreshCalculate.Tick -= TimerCalculate;

            if (null != this.CurrentItem)
            {
                _oldSelectedItem = this.CurrentItem;
            }

            SetCurrent(null, -1);
        }

        #endregion


        #region Private Method

        private void StartRefresh()
        {
            _timerRefreshCurrentItem.Stop();
            _timerRefreshCurrentItem.Tick -= TimerCurrentItem;

            _timerRefreshUI.Stop();
            _timerRefreshUI.Tick -= TimerUI;

            _timerRefreshCalculate.Stop();
            _timerRefreshCalculate.Tick -= TimerCalculate;

            _isRefreshingCalculate = true;

            _timerRefreshCalculate.Tick += TimerCalculate;
            _timerRefreshCalculate.Start();
        }

        private void RefreshCalculate(CancellationToken? token)
        {
            _timerRefreshCalculate.Tick -= TimerCalculate;

            if (null != token && null != token.Value)
            {
                token.Value.ThrowIfCancellationRequested();
            }

            _isRefreshingCalculate = true;

            base.RefreshOverride();

            _isRefreshingCalculate = false;

            if (null != token && null != token.Value)
            {
                token.Value.ThrowIfCancellationRequested();
            }
        }

        private void RefreshUI()
        {
            try
            {
                _timerRefreshUI.Tick -= TimerUI;

                base.OnCollectionChanged(new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Reset));

                _timerRefreshCurrentItem.Tick -= TimerCurrentItem;
                _timerRefreshCurrentItem.Tick += TimerCurrentItem;
                _timerRefreshCurrentItem.Start();
            }
            catch (OperationCanceledException)
            {
                return;
            }
        }

        private void RefreshCurrentItem()
        {
            _timerRefreshCurrentItem.Tick -= TimerCurrentItem;

            if (null == this.InternalList || this.InternalList.Count <= 0)
            {
                return;
            }

            if (null != _oldSelectedItem)
            {
                var index = this.InternalList.IndexOf(_oldSelectedItem);
                if (index != -1)
                {
                    SetCurrent(_oldSelectedItem, index);
                }
                else
                {
                    SetCurrent(this.InternalList[0], 0);
                }
            }
            else
            {
                SetCurrent(this.InternalList[0], 0);
            }

            base.OnCurrentChanged();

            this.OnPropertyChanged("IsCurrentAfterLast");
            this.OnPropertyChanged("IsCurrentBeforeFirst");
            this.OnPropertyChanged("CurrentPosition");
            this.OnPropertyChanged("CurrentItem");
        }

        private void TimerCalculate(object sender, EventArgs e)
        {
            _timerRefreshCurrentItem.Stop();
            _timerRefreshCurrentItem.Tick -= TimerCurrentItem;

            _timerRefreshUI.Stop();
            _timerRefreshUI.Tick -= TimerUI;

            try
            {
                RefreshCalculate(null);
            }
            catch (OperationCanceledException)
            {
            }

            _timerRefreshUI.Interval = new TimeSpan(0, 0, 0, 0, 50 + Math.Min((int)(this.InternalCount / 80), 300));
            _timerRefreshUI.Tick += TimerUI;
            _timerRefreshUI.Start();
        }

        private void TimerUI(object sender, EventArgs e)
        {
            RefreshUI();
        }

        private void TimerCurrentItem(object sender, EventArgs e)
        {
            RefreshCurrentItem();
        }

        private void OnPropertyChanged(string propertyName)
        {
            OnPropertyChanged(new PropertyChangedEventArgs(propertyName));
        }

        #endregion
    }

    public class CustomCollection<T> : ObservableCollection<T>, ICollectionViewFactory
    {
        public CustomCollection()
        {
        }

        public CustomCollection(List<T> list)
            : base(list)
        {
        }

        public CustomCollection(IEnumerable<T> collection)
            : base(collection)
        {
        }

        public ICollectionView CreateView()
        {
            return new CustomCollectionView<T>(this);
        }
    }
}
