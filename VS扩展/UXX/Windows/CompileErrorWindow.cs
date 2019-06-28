using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows;
using System.Runtime.InteropServices;
using Microsoft.VisualStudio.Shell.Interop;
using Microsoft.VisualStudio.Shell;
using System.ComponentModel.Design;
using UXX.Compiler;

namespace UXX.Windows
{
    [Guid("39817FF7-D4E4-442C-A4D6-C19E708F7893")]
    public class CompileErrorWindow : ToolWindowPane
    {

        private ErrorPanel _error;

        public CompileErrorWindow() :
            base(null)
        {
            this.Caption = "U++错误列表";
            this.BitmapResourceID = 301;
            this.BitmapIndex = 1;

            _error = new ErrorPanel();
            base.Content = _error;
            UPPCompiler.Reported += UPPCompiler_Reported;
        }

        private void UPPCompiler_Reported(object sender, CompilerReportedEventArgs e)
        {
            if (e.Clear) Clear();
            else
            {
                if (!e.IsZombie)
                {
                    var pj = e.Compiler.VCConfig;
                    if (pj != null)
                    {
                       switch(e.Mode)
                       {
                           case UWorkerReports.Tip:
                               Add(CompileIntellisenseKinds.Tip, e.Message, e.File, e.Line, e.Column, pj.Name);
                               break;
                           case UWorkerReports.Warning:
                                 Add(CompileIntellisenseKinds.Warning, e.Message, e.File, e.Line, e.Column, pj.Name);
                               break;
                           case UWorkerReports.Error:
                               Add(CompileIntellisenseKinds.Error, e.Message, e.Solution, e.File, e.Line, e.Column, pj.Name);
                               break;
                       }
                    }
                }
            }
        }

        public override void OnToolWindowCreated()
        {
            base.OnToolWindowCreated();
        }

        public void Add(CompileIntellisenseKinds kind, string desc, string file, int line, int column, string pj)
        {
            _error.Add(kind, desc, string.Empty, file, line, column, pj);
        }
        public void Add(CompileIntellisenseKinds kind, string desc, string sol, string file, int line, int column, string pj)
        {
           _error.Add(kind, desc, sol, file, line, column, pj);
        }

        public void Clear()
        {
            _error.Clear();
        }
    }
}
