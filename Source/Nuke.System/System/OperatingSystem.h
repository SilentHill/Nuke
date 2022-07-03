
#pragma once

namespace Nuke::System
{
	class OperatingSystem
	{
    public:
		static bool IsWindows();
		static bool IsMacOS();
		static bool IsLinux();
	};
}