
#pragma once

#include <type_traits>

namespace Nuke::System
{
	class Math
	{
	public:
		template <typename NumericType>
		static NumericType Min(NumericType n1, NumericType n2)
		{
			return (n1 <= n2) ? n1 : n2;
		}
		static double Round(double a);
		static float Round(float a);
	private:
		Math() = delete;
		Math(const Math& math) = delete;
		~Math() = delete;
	};
}