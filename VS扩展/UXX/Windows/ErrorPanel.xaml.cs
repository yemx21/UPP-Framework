using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.ComponentModel;
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
    public enum CompileIntellisenseKinds
    {
        Invalid = 0,
        Error = 1,
        Tip = 2,
        Warning = 3,
        Output = 4,
    }

    public class CompileIntellisense : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;
        protected void OnPropertyChanged(string propertyName)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        private string _description;

        public string Description
        {
            get { return _description; }
            set
            {
                if (_description != value)
                {
                    _description = value;
                    OnPropertyChanged("Description");
                    _dessol = _description + " -> " + _solution;
                    OnPropertyChanged("DescriptionAndSolution");
                }
            }
        }

        private string _solution;

        public string Solution
        {
            get { return _solution; }
            set
            {
                if (_solution != value)
                {
                    _solution = value;
                    OnPropertyChanged("Solution");
                    _dessol = _description + " -> " + _solution;
                    OnPropertyChanged("DescriptionAndSolution");
                }
            }
        }

        private string _dessol;

        public string DescriptionAndSolution
        {
            get { return _dessol; }
            set
            {
                if (_dessol != value)
                {
                    _dessol = value;
                    OnPropertyChanged("DescriptionAndSolution");
                }
            }
        }


        private CompileIntellisenseKinds _kind;

        public CompileIntellisenseKinds Kind
        {
            get { return _kind; }
            set
            {
                if (_kind != value)
                {
                    _kind = value;
                    OnPropertyChanged("Kind");
                }
            }
        }

        private string _file;

        public string File
        {
            get { return _file; }
            set
            {
                if (_file != value)
                {
                    _file = value;
                    OnPropertyChanged("File");
                }
            }
        }

        private string _project;

        public string Project
        {
            get { return _project; }
            set
            {
                if (_project != value)
                {
                    _project = value;
                    OnPropertyChanged("Project");
                }
            }
        }

        private int _line;

        public int Line
        {
            get { return _line; }
            set
            {
                if (_line != value)
                {
                    _line = value;
                    OnPropertyChanged("Line");
                }
            }
        }

        private int _column;

        public int Column
        {
            get { return _column; }
            set
            {
                if (_column != value)
                {
                    _column = value;
                    OnPropertyChanged("Column");
                }
            }
        }

        public CompileIntellisense()
        {

        }

        public CompileIntellisense(CompileIntellisenseKinds kind, string desc, string sol, string file, int line, int column, string pj)
        {
            _kind = kind;
            _description = desc;
            _solution = sol;
            _file = file;
            _line = line;
            _column = column;
            _project = pj;
        }

    }

    public class CompileErrorModel : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;
        protected void OnPropertyChanged(string propertyName)
        {
            if (PropertyChanged != null)
            {
                PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
            }
        }

        private CustomCollection<CompileIntellisense> _data = new CustomCollection<CompileIntellisense>();

        public CustomCollection<CompileIntellisense> Data
        {
            get { return _data; }
            set
            {
                if (_data != value)
                {
                    _data = value;
                    OnPropertyChanged("Data");
                }
            }
        }

        private CollectionViewSource _dataView = new CollectionViewSource();

        public CollectionViewSource DataView
        {
            get { return _dataView; }
            set
            {
                if (_dataView != value)
                {
                    _dataView.Filter -= _dataView_Filter;
                    _dataView = value;
                    _dataView.Filter += _dataView_Filter;
                    OnPropertyChanged("DataView");
                }
            }
        }

        private bool[] _flag = new bool[3] { true, false, false };
        public bool DisplayError
        {
            get { return _flag[0]; }
            set
            {
                if (_flag[0] != value)
                {
                    _flag[0] = value;
                    OnPropertyChanged("DisplayError");
                }
            }
        }

        public bool DisplayWarning
        {
            get { return _flag[1]; }
            set
            {
                if (_flag[1] != value)
                {
                    _flag[1] = value;
                    OnPropertyChanged("DisplayWarning");
                }
            }
        }

        public bool DisplayTip
        {
            get { return _flag[2]; }
            set
            {
                if (_flag[2] != value)
                {
                    _flag[2] = value;
                    OnPropertyChanged("DisplayTip");
                }
            }
        }

        private string _errorHeader = "0个错误";
        private string _warningHeader = "0个警告";
        private string _tipHeader = "0个提示";

        public string ErrorHeader
        {
            get { return _errorHeader; }
            set
            {
                if (_errorHeader != value)
                {
                    _errorHeader = value;
                    OnPropertyChanged("ErrorHeader");
                }
            }
        }

        public string WarningHeader
        {
            get { return _warningHeader; }
            set
            {
                if (_warningHeader != value)
                {
                    _warningHeader = value;
                    OnPropertyChanged("WarningHeader");
                }
            }
        }

        public string TipHeader
        {
            get { return _tipHeader; }
            set
            {
                if (_tipHeader != value)
                {
                    _tipHeader = value;
                    OnPropertyChanged("TipHeader");
                }
            }
        }

        public CompileErrorModel()
        {
            _data.CollectionChanged += _data_CollectionChanged;
            _dataView.Source = _data;
            _dataView.Filter += _dataView_Filter;
        }

        private const string ERROR_FORMAT = "{0}个错误";
        private const string WARNING_FORMAT = "{0}个警告";
        private const string TIP_FORMAT = "{0}个提示";

        void _data_CollectionChanged(object sender, NotifyCollectionChangedEventArgs e)
        {
            int error = 0;
            int warning = 0;
            int tip = 0;
            foreach (var d in _data)
            {
                switch (d.Kind)
                {
                    case CompileIntellisenseKinds.Error:
                        error++;
                        break;
                    case CompileIntellisenseKinds.Tip:
                        tip++;
                        break;
                    case CompileIntellisenseKinds.Warning:
                        warning++;
                        break;
                }
            }
            ErrorHeader = string.Format(ERROR_FORMAT, error);
            WarningHeader = string.Format(WARNING_FORMAT, warning);
            TipHeader = string.Format(TIP_FORMAT, tip);
        }

        void _dataView_Filter(object sender, FilterEventArgs e)
        {
            CompileIntellisense ci = e.Item as CompileIntellisense;
            if (ci != null)
            {
                if (ci.Kind == CompileIntellisenseKinds.Error && _flag[0])
                {
                    e.Accepted = true;
                    return;
                }
                if (ci.Kind == CompileIntellisenseKinds.Tip && _flag[2])
                {
                    e.Accepted = true;
                    return;
                }
                if (_flag[1])
                {
                    e.Accepted = ci.Kind.CompareTo(CompileIntellisenseKinds.Tip) > 0;
                    return;
                }
                e.Accepted = false;
            }
        }
    }



    public partial class ErrorPanel : UserControl
    {
        public ErrorPanel()
        {
            InitializeComponent();
            Model = new CompileErrorModel();
        }

        public static readonly DependencyProperty ModelProperty = DependencyProperty.Register("Model", typeof(CompileErrorModel), typeof(ErrorPanel), new PropertyMetadata(null));
        /// <summary>
        /// 源
        /// </summary>
        public CompileErrorModel Model
        {
            get { return (CompileErrorModel)GetValue(ModelProperty); }
            set { SetValue(ModelProperty, value); }
        }

        public void Add(CompileIntellisenseKinds kind, string desc, string file, int line, int column, string pj)
        {
            //Dispatcher.BeginInvoke((Action)delegate () { Model.Data.Add(new CompileIntellisense(kind, desc, string.Empty, file, line, column, pj)); }, DispatcherPriority.DataBind);
        }
        public void Add(CompileIntellisenseKinds kind, string desc, string sol, string file, int line, int column, string pj)
        {
            //Dispatcher.BeginInvoke((Action)delegate () { Model.Data.Add(new CompileIntellisense(kind, desc, sol, file, line, column, pj)); }, DispatcherPriority.DataBind);
        }

        public void Clear()
        {
            //Dispatcher.BeginInvoke((Action)delegate () { Model.Data.Clear(); }, DispatcherPriority.DataBind);
        }

    }
}
