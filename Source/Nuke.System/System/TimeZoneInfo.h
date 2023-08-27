
#pragma once

#include <System/DateTime.h>

namespace Nuke::System
{
	class TimeZoneInfo
	{
	public:

	private:
		TimeZoneInfo();
		class TimeZoneInfoInternals;
		TimeZoneInfoInternals* internals;
	};
}