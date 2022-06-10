
#pragma once

#include "_pch.h"
#include "System/Event.h"
#include "System/DateTime.h"


namespace Nuke::System::Timers
{
	class Timer final
	{
	public: // 构造析构
		Timer();
		Timer(const Timer&) = delete;
		Timer(Timer&&) = delete;
		~Timer();

	public: // 行为方法
		void Start();
		void Stop();

	public: // 属性方法
		void SetAutoReset(bool autoReset);
		bool GetAutoReset();
		void SetEnabled(bool enabled);
		bool GetEnabled();
		void SetInterval(double interval);
		double GetInterval();

	public: // 事件

		// Elapsed: 达到间隔时发生
		struct ElapsedEventArgs
		{
		public:
			DateTime SignalTime;
		};
		Nuke::System::Event<ElapsedEventArgs> Elapsed;

	private: // 私有
		class TimerImpl;
		std::unique_ptr<TimerImpl> impl;
	};
}