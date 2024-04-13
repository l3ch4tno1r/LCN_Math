#pragma once

#include <ratio>

#include "Units.h"

namespace LCN::Units
{
#pragma region Metric

	class Distance;

	template<typename _Type> using KiloMeter  = Unit<Distance, _Type, std::kilo>;
	template<typename _Type> using HectoMeter = Unit<Distance, _Type, std::hecto>;
	template<typename _Type> using DecaMeter  = Unit<Distance, _Type, std::deca>;
	template<typename _Type> using Meter      = Unit<Distance, _Type, std::ratio<1>>;
	template<typename _Type> using DeciMeter  = Unit<Distance, _Type, std::deci>;
	template<typename _Type> using CentiMeter = Unit<Distance, _Type, std::centi>;
	template<typename _Type> using MilliMeter = Unit<Distance, _Type, std::milli>;
	template<typename _Type> using MicroMeter = Unit<Distance, _Type, std::micro>;

	using KiloMeterLL  = KiloMeter<long long>;
	using HectoMeterLL = HectoMeter<long long>;
	using DecaMeterLL  = DecaMeter<long long>;
	using MeterLL      = Meter<long long>;
	using DeciMeterLL  = DeciMeter<long long>;
	using CentiMeterLL = CentiMeter<long long>;
	using MilliMeterLL = MilliMeter<long long>;
	using MicroMeterLL = MicroMeter<long long>;

	using KiloMeterLD  = KiloMeter<long double>;
	using HectoMeterLD = HectoMeter<long double>;
	using DecaMeterLD  = DecaMeter<long double>;
	using MeterLD      = Meter<long double>;
	using DeciMeterLD  = DeciMeter<long double>;
	using CentiMeterLD = CentiMeter<long double>;
	using MilliMeterLD = MilliMeter<long double>;
	using MicroMeterLD = MicroMeter<long double>;

	namespace Literals
	{
		constexpr KiloMeterLL operator"" km(unsigned long long value) noexcept { return KiloMeterLL{ value }; }

		constexpr HectoMeterLL operator"" hm(unsigned long long value) noexcept { return HectoMeterLL{ value }; }

		constexpr DecaMeterLL operator"" dam(unsigned long long value) noexcept { return DecaMeterLL{ value }; }

		constexpr MeterLL operator"" m(unsigned long long value) noexcept { return MeterLL{ value }; }

		constexpr DeciMeterLL operator"" dm(unsigned long long value) noexcept { return DeciMeterLL{ value }; }

		constexpr CentiMeterLL operator"" cm(unsigned long long value) noexcept { return CentiMeterLL{ value }; }

		constexpr MilliMeterLL operator"" mm(unsigned long long value) noexcept { return MilliMeterLL{ value }; }

		constexpr MicroMeterLL operator"" um(unsigned long long value) noexcept { return MicroMeterLL{ value }; }


		constexpr KiloMeterLD operator"" km(long double value) noexcept { return KiloMeterLD{ value }; }

		constexpr HectoMeterLD operator"" hm(long double value) noexcept { return HectoMeterLD{ value }; }

		constexpr DecaMeterLD operator"" dam(long double value) noexcept { return DecaMeterLD{ value }; }

		constexpr MeterLD operator"" m(long double value) noexcept { return MeterLD{ value }; }

		constexpr DeciMeterLD operator"" dm(long double value) noexcept { return DeciMeterLD{ value }; }

		constexpr CentiMeterLD operator"" cm(long double value) noexcept { return CentiMeterLD{ value }; }

		constexpr MilliMeterLD operator"" mm(long double value) noexcept { return MilliMeterLD{ value }; }

		constexpr MicroMeterLD operator"" um(long double value) noexcept { return MicroMeterLD{ value }; }
	}

#pragma endregion

#pragma region Imperial

	template<typename _Type> using Inch = Unit<Distance, _Type, std::ratio<254, 10000>>;
	template<typename _Type> using Foot = Unit<Distance, _Type, std::ratio<3048, 10000>>;
	template<typename _Type> using Yard = Unit<Distance, _Type, std::ratio<9144, 10000>>;
	template<typename _Type> using Mile = Unit<Distance, _Type, std::ratio<160934, 100>>;

	using InchLL = Inch<long long>;
	using FootLL = Foot<long long>;
	using YardLL = Yard<long long>;
	using MileLL = Mile<long long>;

	using InchLD = Inch<long double>;
	using FootLD = Foot<long double>;
	using YardLD = Yard<long double>;
	using MileLD = Mile<long double>;

	namespace Literals
	{
		constexpr InchLL operator"" inch(unsigned long long value) noexcept { return InchLL{ value }; }

		constexpr FootLL operator"" ft(unsigned long long value) noexcept { return FootLL{ value }; }

		constexpr YardLL operator"" yd(unsigned long long value) noexcept { return YardLL{ value }; }

		constexpr MileLL operator"" mi(unsigned long long value) noexcept { return MileLL{ value }; }


		constexpr InchLD operator"" inch(long double value) noexcept { return InchLD{ value }; }

		constexpr FootLD operator"" ft(long double value) noexcept { return FootLD{ value }; }

		constexpr YardLD operator"" yd(long double value) noexcept { return YardLD{ value }; }

		constexpr MileLD operator"" mi(long double value) noexcept { return MileLD{ value }; }
	}

#pragma endregion
}