using EnvDTE;
using Microsoft.VisualStudio;
using Microsoft.VisualStudio.Shell;
using Microsoft.VisualStudio.Shell.Interop;
using Microsoft.VisualStudio.VCProjectEngine;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;


namespace UXX
{
    public static class Helper
    {
        public static Project FindProject(_DTE vs, Predicate<Project> match)
        {
            ThreadHelper.ThrowIfNotOnUIThread();
            if (vs == null) throw new ArgumentNullException("vs");
            if (match == null) throw new ArgumentNullException("match");

            foreach (Project project in vs.Solution.Projects)
            {
                if (match(project))
                {
                    return project;
                }

                if (project.ProjectItems != null)
                {
                    Project child = FindProjectInternal(project.ProjectItems, match);
                    if (child != null)
                    {
                        return child;
                    }
                }
            }

            return null;
        }

        private static Project FindProjectInternal(ProjectItems items, Predicate<Project> match)
        {
            ThreadHelper.ThrowIfNotOnUIThread();
            foreach (ProjectItem item in items)
            {
                Project project = item.Object as Project;

                if (project == null && item.SubProject != null)
                {
                    project = item.SubProject as Project;
                }

                if (project != null && match(project))
                {
                    return project;
                }
                else if (item.ProjectItems != null)
                {
                    Project child = FindProjectInternal(item.ProjectItems, match);
                    if (child != null)
                    {
                        return child;
                    }
                }
                else if (project != null && project.ProjectItems != null)
                {
                    Project child = FindProjectInternal(project.ProjectItems, match);
                    if (child != null)
                    {
                        return child;
                    }
                }
            }

            return null;
        }

        public static Guid GetProjectGuid(this Project project, IServiceProvider serviceProvider)
        {
            ThreadHelper.ThrowIfNotOnUIThread();
            IVsSolution solution =
             (IVsSolution)serviceProvider.GetService(typeof(SVsSolution)) as
               IVsSolution;
            IVsHierarchy hierarchy;
            solution.GetProjectOfUniqueName(project.FullName, out hierarchy);
            if (hierarchy != null)
            {
                Guid projectGuid;

                ErrorHandler.ThrowOnFailure(
                  hierarchy.GetGuidProperty(
                  VSConstants.VSITEMID_ROOT,
                (int)__VSHPROPID.VSHPROPID_ProjectIDGuid,
                  out projectGuid));

                if (projectGuid != null)
                {
                    return projectGuid;
                }
            }

            return Guid.Empty;
        }

        private static VCProject InternalGetVCProject(ProjectItems items, Guid id, IServiceProvider serviceProvider)
        {
            ThreadHelper.ThrowIfNotOnUIThread();
            foreach (ProjectItem item in items)
            {
                Project pj = item.Object as Project;
                if (pj == null && item.SubProject != null)
                {
                    pj = item.SubProject as Project;
                }

                if (pj != null)
                {
                    if (pj.GetProjectGuid(serviceProvider) == id)
                    {
                        Object opj = pj.Object;
                        if (opj == null) return null;
                        return opj as VCProject;
                    }
                }

                if (item.ProjectItems != null)
                {
                    VCProject vcpj1 = InternalGetVCProject(item.ProjectItems, id, serviceProvider);
                    if (vcpj1 != null) return vcpj1;
                }
                else if (pj != null && pj.ProjectItems != null)
                {
                    VCProject vcpj2 = InternalGetVCProject(pj.ProjectItems, id, serviceProvider);
                    if (vcpj2 != null) return vcpj2;
                }
            }
            return null;
        }

        public static VCProject GetVCProject(this EnvDTE80.DTE2 dte, Guid id)
        {
            ThreadHelper.ThrowIfNotOnUIThread();
            System.IServiceProvider serviceProvider = new ServiceProvider(dte.DTE as Microsoft.VisualStudio.OLE.Interop.IServiceProvider);
            foreach (Project pj in dte.Solution.Projects)
            {
                if (pj.GetProjectGuid(serviceProvider) == id)
                {
                    Object opj = pj.Object;
                    if (opj == null) return null;
                    return opj as VCProject;
                }
                else
                {
                    VCProject vcpj = InternalGetVCProject(pj.ProjectItems, id, serviceProvider);
                    if (vcpj != null) return vcpj;
                }
            }
            return null;
        }


        public static bool IsFileUpdate(this string filename, DateTime time)
        {
            return System.IO.File.GetLastWriteTimeUtc(filename) > time;
        }

        public static IEnumerable<VCFile> GetAllVCFiles(this VCFilter col)
        {
            IVCCollection files = col.Files;
            foreach (VCFile vcfile in files)
            {
                yield return vcfile;
            }
            IVCCollection filters = col.Filters;
            foreach (VCFilter vcfilter in filters)
            {
                var sfiles = vcfilter.GetAllVCFiles();
                foreach (VCFile svcfile in sfiles)
                {
                    yield return svcfile;
                }
            }
            yield break;
        }

        public static IEnumerable<VCFile> GetAllVCFiles(this VCProject col)
        {
            IVCCollection files = col.Files;
            foreach (VCFile vcfile in files)
            {
                yield return vcfile;
            }
            IVCCollection filters = col.Filters;
            foreach (VCFilter vcfilter in filters)
            {
                var sfiles = vcfilter.GetAllVCFiles();
                foreach (VCFile svcfile in sfiles)
                {
                    yield return svcfile;
                }
            }
            yield break;
        }

        public static VCFile AddRelativeFile(this VCFilter root, string relativepath, string fullpath)
        {
            VCFilter cur = root;
            var dirs = relativepath.Split(new char[] { '\\' }, StringSplitOptions.RemoveEmptyEntries).ToList();
            if (dirs.Count>1)
            {
                for (int i=0; i<dirs.Count-1;i++)
                {
                    string idir = dirs[i];
                    if (cur != null)
                    {
                        VCFilter next = null;
                        IVCCollection subs = cur.Filters;
                        foreach (VCFilter sub in subs)
                        {
                            if (sub.Name == idir)
                            {
                                next = sub;
                                break;
                            }
                        }
                        if(next!=null)
                        {
                            next = cur.AddFilter(idir);
                            cur = next;
                        }
                    }
                }
            }

            return cur.AddFile(fullpath);
        }

        public static void AddVCTool(this VCFile vcfile, string config, string platform)
        {
            foreach (VCFileConfiguration ffs in (IVCCollection)vcfile.FileConfigurations)
            {
                VCConfiguration fvc = ffs.ProjectConfiguration as VCConfiguration;
                if (fvc != null)
                {
                    if (fvc.ConfigurationName == config)
                    {
                        VCPlatform fplat = fvc.Platform;
                        if (fplat != null)
                        {
                            if (fplat.Name == platform)
                            {
                                VCCLCompilerTool clt = (VCCLCompilerTool)fvc.Tools.Item("VCCLCompilerTool");
                                clt.ObjectFile = @"$(IntDir)%(RelativeDir)";
                            }
                        }
                    }
                }
            }
        }

        public static string FixDirPath(this string dir)
        {
            if (dir.EndsWith("\\"))
                return dir.Substring(0, dir.Length - 1);
            return dir;
        }
    }


    internal class ConcurrentDictionary<TKey, TValue>
    {
        private readonly IDictionary<TKey, TValue> _inner;

        public ConcurrentDictionary()
        {
            _inner = new Dictionary<TKey, TValue>();
        }

        public TValue GetOrAdd(TKey key, Func<TKey, TValue> valueFactory)
        {
            lock (_inner)
            {
                if (!_inner.TryGetValue(key, out var value))
                {
                    _inner[key] = value = valueFactory(key);
                }
                return value;
            }
        }
    }

    public class WeakEventSource<TEventArgs>
#if NET40 || NET35
        where TEventArgs : EventArgs
#endif
    {
        private readonly DelegateCollection _handlers;

        public WeakEventSource()
        {
            _handlers = new DelegateCollection();
        }

        public void Raise(object sender, TEventArgs e)
        {
            List<StrongHandler> validHandlers;
            lock (_handlers)
            {
                validHandlers = new List<StrongHandler>(_handlers.Count);
                for (int i = 0; i < _handlers.Count; i++)
                {
                    var weakHandler = _handlers[i];
                    if (weakHandler != null)
                    {
                        if (weakHandler.TryGetStrongHandler() is StrongHandler handler)
                            validHandlers.Add(handler);
                        else
                            _handlers.Invalidate(i);
                    }
                }

                _handlers.CollectDeleted();
            }

            foreach (var handler in validHandlers)
            {
                handler.Invoke(sender, e);
            }
        }

        public void Subscribe(EventHandler<TEventArgs> handler)
        {
            var singleHandlers = handler
                .GetInvocationList()
                .Cast<EventHandler<TEventArgs>>()
                .ToList();

            lock (_handlers)
            {
                foreach (var h in singleHandlers)
                    _handlers.Add(h);
            }
        }

        public void Unsubscribe(EventHandler<TEventArgs> handler)
        {
            var singleHandlers = handler
                .GetInvocationList()
                .Cast<EventHandler<TEventArgs>>();

            lock (_handlers)
            {
                foreach (var singleHandler in singleHandlers)
                {
                    _handlers.Remove(singleHandler);
                }

                _handlers.CollectDeleted();
            }
        }

        private delegate void OpenEventHandler(object target, object sender, TEventArgs e);

        private class WeakDelegate
        {
            #region Open handler generation and cache

            // ReSharper disable once StaticMemberInGenericType (by design)
            private static readonly ConcurrentDictionary<MethodInfo, OpenEventHandler> OpenHandlerCache =
                new ConcurrentDictionary<MethodInfo, OpenEventHandler>();

            private static OpenEventHandler CreateOpenHandler(MethodInfo method)
            {
                var target = System.Linq.Expressions.Expression.Parameter(typeof(object), "target");
                var sender = System.Linq.Expressions.Expression.Parameter(typeof(object), "sender");
                var e = System.Linq.Expressions.Expression.Parameter(typeof(TEventArgs), "e");

                if (method.IsStatic)
                {
                    var expr = System.Linq.Expressions.Expression.Lambda<OpenEventHandler>(
                        System.Linq.Expressions.Expression.Call(
                            method,
                            sender, e),
                        target, sender, e);
                    return expr.Compile();
                }
                else
                {
                    var expr = System.Linq.Expressions.Expression.Lambda<OpenEventHandler>(
                        System.Linq.Expressions.Expression.Call(
                            System.Linq.Expressions.Expression.Convert(target, method.DeclaringType),
                            method,
                            sender, e),
                        target, sender, e);
                    return expr.Compile();
                }
            }

            #endregion

            private readonly WeakReference _weakTarget;
            private readonly MethodInfo _method;
            private readonly OpenEventHandler _openHandler;

            public WeakDelegate(Delegate handler)
            {
                _weakTarget = handler.Target != null ? new WeakReference(handler.Target) : null;
                _method = handler.GetMethodInfo();
                _openHandler = OpenHandlerCache.GetOrAdd(_method, CreateOpenHandler);
            }

            public StrongHandler? TryGetStrongHandler()
            {
                object target = null;
                if (_weakTarget != null)
                {
                    target = _weakTarget.Target;
                    if (target == null)
                        return null;
                }

                return new StrongHandler(target, _openHandler);
            }

            public bool IsMatch(EventHandler<TEventArgs> handler)
            {
                return ReferenceEquals(handler.Target, _weakTarget?.Target)
                       && handler.GetMethodInfo().Equals(_method);
            }

            public static int GetHashCode(EventHandler<TEventArgs> handler)
            {
                var hashCode = -335093136;
                hashCode = hashCode * -1521134295 + (handler?.Target?.GetHashCode()).GetValueOrDefault();
                hashCode = hashCode * -1521134295 + (handler?.GetMethodInfo()?.GetHashCode()).GetValueOrDefault();
                return hashCode;
            }
        }

        private struct StrongHandler
        {
            private readonly object _target;
            private readonly OpenEventHandler _openHandler;

            public StrongHandler(object target, OpenEventHandler openHandler)
            {
                _target = target;
                _openHandler = openHandler;
            }

            public void Invoke(object sender, TEventArgs e)
            {
                _openHandler(_target, sender, e);
            }
        }

        private class DelegateCollection : IEnumerable<WeakDelegate>
        {
            private List<WeakDelegate> _delegates;

            private Dictionary<long, List<int>> _index;

            private int _deletedCount;

            public DelegateCollection()
            {
                _delegates = new List<WeakDelegate>();
                _index = new Dictionary<long, List<int>>();
            }

            public void Add(EventHandler<TEventArgs> singleHandler)
            {
                _delegates.Add(new WeakDelegate(singleHandler));
                var index = _delegates.Count - 1;
                AddToIndex(singleHandler, index);
            }

            public void Invalidate(int index)
            {
                _delegates[index] = null;
                _deletedCount++;
            }

            internal void Remove(EventHandler<TEventArgs> singleHandler)
            {
                var hashCode = WeakDelegate.GetHashCode(singleHandler);

                if (!_index.ContainsKey(hashCode))
                    return;

                var indices = _index[hashCode];
                for (int i = indices.Count - 1; i >= 0; i--)
                {
                    int index = indices[i];
                    if (_delegates[index] != null &&
                        _delegates[index].IsMatch(singleHandler))
                    {
                        _delegates[index] = null;
                        _deletedCount++;
                        indices.Remove(i);
                    }
                }

                if (indices.Count == 0)
                    _index.Remove(hashCode);
            }

            public void CollectDeleted()
            {
                if (_deletedCount < _delegates.Count / 4)
                    return;

                Dictionary<int, int> newIndices = new Dictionary<int, int>();
                var newDelegates = new List<WeakDelegate>();
                int oldIndex = 0;
                int newIndex = 0;
                foreach (var item in _delegates)
                {
                    if (item != null)
                    {
                        newDelegates.Add(item);
                        newIndices.Add(oldIndex, newIndex);
                        newIndex++;
                    }

                    oldIndex++;
                }

                _delegates = newDelegates;

                var hashCodes = _index.Keys.ToList();
                foreach (var hashCode in hashCodes)
                {
                    _index[hashCode] = _index[hashCode]
                        .Where(oi => newIndices.ContainsKey(oi))
                        .Select(oi => newIndices[oi]).ToList();
                }

                _deletedCount = 0;
            }

            private void AddToIndex(EventHandler<TEventArgs> singleHandler, int index)
            {
                var hashCode = WeakDelegate.GetHashCode(singleHandler);
                if (_index.ContainsKey(hashCode))
                    _index[hashCode].Add(index);
                else
                    _index.Add(hashCode, new List<int> { index });
            }

            public WeakDelegate this[int index] => _delegates[index];

            /// <summary>Returns an enumerator that iterates through the collection.</summary>
            /// <returns>A <see cref="T:System.Collections.Generic.IEnumerator`1" /> that can be used to iterate through the collection.</returns>
            public IEnumerator<WeakDelegate> GetEnumerator()
            {
                return _delegates.GetEnumerator();
            }

            /// <summary>Returns an enumerator that iterates through a collection.</summary>
            /// <returns>An <see cref="T:System.Collections.IEnumerator" /> object that can be used to iterate through the collection.</returns>
            System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
            {
                return GetEnumerator();
            }

            public int Count => _delegates.Count;
        }
    }

    public delegate System.Threading.Tasks.Task AsyncEventHandler<TEventArgs>(object sender, TEventArgs e);

    public class AsyncWeakEventSource<TEventArgs>
    {
        private readonly DelegateCollection _handlers;

        public AsyncWeakEventSource()
        {
            _handlers = new DelegateCollection();
        }

        public async System.Threading.Tasks.Task RaiseAsync(object sender, TEventArgs e)
        {
            List<StrongHandler> validHandlers;
            lock (_handlers)
            {
                validHandlers = new List<StrongHandler>(_handlers.Count);
                for (int i = 0; i < _handlers.Count; i++)
                {
                    var weakHandler = _handlers[i];
                    if (weakHandler != null)
                    {
                        if (weakHandler.TryGetStrongHandler() is StrongHandler handler)
                            validHandlers.Add(handler);
                        else
                            _handlers.Invalidate(i);
                    }
                }

                _handlers.CollectDeleted();
            }

            foreach (var handler in validHandlers)
            {
                await handler.Invoke(sender, e);
            }
        }

        public void Subscribe(AsyncEventHandler<TEventArgs> handler)
        {
            var singleHandlers = handler
                .GetInvocationList()
                .Cast<AsyncEventHandler<TEventArgs>>()
                .ToList();

            lock (_handlers)
            {
                foreach (var h in singleHandlers)
                    _handlers.Add(h);
            }
        }

        public void Unsubscribe(AsyncEventHandler<TEventArgs> handler)
        {
            var singleHandlers = handler
                .GetInvocationList()
                .Cast<AsyncEventHandler<TEventArgs>>();

            lock (_handlers)
            {
                foreach (var singleHandler in singleHandlers)
                {
                    _handlers.Remove(singleHandler);
                }

                _handlers.CollectDeleted();
            }
        }

        private delegate System.Threading.Tasks.Task OpenEventHandler(object target, object sender, TEventArgs e);

        private class WeakDelegate
        {
            #region Open handler generation and cache

            // ReSharper disable once StaticMemberInGenericType (by design)
            private static readonly ConcurrentDictionary<MethodInfo, OpenEventHandler> OpenHandlerCache =
                new ConcurrentDictionary<MethodInfo, OpenEventHandler>();

            private static OpenEventHandler CreateOpenHandler(MethodInfo method)
            {
                var target = System.Linq.Expressions.Expression.Parameter(typeof(object), "target");
                var sender = System.Linq.Expressions.Expression.Parameter(typeof(object), "sender");
                var e = System.Linq.Expressions.Expression.Parameter(typeof(TEventArgs), "e");

                if (method.IsStatic)
                {
                    var expr = System.Linq.Expressions.Expression.Lambda<OpenEventHandler>(
                        System.Linq.Expressions.Expression.Call(
                            method,
                            sender, e),
                        target, sender, e);
                    return expr.Compile();
                }
                else
                {
                    var expr = System.Linq.Expressions.Expression.Lambda<OpenEventHandler>(
                        System.Linq.Expressions.Expression.Call(
                            System.Linq.Expressions.Expression.Convert(target, method.DeclaringType),
                            method,
                            sender, e),
                        target, sender, e);
                    return expr.Compile();
                }
            }

            #endregion

            private readonly WeakReference _weakTarget;
            private readonly MethodInfo _method;
            private readonly OpenEventHandler _openHandler;

            public WeakDelegate(Delegate handler)
            {
                _weakTarget = handler.Target != null ? new WeakReference(handler.Target) : null;
                _method = handler.GetMethodInfo();
                _openHandler = OpenHandlerCache.GetOrAdd(_method, CreateOpenHandler);
            }

            public StrongHandler? TryGetStrongHandler()
            {
                object target = null;
                if (_weakTarget != null)
                {
                    target = _weakTarget.Target;
                    if (target == null)
                        return null;
                }

                return new StrongHandler(target, _openHandler);
            }

            public bool IsMatch(AsyncEventHandler<TEventArgs> handler)
            {
                return ReferenceEquals(handler.Target, _weakTarget?.Target)
                       && handler.GetMethodInfo().Equals(_method);
            }

            public static int GetHashCode(AsyncEventHandler<TEventArgs> handler)
            {
                var hashCode = -335093136;
                hashCode = hashCode * -1521134295 + (handler?.Target?.GetHashCode()).GetValueOrDefault();
                hashCode = hashCode * -1521134295 + (handler?.GetMethodInfo()?.GetHashCode()).GetValueOrDefault();
                return hashCode;
            }
        }

        private struct StrongHandler
        {
            private readonly object _target;
            private readonly OpenEventHandler _openHandler;

            public StrongHandler(object target, OpenEventHandler openHandler)
            {
                _target = target;
                _openHandler = openHandler;
            }

            public System.Threading.Tasks.Task Invoke(object sender, TEventArgs e)
            {
                return _openHandler(_target, sender, e);
            }
        }

        private class DelegateCollection : IEnumerable<WeakDelegate>
        {
            private List<WeakDelegate> _delegates;

            private Dictionary<long, List<int>> _index;

            private int _deletedCount;

            public DelegateCollection()
            {
                _delegates = new List<WeakDelegate>();
                _index = new Dictionary<long, List<int>>();
            }

            public void Add(AsyncEventHandler<TEventArgs> singleHandler)
            {
                _delegates.Add(new WeakDelegate(singleHandler));
                var index = _delegates.Count - 1;
                AddToIndex(singleHandler, index);
            }

            public void Invalidate(int index)
            {
                _delegates[index] = null;
                _deletedCount++;
            }

            internal void Remove(AsyncEventHandler<TEventArgs> singleHandler)
            {
                var hashCode = WeakDelegate.GetHashCode(singleHandler);

                if (!_index.ContainsKey(hashCode))
                    return;

                var indices = _index[hashCode];
                for (int i = indices.Count - 1; i >= 0; i--)
                {
                    int index = indices[i];
                    if (_delegates[index] != null &&
                        _delegates[index].IsMatch(singleHandler))
                    {
                        _delegates[index] = null;
                        _deletedCount++;
                        indices.Remove(i);
                    }
                }

                if (indices.Count == 0)
                    _index.Remove(hashCode);
            }

            public void CollectDeleted()
            {
                if (_deletedCount < _delegates.Count / 4)
                    return;

                Dictionary<int, int> newIndices = new Dictionary<int, int>();
                var newDelegates = new List<WeakDelegate>();
                int oldIndex = 0;
                int newIndex = 0;
                foreach (var item in _delegates)
                {
                    if (item != null)
                    {
                        newDelegates.Add(item);
                        newIndices.Add(oldIndex, newIndex);
                        newIndex++;
                    }

                    oldIndex++;
                }

                _delegates = newDelegates;

                var hashCodes = _index.Keys.ToList();
                foreach (var hashCode in hashCodes)
                {
                    _index[hashCode] = _index[hashCode]
                        .Where(oi => newIndices.ContainsKey(oi))
                        .Select(oi => newIndices[oi]).ToList();
                }

                _deletedCount = 0;
            }

            private void AddToIndex(AsyncEventHandler<TEventArgs> singleHandler, int index)
            {
                var hashCode = WeakDelegate.GetHashCode(singleHandler);
                if (_index.ContainsKey(hashCode))
                    _index[hashCode].Add(index);
                else
                    _index.Add(hashCode, new List<int> { index });
            }

            public WeakDelegate this[int index] => _delegates[index];

            /// <summary>Returns an enumerator that iterates through the collection.</summary>
            /// <returns>A <see cref="T:System.Collections.Generic.IEnumerator`1" /> that can be used to iterate through the collection.</returns>
            public IEnumerator<WeakDelegate> GetEnumerator()
            {
                return _delegates.GetEnumerator();
            }

            /// <summary>Returns an enumerator that iterates through a collection.</summary>
            /// <returns>An <see cref="T:System.Collections.IEnumerator" /> object that can be used to iterate through the collection.</returns>
            System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
            {
                return GetEnumerator();
            }

            public int Count => _delegates.Count;
        }
    }

}
