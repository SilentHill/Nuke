
#include "Stopwatch.h"
#include "CrossPlatform/MiscApi.h"

namespace Nuke::System::Diagnostics
{
    using namespace Nuke::CrossPlatform;

    const int64_t Stopwatch::Frequency = MiscApi::Crossplatform_QueryPerformanceFrequency();
    const bool Stopwatch::IsHighResolution = true;

    namespace StopwatchConstants
    {
        const int64_t TicksPerMillisecond = 10000;
        const int64_t TicksPerSecond = TicksPerMillisecond * 1000;
        const double s_tickFrequency = (double)TicksPerSecond / Stopwatch::Frequency;
    }

    class Stopwatch::StopwatchImpl
    {
    public:
        StopwatchImpl() = default;
        ~StopwatchImpl() = default;

    private:
        void Start()
        {
            if (!_isRunning)
            {
                _startTimeStamp = GetTimestamp();
                _isRunning = true;
            }
        }

        void Stop()
        {
            if (_isRunning)
            {
                int64_t endTimeStamp = GetTimestamp();
                int64_t elapsedThisPeriod = endTimeStamp - _startTimeStamp;
                _elapsed += elapsedThisPeriod;
                _isRunning = false;
                if (_elapsed < 0)
                {
                    // 某些BIOS或HAL由于有bug，会对变速CPU会获得负值，这里做保护
                    _elapsed = 0;
                }
            }
        }

        void Reset()
        {
            _elapsed = 0;
            _isRunning = false;
            _startTimeStamp = 0;
        }

        void Restart()
        {
            _elapsed = 0;
            _startTimeStamp = GetTimestamp();
            _isRunning = true;
        }

        bool IsRunning()
        {
            return _isRunning;
        }

        TimeSpan Elapsed()
        {
            return TimeSpan(GetElapsedDateTimeTicks());
        }

        int64_t ElapsedMilliseconds()
        {
            return GetElapsedDateTimeTicks() / StopwatchConstants::TicksPerMillisecond;
        }

        int64_t ElapsedTicks()
        {
            return GetRawElapsedTicks();
        }

        static int64_t GetTimestamp()
        {
            return MiscApi::Crossplatform_QueryPerformanceCounter();
        }

        int64_t GetRawElapsedTicks()
        {
            int64_t timeElapsed = _elapsed;

            if (_isRunning)
            {
                int64_t currentTimeStamp = GetTimestamp();
                int64_t elapsedUntilNow = currentTimeStamp - _startTimeStamp;
                timeElapsed += elapsedUntilNow;
            }
            return timeElapsed;
        }

        int64_t GetElapsedDateTimeTicks()
        {
            return (long)(GetRawElapsedTicks() * StopwatchConstants::s_tickFrequency);
        }

    private:
        friend class Stopwatch;
        int64_t _elapsed;
        int64_t _startTimeStamp;
        bool _isRunning;
    };


    Stopwatch::Stopwatch()
    {
        impl = std::make_unique<StopwatchImpl>();
        Reset();
    }

    Stopwatch::~Stopwatch()
    {

    }

    void Stopwatch::Reset()
    {
        impl->Reset();
    }

    void Stopwatch::Restart()
    {
        impl->Restart();
    }

    void Stopwatch::Start()
    {
        impl->Start();
    }

    void Stopwatch::Stop()
    {
        impl->Stop();
    }

    Stopwatch StartNew()
    {
        Stopwatch s;
        s.Start();
        return s;
    }

    int64_t Stopwatch::GetTimestamp()
    {
        return StopwatchImpl::GetTimestamp();
    }

    bool Stopwatch::IsRunning()
    {
        return impl->IsRunning();
    }

    TimeSpan Stopwatch::Elapsed()
    {
        return impl->Elapsed();
    }

    int64_t Stopwatch::ElapsedMilliseconds()
    {
        return impl->ElapsedMilliseconds();
    }

    int64_t Stopwatch::ElapsedTicks()
    {
        return impl->ElapsedTicks();
    }
}