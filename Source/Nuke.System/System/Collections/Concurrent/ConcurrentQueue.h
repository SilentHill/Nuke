
#pragma once

namespace Nuke::System::Collections::Concurrent
{
    template <typename T> 
    class ConcurrentQueue
    {
        std::queue<T> _queue;
        std::mutex _mutex;
        std::condition_variable _conditionVariable;

    public:
        void Enqueue(const T& val)
        {
            std::lock_guard<std::mutex> lock(_mutex);
            bool canNotify = _queue.empty();
            _queue.push(val);
            if (canNotify)
            {
                _conditionVariable.notify_one();
            }
        }

        T Dequeue()
        {
            std::unique_lock<std::mutex> uniqueLock(_mutex);
            while (_queue.empty())
            {
                _conditionVariable.wait(uniqueLock);
            }
            T retval = _queue.front();
            _queue.pop();
            return retval;
        }
    };
}