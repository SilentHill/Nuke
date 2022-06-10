
#include "_pch.h"
#include "Timer.h"

namespace Nuke::System::Timers
{

    class Timer::TimerImpl
    {
    public:
        TimerImpl(Timer& parent)
            :_parent(parent)
        {

        }
        ~TimerImpl()
        {

        }
        void Start()
        {
            if (!_needStop)
            {
                return;
            }
            _needStop = false;
            _timerFuture = std::async(std::launch::async, [this, intervalCopy = _interval]()
                {
                    while (!_needStop)
                    {
                        std::unique_lock<std::mutex> lock(_waitMutex);
                        const auto status = _conditionVariable.wait_for(lock, std::chrono::milliseconds(static_cast<int64_t>(intervalCopy)));
                        if (status == std::cv_status::timeout)
                        {
                            if (GetEnabled())
                            {
                                _parent.Elapsed.Raise({ DateTime::Now() });
                            }
                        }
                    }
                });
        }

        void Stop()
        {
            if (_needStop)
            {
                return;
            }

            {
                std::unique_lock<std::mutex> lock(_waitMutex);
                _needStop = true;
            }
            _conditionVariable.notify_one();
            _timerFuture.wait();
        }
        void SetAutoReset(bool autoReset)
        {
            _autoReset = autoReset;
        }

        bool GetAutoReset()
        {
            return _autoReset;
        }

        void SetEnabled(bool enabled)
        {
            _enabled = enabled;
        }

        bool GetEnabled()
        {
            return _enabled;
        }

        void SetInterval(double interval)
        {
            _interval = interval;
        }

        double GetInterval()
        {
            return _interval;
        }
    private:
        friend class Timer;
        Timer& _parent;
        bool _autoReset = false;
        bool _enabled = true;
        double _interval = 0.0;
        std::unique_lock<std::mutex> _waitLock;
        std::mutex _waitMutex;
        std::condition_variable _conditionVariable;
        std::future<void> _timerFuture;
        bool _needStop = true;
    };

    Timer::Timer()
    {
        impl = std::make_unique<Timer::TimerImpl>(*this);
    }

    Timer::~Timer()
    {

    }

    void Timer::Start()
    {
        impl->Start();
    }

    void Timer::Stop()
    {
        impl->Stop();
    }

    void Timer::SetAutoReset(bool autoReset)
    {
        impl->SetAutoReset(autoReset);
    }

    bool Timer::GetAutoReset()
    {
        return impl->GetAutoReset();
    }

    void Timer::SetEnabled(bool enabled)
    {
        impl->SetEnabled(enabled);
    }

    bool Timer::GetEnabled()
    {
        return impl->GetEnabled();
    }

    void Timer::SetInterval(double interval)
    {
        impl->SetInterval(interval);
    }

    double Timer::GetInterval()
    {
        return impl->GetInterval();
    }
}