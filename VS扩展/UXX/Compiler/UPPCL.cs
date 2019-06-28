using EnvDTE;
using Microsoft.VisualStudio.VCProjectEngine;
using System;
using System.Linq;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.IO;
using System.Text;
using System.Xml;
using System.Threading;
using System.Threading.Tasks;
using System.Text.RegularExpressions;
using System.Reflection;

namespace UXX.Compiler
{
    public class SDKVer
    {
        public int Version;
        public string Dir;
        public string CompilerDir;
        public string IncludeDir;
        public string LibraryDir;
        public string VCBuildDir;
        public string Language;

        public SDKVer(int ver, string dir)
        {
            Version = ver;
            Dir = dir;
        }
    }

    public enum SDKError
    {
        NotSupported,
        CompileOff,
        ConfigNeed,
        Success,
    }

    public static class UPPCompilerExtensions
    {
        public static string DetectSDKRootFolder()
        {
            string root = System.Environment.GetEnvironmentVariable("UPPSDK_DIR");
            if (!string.IsNullOrWhiteSpace(root))
            {
                if (Directory.Exists(root))
                {
                    root = root.Trim();
                    if (!root.EndsWith("\\")) root += "\\";
                    return root;
                }
            }
            return string.Empty;
        }

        public static bool IsUSDKSupported()
        {
            string root = DetectSDKRootFolder();
            return !string.IsNullOrWhiteSpace(root);
        }

        public static IEnumerable<SDKVer> DetectVersions()
        {
            string root = DetectSDKRootFolder();
            if (!string.IsNullOrWhiteSpace(root))
            {
                var dirs = Directory.EnumerateDirectories(root);
                foreach (string dir in dirs)
                {
                    SDKVer sdk = null;
                    string cfg = Path.Combine(dir, "SDK.config");
                    try
                    {
                        XmlDocument doc = new XmlDocument();
                        doc.Load(cfg);
                        XmlElement rele = doc.DocumentElement;
                        if (rele != null)
                        {
                            if (string.Compare(rele.Name, "SDK", true) == 0)
                            {
                                string ver = rele.GetAttribute("Version");
                                int num = -1;
                                if (int.TryParse(ver, out num))
                                {
                                    string vdir = dir.Trim();
                                    if (!vdir.EndsWith("\\")) vdir += "\\";
                                    if (num >= 1000)
                                        sdk = new SDKVer(num, vdir);
                                }
                                if (sdk != null)
                                {
                                    var clist = rele.ChildNodes;
                                    foreach (XmlNode cn in clist)
                                    {
                                        XmlElement cne = cn as XmlElement;
                                        if (cne != null)
                                        {
                                            var attr = cne.GetAttribute("Url");
                                            if (!string.IsNullOrWhiteSpace(attr))
                                            {
                                                attr = attr.Trim();
                                                bool flagend = !attr.EndsWith("\\");
                                                if (string.Compare(cn.Name, "UComplier", true) == 0)
                                                {
                                                    sdk.CompilerDir = flagend ? attr + "\\" : attr;
                                                }
                                                else if (string.Compare(cn.Name, "Include", true) == 0)
                                                {
                                                    sdk.IncludeDir = flagend ? attr + "\\" : attr;
                                                }
                                                else if (string.Compare(cn.Name, "Library", true) == 0)
                                                {
                                                    sdk.LibraryDir = flagend ? attr + "\\" : attr;
                                                }
                                                else if (string.Compare(cn.Name, "VCBuild", true) == 0)
                                                {
                                                    sdk.VCBuildDir = flagend ? attr + "\\" : attr;
                                                }
                                                else if (string.Compare(cn.Name, "Language", true) == 0)
                                                {
                                                    sdk.Language = flagend ? attr : attr.Substring(0, attr.Length - 1);
                                                }
                                            }
                                        }
                                    }

                                }
                            }
                        }
                    }
                    catch { continue; }
                    if (sdk != null) yield return sdk;
                }
            }
            yield break;
        }

        public static SDKVer DetectVersion(string incdir, string libdir)
        {
            string root = DetectSDKRootFolder();
            if (!string.IsNullOrWhiteSpace(root))
            {
                var dirs = Directory.EnumerateDirectories(root);
                foreach (string dir in dirs)
                {
                    SDKVer sdk = null;
                    string cfg = Path.Combine(dir, "SDK.config");
                    try
                    {
                        XmlDocument doc = new XmlDocument();
                        doc.Load(cfg);
                        XmlElement rele = doc.DocumentElement;
                        if (rele != null)
                        {
                            if (string.Compare(rele.Name, "SDK", true) == 0)
                            {
                                string ver = rele.GetAttribute("Version");
                                int num = -1;
                                if (int.TryParse(ver, out num))
                                {
                                    string vdir = dir.Trim();
                                    if (!vdir.EndsWith("\\")) vdir += "\\";
                                    if (num >= 1000)
                                        sdk = new SDKVer(num, vdir);
                                }
                                if (sdk != null)
                                {
                                    var clist = rele.ChildNodes;
                                    foreach (XmlNode cn in clist)
                                    {
                                        XmlElement cne = cn as XmlElement;
                                        if (cne != null)
                                        {
                                            var attr = cne.GetAttribute("Url");
                                            if (!string.IsNullOrWhiteSpace(attr))
                                            {
                                                attr = attr.Trim();
                                                bool flagend = !attr.EndsWith("\\");
                                                if (string.Compare(cn.Name, "UCompiler", true) == 0)
                                                {
                                                    sdk.CompilerDir = flagend ? attr + "\\" : attr;
                                                }
                                                else if (string.Compare(cn.Name, "Include", true) == 0)
                                                {
                                                    sdk.IncludeDir = flagend ? attr + "\\" : attr;
                                                }
                                                else if (string.Compare(cn.Name, "Library", true) == 0)
                                                {
                                                    sdk.LibraryDir = flagend ? attr + "\\" : attr;
                                                }
                                                else if (string.Compare(cn.Name, "VCBuild", true) == 0)
                                                {
                                                    sdk.VCBuildDir = flagend ? attr + "\\" : attr;
                                                }
                                                else if (string.Compare(cn.Name, "Language", true) == 0)
                                                {
                                                    sdk.Language = flagend ? attr : attr.Substring(0, attr.Length - 1);
                                                }
                                            }
                                        }
                                    }
                                    string sdk_inc_dir = string.Empty;
                                    if (!string.IsNullOrWhiteSpace(sdk.IncludeDir))
                                    {
                                        sdk_inc_dir = sdk.Dir + sdk.IncludeDir;
                                        if (!sdk_inc_dir.EndsWith("\\")) sdk_inc_dir += "\\";
                                    }
                                    else
                                    {
                                        sdk_inc_dir = sdk.Dir + "Include\\";
                                    }
                                    if (string.Compare(sdk_inc_dir, incdir, true) != 0) sdk = null;
                                    if (sdk != null)
                                    {
                                        string sdk_lib_dir = string.Empty;
                                        if (!string.IsNullOrWhiteSpace(sdk.LibraryDir))
                                        {
                                            sdk_lib_dir = sdk.Dir + sdk.LibraryDir;
                                            if (!sdk_lib_dir.EndsWith("\\")) sdk_lib_dir += "\\";
                                        }
                                        else
                                        {
                                            sdk_lib_dir = sdk.Dir + "Lib\\";
                                        }
                                        if (!libdir.StartsWith(sdk_lib_dir, StringComparison.InvariantCultureIgnoreCase)) sdk = null;
                                    }
                                }
                            }
                        }
                    }
                    catch { continue; }
                    if (sdk != null) return sdk;
                }
            }
            return null;
        }

        public static SDKError DetectUSDK(this Project proj, string config, string platform, out string configneedmessage)
        {
            Microsoft.VisualStudio.Shell.ThreadHelper.ThrowIfNotOnUIThread();

            configneedmessage = "";
            if (proj == null) return SDKError.NotSupported;
            object pjobj = proj.Object;
            if (pjobj == null) return SDKError.NotSupported;
            VCProject vcpj = pjobj as VCProject;
            System.Collections.IEnumerable cfgs = vcpj.Configurations;
            VCPlatform vcp = null;
            VCConfiguration Config = null;
            foreach (object c in cfgs)
            {
                VCConfiguration vc = c as VCConfiguration;
                if (vc != null)
                {
                    if (vc.ConfigurationName == config)
                    {
                        vcp = vc.Platform;
                        if (vcp != null)
                        {
                            if (vcp.Name == platform)
                            {
                                Config = vc;
                                break;
                            }
                        }
                    }
                }
            }
            if (Config == null || vcp == null) return SDKError.NotSupported;

            IVCCollection rules = Config.Rules;
            IVCRulePropertyStorage ureflection = rules.Item("UReflection");
            if (ureflection != null)
            {
                bool bool_ref = false;
                string bool_ref_str = ureflection.GetEvaluatedPropertyValue("UPPReflection");
                bool.TryParse(bool_ref_str, out bool_ref);
                if (!bool_ref) return SDKError.CompileOff;

                SDKError res = SDKError.Success;
                string pre_compile_header_str = ureflection.GetEvaluatedPropertyValue("UPPPreCompileHeader");
                if (string.IsNullOrWhiteSpace(pre_compile_header_str))
                {
                    res = SDKError.ConfigNeed;
                    configneedmessage = "未设置编译器预编译头文件路径: 工程属性=>属性=>配置属性=>U++反射=>反射特性=>编译器预编译头文件路径";
                }

                bool bool_doc = false;
                string bool_doc_str = ureflection.GetEvaluatedPropertyValue("UPPGenerateDoc");
                bool.TryParse(bool_doc_str, out bool_doc);
                if (bool_doc)
                {
                    string doc_file_str = ureflection.GetEvaluatedPropertyValue("UPPDocFile");
                    if (string.IsNullOrWhiteSpace(doc_file_str))
                    {
                        if (res == SDKError.ConfigNeed)
                        {
                            configneedmessage += Environment.NewLine + "启用生成文档但未设置输出文件路径: 工程属性=>属性=>配置属性=>U++反射=>文档=>文档路径";
                        }
                        else
                        {
                            res = SDKError.ConfigNeed;
                            configneedmessage = "启用生成文档但未设置输出文件路径: 工程属性=>属性=>配置属性=>U++反射=>文档=>文档路径";
                        }
                    }
                }
                return res;
            }
            return SDKError.NotSupported;
        }
    }

    public enum UWorkerReports
    {
        Normal,
        Tip,
        Warning,
        Error,
    }

    public class CompilerReportedEventArgs : EventArgs
    {
        private WeakReference _cl = new WeakReference(null);
        public UPPCompiler Compiler { get { return _cl.IsAlive ? _cl.Target as UPPCompiler : null; } }

        public double? Progress { get; private set; }

        public string Message { get; private set; }

        public string Solution { get; private set; }

        public UWorkerReports Mode { get; private set; }

        public int Line = -1;

        public int Column = -1;

        public string File { get; private set; }

        public bool Clear { get; private set; }

        public bool IsZombie { get { return !_cl.IsAlive; } }

        public CompilerReportedEventArgs(UPPCompiler uppcl)
        {
            this._cl.Target = uppcl;
            Clear = true;
        }

        public CompilerReportedEventArgs(UPPCompiler uppcl, double progress)
        {
            this._cl.Target = uppcl;
            Progress = progress;
            Mode = UWorkerReports.Normal;
        }

        public CompilerReportedEventArgs(UPPCompiler uppcl, string msg)
        {
            this._cl.Target = uppcl;
            Message = msg;
            Mode = UWorkerReports.Normal;
        }

        public CompilerReportedEventArgs(UPPCompiler uppcl, string msg, UWorkerReports mode)
        {
            this._cl.Target = uppcl;
            Message = msg;
            Mode = mode;
        }

        public CompilerReportedEventArgs(UPPCompiler uppcl, string err, string sol)
        {
            Mode = UWorkerReports.Error;
            Message = err;
            Solution = sol;
        }

        public CompilerReportedEventArgs(UPPCompiler uppcl, string msg, UWorkerReports mode, string file = "", int line = -1, int column = -1)
        {
            this._cl.Target = uppcl;
            Message = msg;
            Mode = mode;
            File = file;
            Line = line;
            Column = column;
        }

        public CompilerReportedEventArgs(UPPCompiler uppcl, string err, string sol, string file = "", int line = -1, int column = -1)
        {
            this._cl.Target = uppcl;
            Mode = UWorkerReports.Error;
            Message = err;
            Solution = sol;
            File = file;
            Line = line;
            Column = column;
        }

        public CompilerReportedEventArgs(UPPCompiler uppcl, string err, string file = "", int line = -1, int column = -1)
        {
            this._cl.Target = uppcl;
            Mode = UWorkerReports.Error;
            Message = err;
            File = file;
            Line = line;
            Column = column;
        }

        public CompilerReportedEventArgs(UPPCompiler uppcl, UWorkerReports mode, string msg, string solution="", string file = "", int line = -1, int column = -1)
        {
            this._cl.Target = uppcl;
            Mode = mode;
            Message = msg;
            Solution = solution;
            File = file;
            Line = line;
            Column = column;
        }
    }

    public class Precompile_Config
    {
        public List<string> System_Header_Dirs = new List<string>();
        public List<string> User_Header_Dirs = new List<string>();
        public List<string> CXX_Header_Dirs = new List<string>();
        public List<string> Macros = new List<string>();

        public string Input { get; set; }
        public string Output { get; set; }

        public bool Valid
        {
            get
            {
                return !string.IsNullOrEmpty(Input) && !string.IsNullOrEmpty(Output);
            }
        }

        public string CommandLine
        {
            get
            {
                StringBuilder sb = new StringBuilder();
                if (System_Header_Dirs.Count > 0)
                {
                    sb.Append(string.Join(" ", System_Header_Dirs.Select(dir => string.Format("-IS=\"{0}\"", dir))));
                    sb.Append(" ");
                }
                if (CXX_Header_Dirs.Count > 0)
                {
                    sb.Append(string.Join(" ", CXX_Header_Dirs.Select(dir => string.Format("-IC=\"{0}\"", dir))));
                    sb.Append(" ");
                }
                if (User_Header_Dirs.Count > 0)
                {
                    sb.Append(string.Join(" ", User_Header_Dirs.Select(dir => string.Format("-I=\"{0}\"", dir))));
                    sb.Append(" ");
                }

                if (Macros.Count > 0)
                {
                    sb.Append(string.Join(" ", Macros.Select(m => string.Format("-D=\"{0}\"", m))));
                    sb.Append(" ");
                }

                sb.AppendFormat("-inoutput=\"{0}>{1}\" ", Input, Output);
                sb.Append("--emitpch --slience");
                return sb.ToString();
            }
        }
    }

    public class Compile_Config
    {
        public List<string> System_Header_Dirs = new List<string>();
        public List<string> User_Header_Dirs = new List<string>();
        public List<string> CXX_Header_Dirs = new List<string>();
        public List<string> Macros = new List<string>();

        public List<Tuple<string, string, string>> Files = new List<Tuple<string, string, string>>();

        public List<Tuple<string, Version, Guid>> Assemblies = new List<Tuple<string, Version, Guid>>();
        public string Precompiled_Header { get; set; }
        public string SwitchFlag { get; set; }
        public string EntryHeader { get; set; }
        public string EntrySource { get; set; }

        public bool CustomEntry = false;

        public string AssemblyHeader { get; set; }
        public string AssemblySource { get; set; }

        public bool Valid
        {
            get
            {
                return Files.Count>0  && !string.IsNullOrWhiteSpace(SwitchFlag);
            }
        }

        public string CommandLine
        {
            get
            {
                StringBuilder sb = new StringBuilder();
                if (System_Header_Dirs.Count > 0)
                {
                    sb.Append(string.Join(" ", System_Header_Dirs.Select(dir => string.Format("-IS=\"{0}\"", dir))));
                    sb.Append(" ");
                }
                if (CXX_Header_Dirs.Count > 0)
                {
                    sb.Append(string.Join(" ", CXX_Header_Dirs.Select(dir => string.Format("-IC=\"{0}\"", dir))));
                    sb.Append(" ");
                }
                if (User_Header_Dirs.Count > 0)
                {
                    sb.Append(string.Join(" ", User_Header_Dirs.Select(dir => string.Format("-I=\"{0}\"", dir))));
                    sb.Append(" ");
                }

                if (Macros.Count > 0)
                {
                    sb.Append(string.Join(" ", Macros.Select(m => string.Format("-D=\"{0}\"", m))));
                    sb.Append(" ");
                }

                if (Files.Count > 0)
                {
                    sb.Append(string.Join(" ", Files.Select(m => string.Format("-inoutput=\"{0}>{1}\"", m.Item1, m.Item2))));
                    sb.Append(" ");
                }

                sb.Append("--gen --slience");
                //sb.Append("--gen");

                if (Assemblies.Count>0)
                {
                    sb.AppendFormat(" --asm=\"{0}\"", string.Join(";", Assemblies.Select(m => string.Format("asm={0}, ver={1}, pbk={2}", m.Item1, m.Item2, m.Item3))));
                }

                sb.AppendFormat(" --pch=\"{0}\"", Precompiled_Header);
                sb.AppendFormat(" --switchflag=\"{0}\"", SwitchFlag);
                sb.AppendFormat(" --entryheader=\"{0}\"", EntryHeader);
                sb.AppendFormat(" --entrysource=\"{0}\"", EntrySource);
                sb.AppendFormat(" --asmheader=\"{0}\"", AssemblyHeader);
                sb.AppendFormat(" --asmsource=\"{0}\"", AssemblySource);

                if (CustomEntry)
                {
                    sb.Append(" --customentry");
                }

                return sb.ToString();
            }
        }
    }

    public enum BuiltInDisplayLanguages
    {
        None,
        SimpleChinese,
        UnitedStates,
    }

    public class Translator
    {
        public static BuiltInDisplayLanguages BuiltInUse = BuiltInDisplayLanguages.None;

        protected static Dictionary<string, string> dict_en = new Dictionary<string, string>();

        protected static Dictionary<string, string> dict_cn = new Dictionary<string, string>();

        protected static Dictionary<string, string> dict_special = new Dictionary<string, string>();

        protected static void Load(Stream sm, ref Dictionary<string, string> dict)
        {
            try
            {
                if (sm != null)
                {
                    if (sm.CanRead)
                    {
                        XmlDocument doc = new XmlDocument();
                        doc.Load(sm);
                        var root = doc.DocumentElement;
                        if (root != null)
                        {
                            foreach (var cn in root.ChildNodes)
                            {
                                XmlElement cne = cn as XmlElement;
                                if (cne != null)
                                {
                                    var str = cne.Name;
                                    str = str.Trim();
                                    if (!string.IsNullOrWhiteSpace(str))
                                    {
                                        if (!dict.ContainsKey(str))
                                            dict.Add(str, cne.Value);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            catch { }
        }

        static Translator()
        {
            dict_special.Add("NewLine", Environment.NewLine);
            dict_special.Add("Tab", "\t");

            Assembly exeass = Assembly.GetExecutingAssembly();
            using (Stream sm_en_US = exeass.GetManifestResourceStream("en_US.xml"))
            {
                Load(sm_en_US, ref dict_en);
            }
            using (Stream sm_zh_CN = exeass.GetManifestResourceStream("zh_CN.xml"))
            {
                Load(sm_zh_CN, ref dict_en);
            }
        }


        private static Regex rx = new Regex(@"%\((?<var>\w+)\)", RegexOptions.Compiled);

        private static Regex report_rx = new Regex(@"\s*(?<mode>\d*)\s*((?<project>\w*)>)*((?<file>\w*)\((?<line>[-+]?\d*),(?<column>[-+]?\d*)\)*:)*\s*(?<msg>.*)", RegexOptions.Compiled);


        public Dictionary<string, string> dicts = new Dictionary<string, string>();

        protected bool Load(string file, string name)
        {
            try
            {
                if (!string.IsNullOrWhiteSpace(file))
                {
                    if (System.IO.File.Exists(file))
                    {
                        XmlDocument doc = new XmlDocument();
                        doc.Load(file);
                        var root = doc.DocumentElement;
                        if (root != null)
                        {
                            string na = root.GetAttribute("Name");
                            na = na.Trim();
                            if (na.Equals(name, StringComparison.InvariantCultureIgnoreCase))
                            {
                                foreach (var cn in root.ChildNodes)
                                {
                                    XmlElement cne = cn as XmlElement;
                                    if (cne != null)
                                    {
                                        var str = cne.Name;
                                        str = str.Trim();
                                        if (!string.IsNullOrWhiteSpace(str))
                                        {
                                            if (!dicts.ContainsKey(str))
                                                dicts.Add(str, cne.InnerText);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            catch { }
            return false;
        }

        public bool Load(SDKVer ver)
        {
            try
            {
                string path = ver.Dir + ver.Language;
                if (!string.IsNullOrWhiteSpace(path))
                {
                    if (System.IO.File.Exists(path))
                    {
                        XmlDocument doc = new XmlDocument();
                        doc.Load(path);
                        var root = doc.DocumentElement;
                        if (root != null)
                        {
                            string def = root.GetAttribute("Default");
                            foreach (var cn in root.ChildNodes)
                            {
                                XmlElement cne = cn as XmlElement;
                                if (cne != null)
                                {
                                    var attr = cne.GetAttribute("Url");
                                    if (!string.IsNullOrWhiteSpace(attr))
                                    {
                                        attr = attr.Trim();
                                        var name = cne.GetAttribute("Name");
                                        if (!string.IsNullOrWhiteSpace(name))
                                        {
                                            if (name.Equals(def, StringComparison.InvariantCultureIgnoreCase))
                                            {
                                                string dir = System.IO.Path.GetDirectoryName(path);
                                                if (!dir.EndsWith("\\")) dir += "\\";
                                                attr = dir + attr;
                                                return Load(attr, name);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            catch { }
            return false;
        }

        protected bool TryTranslate(string str, out string res)
        {
            if (dict_special.ContainsKey(str))
            {
                res = dict_special[str];
                return true;
            }
            if (dicts.ContainsKey(str))
            {
                res = dicts[str];
                return true;
            }
            switch (BuiltInUse)
            {
                case BuiltInDisplayLanguages.SimpleChinese:
                    {
                        if (dict_cn.ContainsKey(str))
                        {
                            res = dict_cn[str];
                            return true;
                        }
                    }
                    break;
                case BuiltInDisplayLanguages.UnitedStates:
                    {
                        if (dict_en.ContainsKey(str))
                        {
                            res = dict_en[str];
                            return true;
                        }
                    }
                    break;
                case BuiltInDisplayLanguages.None:
                default:
                    break;
            }
            res = string.Empty;
            return false;
        }

        public string Translate(string str)
        {
            String sb = string.Copy(str);
            int cur = 0;
            while (true)
            {
                Match m = rx.Match(sb, cur);
                if (m.Success)
                {
                    Group v = m.Groups["var"];
                    if (v != null)
                    {
                        if (v.Success)
                        {
                            string res;
                            if (TryTranslate(v.Value, out res))
                            {
                                sb = sb.Remove(m.Index, m.Length);
                                sb = sb.Insert(m.Index, res);
                                cur += res.Length - m.Length;
                            }
                        }
                    }
                    cur += m.Length;
                }
                else
                    break;
            }

            return sb;
        }

        public IEnumerable<CompilerReportedEventArgs> Eval(string str, UPPCompiler sender)
        {
            int cur = 0;
            while (true)
            {
                Match m = report_rx.Match(str, cur);
                if (m.Success && m.Length>0)
                {
                    Group v = m.Groups["mode"];
                    if (v != null)
                    {
                        if (v.Success)
                        {
                            int modeval = -1;
                            if (int.TryParse(v.Value, out modeval))
                            {
                                string proj = string.Empty;
                                string file = string.Empty;
                                int line = -1;
                                int column = -1;
                                string msg = string.Empty;

                                var proj_v = m.Groups["project"];
                                if(proj_v!=null)
                                {
                                    if(proj_v.Success)
                                    {
                                        proj = proj_v.Value;
                                    }
                                }

                                var file_v = m.Groups["file"];
                                if (file_v != null)
                                {
                                    if (file_v.Success)
                                    {
                                        file = file_v.Value;
                                    }
                                }

                                var line_v = m.Groups["line"];
                                if (line_v != null)
                                {
                                    if (line_v.Success)
                                    {
                                        int.TryParse(line_v.Value, out line);
                                    }
                                }

                                var column_v = m.Groups["column"];
                                if (column_v != null)
                                {
                                    if (column_v.Success)
                                    {
                                        int.TryParse(column_v.Value, out column);
                                    }
                                }

                                var msg_v = m.Groups["msg"];
                                if (msg_v != null)
                                {
                                    if (msg_v.Success)
                                    {
                                        msg = Translate(msg_v.Value);
                                    }
                                }


                                if (modeval == 101 || modeval == 102)
                                    yield return new CompilerReportedEventArgs(sender, UWorkerReports.Error, msg, proj, file, line, column);
                                else if (modeval == 103 || modeval == 104)
                                    yield return new CompilerReportedEventArgs(sender, UWorkerReports.Tip, msg, proj, file, line, column);
                                else if (modeval == 105 || modeval == 106)
                                    yield return new CompilerReportedEventArgs(sender, UWorkerReports.Warning, msg, proj, file, line, column);
                                else if (modeval == 107 || modeval == 108)
                                    yield return new CompilerReportedEventArgs(sender, UWorkerReports.Normal, msg, proj);
                            }
                        }
                    }
                    cur += m.Length;
                }
                else
                    break;
            }
        }
    }


    public class UPPCompiler: IDisposable
    {
        public int Id { get; set; }
        public string Config { get; set; }
        public string Platform { get; set; }

        private BackgroundWorker worker = null;
        private AsyncManualResetEvent<bool> resetEvent = new AsyncManualResetEvent<bool>(false, false);

        public SDKVer SDKVersion = null;

        public VCConfiguration VCConfig = null;

        public string ProjectFile;
        public string FullConfigName;

        private Precompile_Config cfg_precompile = null;
        private Compile_Config cfg_compile = null;

        private Translator translator = null;
        

        private static WeakEventSource<CompilerReportedEventArgs> reportedSource = new WeakEventSource<CompilerReportedEventArgs>();
        public static event EventHandler<CompilerReportedEventArgs> Reported
        {
            add { reportedSource.Subscribe(value); }
            remove { reportedSource.Unsubscribe(value); }
        }

        public UPPCompiler()
        {
            worker = new BackgroundWorker();
            worker.DoWork += compiler_DoWork;
        }

        public static void RaiseReported(object sender, CompilerReportedEventArgs e)
        {
            reportedSource.Raise(sender, e);
        }

        private bool Initilize(VCProject project, string config, string platform)
        {
            Config = config;
            Platform = platform;

            if (project == null) return false;
            ProjectFile = project.ProjectFile;
            System.Collections.IEnumerable cfgs = project.Configurations;
            VCPlatform vcp = null;
            foreach (object c in cfgs)
            {
                VCConfiguration vc = c as VCConfiguration;
                if (vc != null)
                {
                    if (vc.ConfigurationName == Config)
                    {
                        vcp = vc.Platform;
                        if (vcp != null)
                        {
                            if (vcp.Name == Platform)
                            {
                                VCConfig = vc;
                                break;
                            }
                        }
                    }
                }
            }
            if (Config == null || vcp == null) return false;
            string root = UPPCompilerExtensions.DetectSDKRootFolder();
            if (string.IsNullOrWhiteSpace(root)) return false;
            FullConfigName = string.Format("{0}|{1}", Config, Platform);
            string inc = vcp.IncludeDirectories;

            string[] incs = inc.Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries);
            string inc_res = string.Empty;
            string sdk_inc_dir = string.Empty;
            if (incs != null)
            {
                foreach (string ic in incs)
                {
                    if (ic.StartsWith("$(UPPSDK_DIR)\\"))
                    {
                        sdk_inc_dir = ic.Replace("$(UPPSDK_DIR)\\", root);
                        break;
                    }
                    else if (ic.StartsWith("$(UPPSDK_DIR)"))
                    {
                        sdk_inc_dir = ic.Replace("$(UPPSDK_DIR)", root);
                        break;
                    }
                }
            }

            string lib = vcp.LibraryDirectories;
            string[] libs = lib.Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries);
            string lib_res = string.Empty;
            string sdk_lib_dir = string.Empty;
            if (libs != null)
            {
                foreach (string ic in libs)
                {
                    if (ic.StartsWith("$(UPPSDK_DIR)\\"))
                    {
                        sdk_lib_dir = ic.Replace("$(UPPSDK_DIR)\\", root);
                        break;
                    }
                    else if (ic.StartsWith("$(UPPSDK_DIR)"))
                    {
                        sdk_lib_dir = ic.Replace("$(UPPSDK_DIR)", root);
                        break;
                    }
                }
            }

            if (!sdk_inc_dir.EndsWith("\\")) sdk_inc_dir += "\\";
            if (!sdk_lib_dir.EndsWith("\\")) sdk_lib_dir += "\\";
            SDKVersion = UPPCompilerExtensions.DetectVersion(sdk_inc_dir, sdk_lib_dir);

            translator = new Translator();
            translator.Load(SDKVersion);

            return SDKVersion != null;
        }

        private bool PrepareConfigs(out string msg)
        {
            msg = string.Empty;

            cfg_precompile = new Precompile_Config();
            cfg_compile = new Compile_Config();

            var rules = VCConfig.Rules;

            IVCRulePropertyStorage cfg_dirs = rules.Item("ConfigurationDirectories");
            string headerdirs = cfg_dirs.GetUnevaluatedPropertyValue("IncludePath");
            var header_dirs = headerdirs.Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries).ToList();

            if (header_dirs.Contains("$(IncludePath)"))
            {
                header_dirs.Remove("$(IncludePath)");

                IVCCollection pss = VCConfig.PropertySheets;

                foreach (VCPropertySheet ps in pss)
                {
                    var ps_rules = ps.Rules;
                    IVCRulePropertyStorage ps_cfg_dirs = ps_rules.Item("ConfigurationDirectories");
                    string ps_headerdirs = ps_cfg_dirs.GetUnevaluatedPropertyValue("IncludePath");
                    if(!string.IsNullOrWhiteSpace(ps_headerdirs))
                    {
                        var ps_incs = ps_headerdirs.Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries).Select(xx => xx.Trim().FixDirPath()).Where(xx=>!string.IsNullOrWhiteSpace(xx)).ToList();
                        if (ps_incs.Count > 0)
                            header_dirs.AddRange(ps_incs);
                    }
                }
            }


            if (header_dirs.Contains("$(VC_IncludePath)"))
            {
                var cxx_dirs_evalstr = VCConfig.Evaluate("$(VC_IncludePath)");
                cfg_precompile.CXX_Header_Dirs = cxx_dirs_evalstr.Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries).Select(xx=>xx.Trim().FixDirPath()).ToList();
                cfg_compile.CXX_Header_Dirs = new List<string>(cfg_precompile.CXX_Header_Dirs);
                header_dirs.Remove("$(VC_IncludePath)");
            }
            else
            {
                msg = "Can not find $(VC_IncludePath) defined in the project configurations";
                return false;
            }

            if (header_dirs.Contains("$(WindowsSDK_IncludePath)"))
            {
                var sys_dirs_evalstr = VCConfig.Evaluate("$(WindowsSDK_IncludePath)");
                cfg_precompile.System_Header_Dirs = sys_dirs_evalstr.Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries).Select(xx => xx.Trim().FixDirPath()).ToList();
                cfg_compile.System_Header_Dirs = new List<string>(cfg_precompile.System_Header_Dirs);
                header_dirs.Remove("$(WindowsSDK_IncludePath)");
            }
            else
            {
                msg = "Can not find $(WindowsSDK_IncludePath) defined in the project configurations";
                return false;
            }

            foreach (var dir in header_dirs)
            {
                var extra_dirs_evalstr = VCConfig.Evaluate(dir);
                if (string.IsNullOrWhiteSpace(extra_dirs_evalstr)) continue;
                var evals = extra_dirs_evalstr.Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries).Select(xx => xx.Trim().FixDirPath()).ToList();
                cfg_precompile.User_Header_Dirs.AddRange(evals);
                cfg_compile.User_Header_Dirs.AddRange(evals);
            }

            IVCRulePropertyStorage cfg_cl = rules.Item("CL");
            var add_dirs_evalstr = cfg_cl.GetEvaluatedPropertyValue("AdditionalIncludeDirectories");
            if (!string.IsNullOrWhiteSpace(add_dirs_evalstr))
            {
                var dir_evals = add_dirs_evalstr.Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries).Select(xx => xx.Trim().FixDirPath()).ToList();
                foreach (var dir in dir_evals)
                {
                    cfg_precompile.User_Header_Dirs.Add(dir);
                    cfg_compile.User_Header_Dirs.Add(dir);
                }
            }

            var macro_true_evalstr = cfg_cl.GetEvaluatedPropertyValue("PreprocessorDefinitions");
            var macro_false_evalstr = cfg_cl.GetEvaluatedPropertyValue("UndefinePreprocessorDefinitions");

            if (!string.IsNullOrWhiteSpace(macro_true_evalstr))
            {
                var macro_true_evals = macro_true_evalstr.Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries).Select(xx => xx.Trim()).ToList();

                foreach (var macro_true_eval in macro_true_evals)
                {
                    var macro_true = macro_true_eval.Split(new char[] { '=' }, StringSplitOptions.RemoveEmptyEntries).Select(xx => xx.Trim()).ToList();

                    if (macro_true.Count == 1)
                    {
                        cfg_precompile.Macros.Add(macro_true[0]);
                        cfg_compile.Macros.Add(macro_true[0]);
                    }
                    else if (macro_true.Count == 2)
                    {
                        if (macro_true[1].ToLower()=="true")
                        {
                            cfg_precompile.Macros.Add(macro_true[0]);
                            cfg_compile.Macros.Add(macro_true[0]);
                        }
                        else if (macro_true[1].ToLower() == "false")
                        {
                            cfg_precompile.Macros.Add(string.Format("{0}=0", macro_true[0]));
                            cfg_compile.Macros.Add(string.Format("{0}=0", macro_true[0]));
                        }
                        else
                        {
                            int macro_val = 0;
                            if(int.TryParse(macro_true[1], out macro_val))
                            {
                                if(macro_val>0)
                                {
                                    cfg_precompile.Macros.Add(macro_true[0]);
                                    cfg_compile.Macros.Add(macro_true[0]);
                                }
                                else
                                {
                                    cfg_precompile.Macros.Add(string.Format("{0}=0", macro_true[0]));
                                    cfg_compile.Macros.Add(string.Format("{0}=0", macro_true[0]));
                                }
                            }
                        }
                    }
                }
            }

            if (!string.IsNullOrWhiteSpace(macro_false_evalstr))
            {
                var macro_false_evals = macro_false_evalstr.Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries).Select(xx => xx.Trim()).ToList();

                foreach (var macro_false_eval in macro_false_evals)
                {
                    var macro_false = macro_false_eval.Split(new char[] { '=' }, StringSplitOptions.RemoveEmptyEntries).Select(xx => xx.Trim()).ToList();

                    if (macro_false.Count == 1)
                    {
                        cfg_precompile.Macros.Add(string.Format("{0}=0", macro_false[0]));
                        cfg_compile.Macros.Add(string.Format("{0}=0", macro_false[0]));
                    }
                    else if (macro_false.Count == 2)
                    {
                        if (macro_false[1].ToLower() == "true")
                        {
                            cfg_precompile.Macros.Add(macro_false[0]);
                            cfg_compile.Macros.Add(macro_false[0]);
                        }
                        else if (macro_false[1].ToLower() == "false")
                        {
                            cfg_precompile.Macros.Add(string.Format("{0}=0", macro_false[0]));
                            cfg_compile.Macros.Add(string.Format("{0}=0", macro_false[0]));
                        }
                        else
                        {
                            int macro_val = 0;
                            if (int.TryParse(macro_false[1], out macro_val))
                            {
                                if (macro_val > 0)
                                {
                                    cfg_precompile.Macros.Add(macro_false[0]);
                                    cfg_compile.Macros.Add(macro_false[0]);
                                }
                                else
                                {
                                    cfg_precompile.Macros.Add(string.Format("{0}=0", macro_false[0]));
                                    cfg_compile.Macros.Add(string.Format("{0}=0", macro_false[0]));
                                }
                            }
                        }
                    }
                }
            }

            IVCRulePropertyStorage ureflection = rules.Item("UReflection");
            string precompile_header = ureflection.GetEvaluatedPropertyValue("UPPPreCompileHeader");
            string upp_gendir = ureflection.GetEvaluatedPropertyValue("UPPGenDir");
            if(string.IsNullOrWhiteSpace(upp_gendir))
            {
                msg = "Please specifiy a directory for UPP Compiler to generate codes";
                return false;
            }
            else
            {
                upp_gendir = upp_gendir.Trim();
                if (!upp_gendir.EndsWith("\\")) upp_gendir += "\\";
            }

            if (!string.IsNullOrWhiteSpace(precompile_header))
            {
                cfg_precompile.Input = precompile_header;
                cfg_precompile.Output = upp_gendir + System.IO.Path.GetFileNameWithoutExtension(precompile_header) + ".pch";
                cfg_compile.Precompiled_Header = cfg_precompile.Output;
            }

            cfg_compile.SwitchFlag = ureflection.GetEvaluatedPropertyValue("UPPSwitchFlag");

            if (string.IsNullOrWhiteSpace(cfg_compile.SwitchFlag))
            {
                msg = "Please specifiy a switchflag for UPP Compiler to generate codes";
                return false;
            }


            string upp_entry = ureflection.GetEvaluatedPropertyValue("UPPEntry");
            if (string.IsNullOrWhiteSpace(upp_entry))
            {
                msg = "Please specifiy an entry file name for UPP Compiler to generate codes";
                return false;
            }
            string upp_assembly = ureflection.GetEvaluatedPropertyValue("UPPAssembly");
            if (string.IsNullOrWhiteSpace(upp_assembly))
            {
                msg = "Please specifiy an assembly file name for UPP Compiler to generate codes";
                return false;
            }

            cfg_compile.EntryHeader = upp_gendir + upp_entry + ".uh";
            cfg_compile.EntrySource = upp_gendir + upp_entry + ".upp";

            cfg_compile.AssemblyHeader = upp_gendir + upp_assembly + ".uh";
            cfg_compile.AssemblySource = upp_gendir + upp_assembly + ".upp";

            string upp_customentry = ureflection.GetEvaluatedPropertyValue("UPPCustomEntryFunction").Trim().ToLower();

            if (upp_customentry=="yes" || upp_customentry=="true")
            {
                cfg_compile.CustomEntry = true;
            }

            string upp_asmdef_evalstr = ureflection.GetEvaluatedPropertyValue("UPPAssemblyDefination"); 
            if (!string.IsNullOrWhiteSpace(upp_asmdef_evalstr))
            {
                var asmdef_evals = upp_asmdef_evalstr.Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries).Select(xx => xx.Trim()).ToList();
                foreach (var asmdef in asmdef_evals)
                {
                    var asm = asmdef.Split(new char[] { ',' }, StringSplitOptions.RemoveEmptyEntries).Select(xx => xx.Trim()).ToList();
                    bool success = true;
                    string assembly_name = string.Empty;
                    Version assembly_ver = null;
                    Guid assembly_pbk = Guid.Empty;

                    if (asm.Count==3)
                    {
                        foreach (var iasm in asm)
                        {
                            var iasm_val = iasm.Split(new char[] { '=' }, StringSplitOptions.RemoveEmptyEntries).Select(xx => xx.Trim()).ToList();
                            if(iasm_val.Count==2)
                            {
                                if(iasm_val[0].ToLower()=="asm")
                                {
                                    if (string.IsNullOrWhiteSpace(iasm_val[1]))
                                    {
                                        success = false;
                                    }
                                    assembly_name = iasm_val[1];
                                }
                                else if (iasm_val[0].ToLower() == "ver")
                                {
                                    if(!Version.TryParse(iasm_val[1], out assembly_ver))
                                    {
                                        success = false;
                                    }
                                }
                                else if (iasm_val[0].ToLower() == "pbk")
                                {
                                    if (!Guid.TryParse(iasm_val[1], out assembly_pbk))
                                    {
                                        success = false;
                                    }
                                }
                            }
                        }
                    }
                    if (!success || string.IsNullOrWhiteSpace(assembly_name) || assembly_ver==null || assembly_pbk==Guid.Empty)
                    {
                        msg = "Can not parse assembly definition: " + asmdef;
                        return false;
                    }
                    else
                    {
                        cfg_compile.Assemblies.Add(new Tuple<string, Version, Guid>(assembly_name, assembly_ver, assembly_pbk));
                    }
                }
            }

            VCProject vcprj = VCConfig.project;

            var headers = vcprj.GetAllVCFiles().Where(vcf => vcf.FileType == eFileType.eFileTypeCppHeader && vcf.Extension.Trim() != ".uh");

            headers = headers.GroupBy(h => h.FullPath).Select(h => h.First()).ToList();

            foreach(var header in headers)
            {
                cfg_compile.Files.Add(new Tuple<string, string, string>(header.FullPath, System.IO.Path.ChangeExtension(upp_gendir + header.RelativePath, ".upp"), System.IO.Path.ChangeExtension(header.RelativePath, ".upp")));
            }

            return true;
        }

        public static UPPCompiler Create(int index, VCProject project, string config, string platform, out string msg)
        {
            msg = string.Empty;
            UPPCompiler uppcl = new UPPCompiler() { Id = index };
            if (uppcl.Initilize(project, config, platform))
            {
                if (uppcl.PrepareConfigs(out msg))
                {
                    return uppcl;
                }
            }
            return null;
        }

        public void Destory()
        {
            worker.CancelAsync();
        }

        public void ReLink()
        {
            VCProject vcprj = VCConfig.project;

            IVCCollection filters = vcprj.Filters;

            VCFilter upp_filter = null;

            foreach(VCFilter filter in filters)
            {
                if(filter.Name=="U++")
                {
                    upp_filter = filter;
                    break;
                }
            }

            VCFilter cur_upp_filter = null;

            if(upp_filter!=null)
            {
                IVCCollection subfilters = upp_filter.Filters;
                foreach (VCFilter filter in subfilters)
                {
                    if (filter.Name == cfg_compile.SwitchFlag)
                    {
                        cur_upp_filter = filter;
                        break;
                    }
                }
            }

            if (cur_upp_filter != null)
            {
                upp_filter.RemoveFilter(cur_upp_filter);
                cur_upp_filter = null;
            }

            if (upp_filter == null)
            {
                upp_filter = vcprj.AddFilter("U++");
            }

            if (upp_filter != null)
            {
                if (cur_upp_filter == null)
                {
                    cur_upp_filter = upp_filter.AddFilter(cfg_compile.SwitchFlag);
                }

                if (cur_upp_filter != null)
                {
                    if (File.Exists(cfg_compile.EntryHeader))
                    {
                        VCFile vcfile = cur_upp_filter.AddFile(cfg_compile.EntryHeader);
                        vcfile.FileType = eFileType.eFileTypeCppHeader;
                        vcfile.ItemType = "ClInclude";
                        if (vcfile != null) vcfile.AddVCTool(Config, Platform);
                    }

                    if (File.Exists(cfg_compile.EntrySource))
                    {
                        VCFile vcfile = cur_upp_filter.AddFile(cfg_compile.EntrySource);
                        vcfile.FileType = eFileType.eFileTypeCppCode;
                        vcfile.ItemType = "ClCompile";
                        if (vcfile != null) vcfile.AddVCTool(Config, Platform);
                    }

                    if (File.Exists(cfg_compile.AssemblyHeader))
                    {
                        VCFile vcfile = cur_upp_filter.AddFile(cfg_compile.AssemblyHeader);
                        vcfile.FileType = eFileType.eFileTypeCppHeader;
                        vcfile.ItemType = "ClInclude";
                        if (vcfile != null) vcfile.AddVCTool(Config, Platform);
                    }

                    if (File.Exists(cfg_compile.AssemblySource))
                    {
                        VCFile vcfile = cur_upp_filter.AddFile(cfg_compile.AssemblySource);
                        vcfile.FileType = eFileType.eFileTypeCppCode;
                        vcfile.ItemType = "ClCompile";
                        if (vcfile != null) vcfile.AddVCTool(Config, Platform);
                    }

                    foreach (var inout in cfg_compile.Files)
                    {
                        if (File.Exists(inout.Item2))
                        {
                            VCFile vcfile = cur_upp_filter.AddRelativeFile(inout.Item3, inout.Item2);
                            if (vcfile != null)
                            {
                                if (vcfile.Extension == ".uh")
                                {
                                    vcfile.FileType = eFileType.eFileTypeCppHeader;
                                    vcfile.ItemType = "ClInclude";
                                }
                                else if (vcfile.Extension == ".upp")
                                {
                                    vcfile.FileType = eFileType.eFileTypeCppCode;
                                    vcfile.ItemType = "ClCompile";
                                }
                                vcfile.AddVCTool(Config, Platform);
                            }
                        }
                    }
                }
            }
            vcprj.SaveUserFile();
            vcprj.Save();
        }

        private long precompile_stdcompleted = 0;
        private long compile_stdcompleted = 0;

        private void compiler_DoWork(object sender, DoWorkEventArgs e)
        {
            e.Result = false;

            Interlocked.Exchange(ref precompile_stdcompleted, 0);
            Interlocked.Exchange(ref compile_stdcompleted, 0);

            bool precompile_success = false;
            // Precompile
            using (System.Diagnostics.Process proc = new System.Diagnostics.Process())
            {
                proc.StartInfo.CreateNoWindow = true;
                proc.StartInfo.WindowStyle = ProcessWindowStyle.Hidden;
                proc.StartInfo.Arguments = cfg_precompile.CommandLine;
                proc.StartInfo.FileName = SDKVersion.Dir + SDKVersion.CompilerDir + "uppcl.exe";
                proc.StartInfo.RedirectStandardOutput = true;
                proc.StartInfo.UseShellExecute = false;
                proc.OutputDataReceived += Precompile_OutputDataReceived;
                proc.EnableRaisingEvents = true;

                if (proc.Start())
                {
                    proc.BeginOutputReadLine();
                    proc.WaitForExit();

                    proc.OutputDataReceived -= Precompile_OutputDataReceived;
                    int ec = proc.ExitCode;

                    if (ec == 0)
                        precompile_success = true;

                }
            }

            if (!precompile_success)
            {
                e.Result = false;
            }
            else
            {
                // Compile
                using (System.Diagnostics.Process proc = new System.Diagnostics.Process())
                {
                    proc.StartInfo.CreateNoWindow = true;
                    proc.StartInfo.WindowStyle = ProcessWindowStyle.Hidden;
                    proc.StartInfo.Arguments = cfg_compile.CommandLine;
                    proc.StartInfo.FileName = SDKVersion.Dir + SDKVersion.CompilerDir + "uppcl.exe";
                    proc.StartInfo.RedirectStandardOutput = true;
                    proc.StartInfo.UseShellExecute = false;
                    proc.OutputDataReceived += Compile_OutputDataReceived;
                    proc.EnableRaisingEvents = true;
                    if (proc.Start())
                    {
                        proc.BeginOutputReadLine();
                        proc.WaitForExit();
                        proc.OutputDataReceived -= Compile_OutputDataReceived;
                        int ec = proc.ExitCode;

                        if (ec == 0)
                            e.Result = true;
                        else
                            e.Result = false;
                    }
                }
            }

            var worker_result = e.Result == null ? false : (bool)e.Result;
            resetEvent.Set(worker_result);
        }

        public bool Compile()
        {
            if (!worker.IsBusy)
            {
                resetEvent.Reset(false);
                worker.RunWorkerAsync();
                return true;
            }
            return false;
        }

        public async Task<bool> WaitForFinishedAsync()
        {
            return await resetEvent.WaitAsync();
        }

        private void Precompile_OutputDataReceived(object sender, DataReceivedEventArgs e)
        {
            string msg = e.Data;

            if(msg==null)
            {
                Interlocked.Exchange(ref precompile_stdcompleted, 1);
            }

            if (string.IsNullOrWhiteSpace(msg)) return;

            var evts = translator.Eval(msg, this);

            foreach (var evt in evts)
            {
                reportedSource.Raise(this, evt);
            }
        }

        private void Compile_OutputDataReceived(object sender, DataReceivedEventArgs e)
        {
            string msg = e.Data;

            if (msg == null)
            {
                Interlocked.Exchange(ref compile_stdcompleted, 1);
            }

            if (string.IsNullOrWhiteSpace(msg)) return;

            var evts = translator.Eval(msg, this);

            foreach (var evt in evts)
            {
                reportedSource.Raise(this, evt);
            }
        }

        public void Dispose()
        {
            worker.CancelAsync();
            worker.DoWork -= compiler_DoWork;
            worker.Dispose();
        }
    }
}
