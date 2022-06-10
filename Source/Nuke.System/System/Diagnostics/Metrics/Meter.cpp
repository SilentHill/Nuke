
#include "_pch.h"
#include "Meter.h"

namespace Nuke::System::Diagnostics::Metrics
{
	class Meter::MeterImpl
	{
	public:
		MeterImpl() = default;
		~MeterImpl() = default;

	private:
		friend class Meter;
		std::string _name;

		std::vector<Instrument> _instruments;
	};

	Meter::Meter(const std::string name)
	{
		impl = std::make_unique<MeterImpl>();
		impl->_name = name;
	}
	std::string Meter::Name()
	{
		return impl->_name;
	}
	std::string Meter::Version()
	{
		return "";
	}
}