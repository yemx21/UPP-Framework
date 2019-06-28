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
    [Guid("CE5E27FB-1C13-4D04-92DC-D0B7B5CD688D")]
    class CompilerOutputWindow : ToolWindowPane
    {
        private OutputPanel _output;

        public CompilerOutputWindow() :
            base(null)
        {
            this.Caption = "U++输出";
            this.BitmapResourceID = 301;
            this.BitmapIndex = 1;

            base.ToolBar = new CommandID(Commands.CommandSet, (int)Commands.PkgCmdIDList.cmdidOutput_toolbar);

            _output = new OutputPanel();
            base.Content = _output;

            UPPCompiler.Reported += UPPCompiler_Reported;

        }

        private void UPPCompiler_Reported(object sender, CompilerReportedEventArgs e)
        {
            if (e.Clear)
                _output.Clear();
            else
            {
                if (!e.IsZombie)
                {
                    if (e.Mode == UWorkerReports.Normal)
                    {
                        UPPCompiler upj = e.Compiler;
                        if (upj != null)
                        {
                            AddMessage(string.Format("{0}> {1}", upj.Id, e.Message));
                        }
                    }
                }
            }
        }

        public override void OnToolWindowCreated()
        {
            base.OnToolWindowCreated();

            OleMenuCommandService mcs = GetService(typeof(IMenuCommandService)) as OleMenuCommandService;

            var cmd = new CommandID(Commands.CommandSet, (int)Commands.PkgCmdIDList.cmdidOutput_clear);
            var menu = new MenuCommand(ClearCommandCallback, cmd);
            mcs.AddCommand(menu);

            cmd = new CommandID(Commands.CommandSet, (int)Commands.PkgCmdIDList.cmdidOutput_autowrap);
            menu = new MenuCommand(AutoWrapCommandCallback, cmd);
            mcs.AddCommand(menu);
        }

        private void ClearCommandCallback(object sender, EventArgs args)
        {
            _output.Clear();
        }

        private void AutoWrapCommandCallback(object sender, EventArgs args)
        {
            _output.AutoWarp = !_output.AutoWarp;
        }

        public void AddMessage(string str)
        {
            _output.AddMessage(str);
        }
    }
}
