using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;

namespace UXX.Windows
{
    public partial class OutputPanel : UserControl
    {
        private DispatcherTimer timer = null;
        private object msglocker = new object();
        private Queue<string> msgs = new Queue<string>();

        public OutputPanel()
        {
            InitializeComponent();
        }

        public void Clear()
        {
            Messager.Clear();
        }

        public bool AutoWarp
        {
            get
            {
                return Messager.TextWrapping == TextWrapping.Wrap;
            }
            set
            {
                Messager.TextWrapping = value ? TextWrapping.Wrap : TextWrapping.NoWrap;
            }
        }

        private static long counter = 0;

        public void AddMessage(string msg)
        {
            System.Threading.Interlocked.Increment(ref counter);
            lock(msglocker)
            {
                msgs.Enqueue(msg);
            }
            System.Threading.Interlocked.Decrement(ref counter);

            //UXXPackage.singleton.JoinableTaskFactory.RunAsync(async delegate
            //{
            //    await UXXPackage.singleton.JoinableTaskFactory.SwitchToMainThreadAsync();

            //    Messager.AppendText(msg + Environment.NewLine);
            //    Messager.ScrollToEnd();

            //    System.Threading.Interlocked.Decrement(ref counter);
            //});
        }

        private void UserControl_Loaded(object sender, RoutedEventArgs e)
        {
            timer = new DispatcherTimer(DispatcherPriority.Render) { Interval = TimeSpan.FromMilliseconds(30) };
            timer.Tick += Timer_Tick;
            timer.Start();
        }

        private void Timer_Tick(object sender, EventArgs e)
        {
            Queue<string> localmsgs = new Queue<string>();
            lock (msglocker)
            {
                while(msgs.Count>0 && localmsgs.Count<6)
                {
                    localmsgs.Enqueue(msgs.Dequeue());
                }   
            }

            if (localmsgs.Count > 0)
            {
                Messager.Dispatcher.BeginInvoke(DispatcherPriority.Render, new Action(() =>
                {
                    while (localmsgs.Count > 0)
                    {
                        Messager.AppendText(localmsgs.Dequeue());
                    }
                    Messager.ScrollToEnd();
                }));
            }
        }

        private void UserControl_Unloaded(object sender, RoutedEventArgs e)
        {
            if (timer != null)
            {
                timer.Tick -= Timer_Tick;
                timer.Stop();
                timer = null;
            }
        }
    }

}
