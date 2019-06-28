using System;
using System.Linq;
using System.ComponentModel.Design;
using System.Diagnostics;
using System.Diagnostics.CodeAnalysis;
using System.Globalization;
using System.Runtime.InteropServices;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using EnvDTE;
using EnvDTE80;
using Microsoft;
using Microsoft.VisualStudio;
using Microsoft.VisualStudio.OLE.Interop;
using Microsoft.VisualStudio.Shell;
using Microsoft.VisualStudio.Shell.Interop;
using Microsoft.Win32;
using UXX.Compiler;
using Task = System.Threading.Tasks.Task;
using UXX.Windows;

namespace UXX
{
    [PackageRegistration(UseManagedResourcesOnly = true, AllowsBackgroundLoading = true)]
    [InstalledProductRegistration("#110", "#112", "1.0", IconResourceID = 400)] // Info on this package for Help/About
    [Guid(UXXPackage.PackageGuidString)]
    [ProvideAutoLoad(UIContextGuids.NoSolution)]
    [ProvideToolWindow(typeof(CompilerOutputWindow))]
    [ProvideToolWindow(typeof(CompileErrorWindow))]
    [SuppressMessage("StyleCop.CSharp.DocumentationRules", "SA1650:ElementDocumentationMustBeSpelledCorrectly", Justification = "pkgdef, VS and vsixmanifest are valid VS terms")]
    [ProvideMenuResource("Menus.ctmenu", 1)]
    public sealed class UXXPackage : AsyncPackage
    {
        public const string PackageGuidString = "bebe7c15-bcaa-42e7-9265-f2d956bb346a";

        public DTE2 dte2 = null;
        public BuildEvents dte2_buildevents = null;
        public CommandEvents dte2_cmdevents = null;
        public static UXXPackage singleton = null;

        internal long IsCompiling = 0;


        public UXXPackage()
        {
            
        }

        protected override async Task InitializeAsync(CancellationToken cancellationToken, IProgress<ServiceProgressData> progress)
        {
            await this.JoinableTaskFactory.SwitchToMainThreadAsync(cancellationToken);
            dte2 = await GetServiceAsync(typeof(SDTE)) as DTE2;
            Assumes.Present(dte2);
            InitializeAsync();
            singleton = this;
            await Commands.InitializeAsync(this);
        }

        private void InitializeAsync()
        {
            ThreadHelper.ThrowIfNotOnUIThread();
            dte2_buildevents = dte2.Events.BuildEvents;
            dte2_cmdevents = dte2.Events.CommandEvents;
        }
        

        public System.Collections.Generic.List<UPPCompiler> Compile(System.Collections.Generic.List<Project> projects)
        {
            ThreadHelper.ThrowIfNotOnUIThread();
            var vtasks = new System.Collections.Generic.List<UPPCompiler>();

            if (UPPCompilerExtensions.IsUSDKSupported())
            {
                var tasks = new System.Collections.Generic.List<UPPCompiler>();

                bool failure = false;
                string failure_title = string.Empty;
                string failure_msg = string.Empty;

                foreach (var project in projects)
                {
                    string ProjectConfig = project.ConfigurationManager.ActiveConfiguration.ConfigurationName;
                    string Platform = project.ConfigurationManager.ActiveConfiguration.PlatformName;

                    string configneedmessage = string.Empty;
                    SDKError uerr = project.DetectUSDK(ProjectConfig, Platform, out configneedmessage);
                    if (uerr == SDKError.ConfigNeed)
                    {
                        failure_title = "未设置指定配置";
                        failure_msg = configneedmessage;
                        failure = true;
                    }

                    if (uerr == SDKError.Success)
                    {
                        using (var serviceProvider = new ServiceProvider(dte2.DTE as Microsoft.VisualStudio.OLE.Interop.IServiceProvider))
                        {
                            Guid guid = project.GetProjectGuid(serviceProvider);

                            string clmsg = string.Empty;
                            UPPCompiler uppcl = UPPCompiler.Create(0, dte2.GetVCProject(guid), ProjectConfig, Platform, out clmsg);

                            if (uppcl != null)
                            {
                                tasks.Add(uppcl);
                            }
                            else
                            {
                                failure_title = "无效项设置";
                                failure_msg = clmsg;
                                failure = true;
                                foreach (var task in tasks)
                                {
                                    task.Dispose();
                                }
                                tasks.Clear();
                            }
                        }
                    }
                }

                if (!failure)
                {
                    if (tasks.Count > 0)
                    {

                        foreach (var task in tasks)
                        {
                            if (task.Compile())
                            {
                                vtasks.Add(task);
                            }
                        }
                    }
                }
            }
            return vtasks;
        }
    }
}
