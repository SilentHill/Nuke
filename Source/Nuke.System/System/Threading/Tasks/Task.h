
#pragma once


namespace Nuke::System::Threading::Tasks
{
    enum class TaskCreationOptions
    {
        None = 0x00,
        PreferFairness = 0x01,
        LongRunning = 0x02,
        AttachedToParent = 0x04,
        DenyChildAttach = 0x08,
        HideScheduler = 0x10,
        RunContinuationsAsynchronously = 0x40
    };

    enum class TaskContinuationOptions
    {
        None = 0,
        PreferFairness = 0x01,
        LongRunning = 0x02,
        AttachedToParent = 0x04,
        DenyChildAttach = 0x08,
        HideScheduler = 0x10,
        LazyCancellation = 0x20,
        RunContinuationsAsynchronously = 0x40,
        NotOnRanToCompletion = 0x10000,
        NotOnFaulted = 0x20000,
        NotOnCanceled = 0x40000,
        OnlyOnRanToCompletion = NotOnFaulted | NotOnCanceled,
        OnlyOnFaulted = NotOnRanToCompletion | NotOnCanceled,
        OnlyOnCanceled = NotOnRanToCompletion | NotOnFaulted,
        ExecuteSynchronously = 0x80000
    };

    class CancellationToken
    {
        // 对stop token的封装
    };

    template <typename TResult>
    class Task
    {
    public:
        TResult GetResult()
        {
            return _future.get();
        }

        template <typename FuncType, class... ArgTypes>
        Task ContinueWith(FuncType&& func, ArgTypes&&... args)
        {
            auto future = _future.then(std::forward<FuncType>(func), std::forward<ArgTypes>(args)...);
            auto sharedFuture = future.share();
            Task task;
            task._future = sharedFuture;
            return task;
        }

    private:
        std::shared_future<TResult> _future;
    };

    class TaskScheduler
    {

    };
    enum class TaskStatus
    {
        Created,
        WaitingForActivation,
        WaitingToRun,
        Running,
        WaitingForChildrenToComplete,
        RanToCompletion,
        Canceled,
        Faulted
    };

    class PackResult
    {
        PackResult()
        {
        }

        std::exception_ptr FullException;
    };

    class Tasks
    {
    public:
        template <typename FuncType, class... ArgTypes>
        static auto Run(CancellationToken* cancellationToken, FuncType&& func, ArgTypes&&... args)
        {
            auto future = std::async(std::forward<FuncType>(func), std::forward<ArgTypes>(args)...);
            auto sharedFuture = future.share();
            using returnType = std::invoke_result<std::decay<FuncType>, std::decay<ArgTypes>...>;
            Task<returnType> task;
            return task;
        }
    };

}
