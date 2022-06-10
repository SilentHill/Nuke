
#pragma once

#include "_pch.h"
#include <System/TimeSpan.h>

namespace Nuke::System::Diagnostics
{
	//
	// 秒表
	// 
	
	class Stopwatch
	{
	public:	// 构造析构
		Stopwatch();
		Stopwatch(const Stopwatch&) = delete;
		Stopwatch(Stopwatch&&) = default;
		~Stopwatch();

	public:	// 全局静态值
		static const int64_t Frequency;		// 获取以每秒刻度数表示的计时器频率
		static const bool IsHighResolution;	// 指示计时器是否基于高精度性能计数器

	public: // 属性方法
		TimeSpan Elapsed();
		int64_t ElapsedMilliseconds();
		int64_t ElapsedTicks();
		bool IsRunning();

	public: // 静态方法
		static int64_t GetTimestamp();
		static Stopwatch StartNew();

	public: // 成员方法
		void Reset(); 
		void Restart();
		void Start();
		void Stop();

	private: // 私有
		class StopwatchImpl;
		std::unique_ptr<StopwatchImpl> impl;

	};

}