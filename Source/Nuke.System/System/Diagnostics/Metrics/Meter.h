
#pragma once

#include "Instrument.h"

namespace Nuke::System::Diagnostics::Metrics
{
	class Meter
	{
	public:
		Meter(const std::string name);
		
	public:
		std::string Name();
		std::string Version();

	private:
		class MeterImpl;
		std::unique_ptr<MeterImpl> impl;
	};

}