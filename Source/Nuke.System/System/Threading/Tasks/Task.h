
#pragma once
#include <System/TimeSpan.h>
#include <future>


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

    class ErrorPack
    {
        int32_t ErrorNumber;
        std::exception_ptr Exception;
        std::unique_ptr<ErrorPack> PrevError;
    };

    class IStopTrigger
    {
    public:
        bool IsTriggered();
    };

    template <typename TResult>
    class Task
    {
    public:
        friend class TaskFactory;
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

        IStopTrigger* stopTrigger;

    private:
        std::shared_future<TResult> _future;
    };

    class TaskScheduler
    {

    };
    enum class TaskStatus
    {
        Created0,
        WaitingForActivation,
        WaitingToRun,
        Running,
        WaitingForChildrenToComplete,
        RanToCompletion,
        Canceled,
        Faulted
    };


    class TimeoutTrigger
    {
    public:
        bool IsTriggered()
        {
            // return if timer is triggered
            return true;
        }
    };

    // 静态任务派发
    class TaskFactory
    {
    public:
        template <class FuncType, class... ArgTypes>
        static Task<std::invoke_result_t<std::decay_t<FuncType>, std::decay_t<ArgTypes>...>>
        RunAsync(TimeSpan timeoutSpan, FuncType&& func, ArgTypes&&... args)
        {
            using _ReturnType = std::invoke_result_t<std::decay_t<FuncType>, std::decay_t<ArgTypes>...>;
            auto future = std::async(std::forward<FuncType>(func), std::forward<ArgTypes>(args)...);
            auto sharedFuture = future.share();
            Task<_ReturnType> task;
            task._future = sharedFuture;
            return task;
        }
    };

    template <typename TResult>
    class PackResult
    {
    public:
        TResult Result;
        std::exception Exception;
    };

    template <typename Func, typename... Args>
    auto RunAsyncWithException(Func&& func, Args&&... args) -> Task<PackResult<decltype(func(args...))>>
    {
        // 返回值包装类型
        using PackResultType = PackResult<decltype(func(args...))>;

        // 这里将原始函数进行异常打包
        auto packLambda = [func = std::forward<Func>(func), args = std::make_tuple(std::forward<Args>(args)...)]() -> PackResultType
        {
            // 返回值
            PackResultType packResult;
            try
            {
                packResult.Result = std::apply(func, args);
                return packResult;
            }
            catch (const std::exception& e)
            {
                packResult.Exception = e;
                return packResult;
            }
            catch (...)
            {
                packResult.Exception = std::exception("unknown exception");
                return packResult;
            }
        };
        return TaskFactory::RunAsync(std::launch::async, packLambda);
    }
    // // 原始同步函数
    // int GetData()
    // {
    //     return 0;
    // }
    // 
    // // 同步函数转异步
    // Task<int> GetDataAsync()
    // {
    //     auto task = TaskFactory::Run(TimeSpan(1000),
    //     []()
    //     {
    //         return GetData();
    //     });
    //     return task;
    // }

}
