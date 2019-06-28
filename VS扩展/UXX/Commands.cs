using System;
using System.Collections.Generic;
using System.ComponentModel.Design;
using System.Globalization;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using EnvDTE;
using Microsoft;
using Microsoft.VisualStudio;
using Microsoft.VisualStudio.Shell;
using Microsoft.VisualStudio.Shell.Interop;
using Microsoft.VisualStudio.Threading;
using UXX.Compiler;
using UXX.Windows;
using Task = System.Threading.Tasks.Task;
using System.Windows.Controls.Primitives;

namespace UXX
{
    internal sealed class Commands
    {
        internal static class PkgCmdIDList
        {
            public const uint cmdidConfig = 0x2002;

            public const uint cmdidCompile = 0x2005;
            public const uint cmdidCompileAll = 0x2006;

            public const uint cmdidCompileError = 0x4001;

            public const uint cmdidOutput = 0x5001;
            public const uint cmdidOutput_toolbar = 0x5002;
            public const uint cmdidOutput_toolgroup = 0x5003;
            public const uint cmdidOutput_clear = 0x5004;
            public const uint cmdidOutput_autowrap = 0x5005;
        };

        public static readonly Guid CommandSet = new Guid("9d1870c7-944c-40be-84fd-348bdc4c098b");

        private readonly UXXPackage package;

        private Commands(UXXPackage package, IVsMonitorSelection monitorSelection, OleMenuCommandService commandService)
        {
            this.package = package ?? throw new ArgumentNullException(nameof(package));
            commandService = commandService ?? throw new ArgumentNullException(nameof(commandService));

            var id = new CommandID(CommandSet, (int)PkgCmdIDList.cmdidCompile);
            var command = new OleMenuCommand(CompileCommandCallback, id);
            command.BeforeQueryStatus += CheckIsCompiling_BeforeQueryStatus;
            commandService.AddCommand(command);

            id = new CommandID(CommandSet, (int)PkgCmdIDList.cmdidCompileAll);
            command = new OleMenuCommand(CompileAllCommandCallback, id);
            command.BeforeQueryStatus += CheckIsCompiling_BeforeQueryStatus;
            commandService.AddCommand(command);

            id = new CommandID(CommandSet, (int)PkgCmdIDList.cmdidCompileError);
            command = new OleMenuCommand(ShowCompileErrorWindow, id);
            commandService.AddCommand(command);

            id = new CommandID(CommandSet, (int)PkgCmdIDList.cmdidOutput);
            command = new OleMenuCommand(ShowOutputWindow, id);
            commandService.AddCommand(command);

        }

        public void Dispose()
        {
            
        }

        private void CheckIsCompiling_BeforeQueryStatus(object sender, EventArgs e)
        {
            long iscompiling = Interlocked.Read(ref package.IsCompiling);
            (sender as OleMenuCommand).Enabled = iscompiling == 0;
        }

        public static Commands Instance
        {
            get;
            private set;
        }

        private Microsoft.VisualStudio.Shell.IAsyncServiceProvider ServiceProvider
        {
            get
            {
                return this.package;
            }
        }

        public static async Task InitializeAsync(UXXPackage package)
        {
            await ThreadHelper.JoinableTaskFactory.SwitchToMainThreadAsync(package.DisposalToken);

            OleMenuCommandService commandService = await package.GetServiceAsync((typeof(IMenuCommandService))) as OleMenuCommandService;

            IVsMonitorSelection monitorSelection = await package.GetServiceAsync(typeof(IVsMonitorSelection)) as IVsMonitorSelection;

            Instance = new Commands(package, monitorSelection, commandService);
        }

        private void ShowOutputWindow(object sender, EventArgs e)
        {
            ThreadHelper.ThrowIfNotOnUIThread();
            ToolWindowPane window = package.FindToolWindow(typeof(CompilerOutputWindow), 0, true);
            if ((null == window) || (null == window.Frame))
            {
                throw new NotSupportedException("无法创建U++输出窗口");
            }
            IVsWindowFrame windowFrame = (IVsWindowFrame)window.Frame;
            ErrorHandler.ThrowOnFailure(windowFrame.Show());
        }

        private void ShowCompileErrorWindow(object sender, EventArgs e)
        {
            ThreadHelper.ThrowIfNotOnUIThread();
            ToolWindowPane window = package.FindToolWindow(typeof(CompileErrorWindow), 0, true);
            if ((null == window) || (null == window.Frame))
            {
                throw new NotSupportedException("无法创建U++错误列表窗口");
            }
            IVsWindowFrame windowFrame = (IVsWindowFrame)window.Frame;
            ErrorHandler.ThrowOnFailure(windowFrame.Show());
        }

        private void CompileCommandCallback(object sender, EventArgs args)
        {
            ThreadHelper.ThrowIfNotOnUIThread();

            //var mainWindow = System.Windows.Application.Current.MainWindow;
            //var contentControl = mainWindow?.Template?.FindName("PART_SccStatusBarHost", mainWindow) as System.Windows.Controls.ContentControl;

            //var statusbar = contentControl?.Content as System.Windows.Controls.Primitives.StatusBar;

            //statusbar.Items.Add(new System.Windows.Controls.Button() { Content = "U++test" });

            Interlocked.Exchange(ref package.IsCompiling, 1);

            if (package.dte2.Solution.SolutionBuild.BuildState != vsBuildState.vsBuildStateInProgress)
            {
                var projs = new List<Project>();
                var selitems = package.dte2.ToolWindows.SolutionExplorer.SelectedItems as Array;
                if (selitems != null)
                {
                    foreach (var selitem in selitems)
                    {
                        var uiitem = selitem as UIHierarchyItem;
                        if (uiitem != null)
                        {
                            var proj = uiitem.Object as Project;
                            if (proj != null)
                            {
                                projs.Add(proj);
                            }
                        }
                    }
                }

                UPPCompiler.RaiseReported(null, new CompilerReportedEventArgs(null));

                package.JoinableTaskFactory.RunAsyncAsVsTask(VsTaskRunContext.UIThreadBackgroundPriority,
                    async cancellationToken =>
                    {
                        await package.JoinableTaskFactory.SwitchToMainThreadAsync();

                        IVsMonitorSelection monitorSelection = await package.GetServiceAsync(typeof(IVsMonitorSelection)) as IVsMonitorSelection;
                        Assumes.Present(monitorSelection);
                        var sol_build_guid = VSConstants.UICONTEXT_SolutionBuilding;
                        uint sol_build_cookie = 0;
                        monitorSelection.GetCmdUIContextCookie(ref sol_build_guid, out sol_build_cookie);

                        int uiactive = -1;
                        monitorSelection.IsCmdUIContextActive(sol_build_cookie, out uiactive);
                        if(uiactive==0)
                            monitorSelection.SetCmdUIContext(sol_build_cookie, 1);


                        IVsStatusbar statusBar = await package.GetServiceAsync(typeof(SVsStatusbar)) as IVsStatusbar;

                        object icon = (short)Microsoft.VisualStudio.Shell.Interop.Constants.SBAI_Build;

                        statusBar?.SetText("开始编译...");
                        statusBar?.Animation(1, ref icon);

                        var tasks = package.Compile(projs);

                        statusBar?.SetText("编译...");

                        await TaskScheduler.Default;

                        await Task.WhenAll(tasks.Select(vtask => vtask.WaitForFinishedAsync()));

                        await package.JoinableTaskFactory.SwitchToMainThreadAsync();

                        statusBar?.SetText("链接源文件...");
                        tasks.ForEach(task => task.ReLink());

                        statusBar?.Animation(0, ref icon);

                        Interlocked.Exchange(ref package.IsCompiling, 0);

                        uiactive = -1;
                        monitorSelection.IsCmdUIContextActive(sol_build_cookie, out uiactive);
                        if (uiactive == 1)
                            monitorSelection.SetCmdUIContext(sol_build_cookie, 0);

                        statusBar?.SetText("已完成");
                        statusBar?.Clear();

                        return VSConstants.S_OK;
                    });
            }
        }

        private void CompileAllCommandCallback(object sender, EventArgs args)
        {
            ThreadHelper.ThrowIfNotOnUIThread();

            Interlocked.Exchange(ref package.IsCompiling, 1);

            var projs = new List<Project>();
            var selitems = package.dte2.ToolWindows.SolutionExplorer.UIHierarchyItems;
            if (selitems != null)
            {
                foreach (UIHierarchyItem uiitem in selitems)
                {
                    if (uiitem != null)
                    {
                        var proj = uiitem.Object as Project;
                        if (proj != null)
                        {
                            projs.Add(proj);
                        }
                    }
                }
            }

            UPPCompiler.RaiseReported(null, new CompilerReportedEventArgs(null));

            package.JoinableTaskFactory.RunAsyncAsVsTask(VsTaskRunContext.UIThreadBackgroundPriority, 
                async cancellationToken=>
            {
                await package.JoinableTaskFactory.SwitchToMainThreadAsync();

                IVsStatusbar statusBar = (IVsStatusbar)package.GetServiceAsync(typeof(SVsStatusbar));

                object icon = (short)Microsoft.VisualStudio.Shell.Interop.Constants.SBAI_Build;

                statusBar?.Animation(1, ref icon);

                var tasks = package.Compile(projs);

                await TaskScheduler.Default;

                await Task.WhenAll(tasks.Select(vtask => vtask.WaitForFinishedAsync()));

                await package.JoinableTaskFactory.SwitchToMainThreadAsync();

                statusBar?.Animation(0, ref icon);

                Interlocked.Exchange(ref package.IsCompiling, 0);

                return VSConstants.S_OK;
            });
        }
    }
}
