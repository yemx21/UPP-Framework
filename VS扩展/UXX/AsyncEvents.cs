using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.ExceptionServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace UXX
{
    public static class TaskCompletionSourceExtensions
    {

        public static TaskCompletionSource<TResult> CreateAsyncTaskSource<TResult>()
        {
            return new TaskCompletionSource<TResult>(TaskCreationOptions.RunContinuationsAsynchronously);
        }

        public static TaskCompletionSource<TResult> CreateAsyncTaskSource<TResult>(TResult val)
        {
            return new TaskCompletionSource<TResult>(val, TaskCreationOptions.RunContinuationsAsynchronously);
        }
    }

    public sealed class CancellationTokenTaskSource<T> : IDisposable
    {
        private readonly IDisposable _registration;

        public CancellationTokenTaskSource(CancellationToken cancellationToken)
        {
            if (cancellationToken.IsCancellationRequested)
            {
                Task = System.Threading.Tasks.Task.FromCanceled<T>(cancellationToken);
                return;
            }
            var tcs = new TaskCompletionSource<T>();
            _registration = cancellationToken.Register(() => tcs.TrySetCanceled(cancellationToken), useSynchronizationContext: false);
            Task = tcs.Task;
        }

        public Task<T> Task { get; private set; }

        public void Dispose()
        {
            _registration?.Dispose();
        }
    }

    internal static class IdManager<TTag>
    {
        private static int _lastId;

        public static int GetId(ref int id)
        {
            if (id != 0)
                return id;

            int newId;

            do
            {
                newId = Interlocked.Increment(ref _lastId);
            } while (newId == 0);

            Interlocked.CompareExchange(ref id, newId, 0);

            return id;
        }
    }

    public static class TaskExtensions
    {
        public static Task WaitAsync(this Task @this, CancellationToken cancellationToken)
        {
            if (@this == null)
                throw new ArgumentNullException(nameof(@this));

            if (!cancellationToken.CanBeCanceled)
                return @this;
            if (cancellationToken.IsCancellationRequested)
                return Task.FromCanceled(cancellationToken);
            return DoWaitAsync(@this, cancellationToken);
        }

        private static async Task DoWaitAsync(Task task, CancellationToken cancellationToken)
        {
            using (var cancelTaskSource = new CancellationTokenTaskSource<object>(cancellationToken))
                await await Task.WhenAny(task, cancelTaskSource.Task).ConfigureAwait(false);
        }

        public static void WaitAndUnwrapException(this Task task)
        {
            if (task == null)
                throw new ArgumentNullException(nameof(task));
            task.GetAwaiter().GetResult();
        }

        public static Task<TResult> WaitAsync<TResult>(this Task<TResult> @this, CancellationToken cancellationToken)
        {
            if (@this == null)
                throw new ArgumentNullException(nameof(@this));

            if (!cancellationToken.CanBeCanceled)
                return @this;
            if (cancellationToken.IsCancellationRequested)
                return Task.FromCanceled<TResult>(cancellationToken);
            return DoWaitAsync(@this, cancellationToken);
        }

        private static async Task<TResult> DoWaitAsync<TResult>(Task<TResult> task, CancellationToken cancellationToken)
        {
            using (var cancelTaskSource = new CancellationTokenTaskSource<TResult>(cancellationToken))
                return await await Task.WhenAny(task, cancelTaskSource.Task).ConfigureAwait(false);
        }

        public static Exception PrepareForRethrow(Exception exception)
        {
            ExceptionDispatchInfo.Capture(exception).Throw();
            return exception;
        }

        public static void WaitAndUnwrapException(this Task task, CancellationToken cancellationToken)
        {
            if (task == null)
                throw new ArgumentNullException(nameof(task));
            try
            {
                task.Wait(cancellationToken);
            }
            catch (AggregateException ex)
            {
                throw PrepareForRethrow(ex.InnerException);
            }
        }

        public static TResult WaitAndUnwrapException<TResult>(this Task<TResult> task)
        {
            if (task == null)
                throw new ArgumentNullException(nameof(task));
            return task.GetAwaiter().GetResult();
        }

        public static TResult WaitAndUnwrapException<TResult>(this Task<TResult> task, CancellationToken cancellationToken)
        {
            if (task == null)
                throw new ArgumentNullException(nameof(task));
            try
            {
                task.Wait(cancellationToken);
                return task.Result;
            }
            catch (AggregateException ex)
            {
                throw PrepareForRethrow(ex.InnerException);
            }
        }

    }

    public sealed class AsyncManualResetEvent<T>
    {
        private readonly object _mutex;
        private TaskCompletionSource<T> _tcs;

        private int _id;

        public AsyncManualResetEvent(bool set, T init)
        {
            _mutex = new object();
            _tcs = TaskCompletionSourceExtensions.CreateAsyncTaskSource<T>();
            if (set)
                _tcs.TrySetResult(init);
        }

        public AsyncManualResetEvent()
            : this(false, default(T))
        {
        }

        public int Id
        {
            get { return IdManager<AsyncManualResetEvent<T>>.GetId(ref _id); }
        }

        public bool IsSet
        {
            get { lock (_mutex) return _tcs.Task.IsCompleted; }
        }

        public Task<T> WaitAsync()
        {
            lock (_mutex)
            {
                return _tcs.Task;
            }
        }

        public Task<T> WaitAsync(CancellationToken cancellationToken)
        {
            var waitTask = WaitAsync();
            if (waitTask.IsCompleted)
                return waitTask;
            return waitTask.WaitAsync(cancellationToken);
        }

        public T Wait()
        {
            return WaitAsync().WaitAndUnwrapException();
        }

        public T Wait(CancellationToken cancellationToken)
        {
            var ret = WaitAsync();
            if (ret.IsCompleted)
                return ret.Result;
            return ret.WaitAndUnwrapException(cancellationToken);
        }

        public void Set(T val)
        {
            lock (_mutex)
            {
                _tcs.TrySetResult(val);
            }
        }

        public void Reset(T val)
        {
            lock (_mutex)
            {
                if (_tcs.Task.IsCompleted)
                    _tcs = TaskCompletionSourceExtensions.CreateAsyncTaskSource<T>(val);
            }
        }
    }

}
