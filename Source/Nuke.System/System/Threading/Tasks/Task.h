
#pragma once
#include <System/TimeSpan.h>
#include <future>


namespace Nuke::System::Threading::Tasks
{
    template <typename TResult>
    class Task
    {
    public:
        Task(Task&&) = default;
        ~Task() = default;

    public:
        TResult Await()
        {
            _future.wait();
            return _future.get();
        }

        TResult Await(TimeSpan timeout)
        {
            std::chrono::milliseconds timeoutDuration(timeout.TotalMilliseconds());
            return _future.wait_for(timeoutDuration);
        }

        template <class FuncType, class... ArgTypes>
        using AsyncInvoker = Task<std::invoke_result_t<std::decay_t<FuncType>, std::decay_t<ArgTypes>...>>(*)(FuncType&& func, ArgTypes&&... args);

    private:
        Task() = default;
        Task(const Task&) = default;
        friend class TaskFactory;
        std::future<TResult> _future;
    };

    template <typename TResult>
    class PackResult
    {
    public:
        TResult Result;
        std::exception Exception;
    };

    // 静态任务派发
    class TaskFactory
    {
    public:
        template <class FuncType, class... ArgTypes>
        static Task<std::invoke_result_t<std::decay_t<FuncType>, std::decay_t<ArgTypes>...>>
            RunAsync(FuncType&& func, ArgTypes&&... args)
        {
            using _ReturnType = std::invoke_result_t<std::decay_t<FuncType>, std::decay_t<ArgTypes>...>;
            auto future = std::async(std::launch::async, std::forward<FuncType>(func), std::forward<ArgTypes>(args)...);
            Task<_ReturnType> task;
            task._future = std::move(future);
            return task;
        }

        template <typename Func, typename... Args>
        static auto RunAsyncWithExceptionCapture(Func&& func, Args&&... args) -> Task<PackResult<decltype(func(args...))>>
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
                    packResult.Exception = std::exception();
                    return packResult;
                }
            };
            auto task = TaskFactory::RunAsync(packLambda);
            return task;
        }
    };


}
