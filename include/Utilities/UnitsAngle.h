#pragma once

#include <ratio>

#include "Units.h"

namespace LCN::Units
{
	class Angle;

	using DegToRadRatio = std::ratio<18000000000, 314159265>;

	template<typename _Type, std::enable_if_t<std::is_floating_point_v<_Type>, bool> = true>
	using Radian = Unit<Angle, _Type, DegToRadRatio>;

	template<typename _Type, std::enable_if_t<std::is_floating_point_v<_Type>, bool> = true>
	using Round = Unit<Angle, _Type, std::ratio<360, 1>>;

	template<typename _Type> using Degree    = Unit<Angle, _Type, std::ratio<1>>;
	template<typename _Type> using ArcMinute = Unit<Angle, _Type, std::ratio<1, 60>>;
	template<typename _Type> using ArcSecond = Unit<Angle, _Type, std::ratio<1, 3600>>;

	using RadianLD    = Radian<long double>;
	using RoundLD     = Round<long double>;
	using DegreeLD    = Degree<long double>;
	using ArcMinuteLD = ArcMinute<long double>;
	using ArcSecondLD = ArcSecond<long double>;
	
	using DegreeLL    = Degree<long long>;
	using ArcMinuteLL = ArcMinute<long long>;
	using ArcSecondLL = ArcSecond<long long>;

	namespace Literals
	{
		constexpr RadianLD operator"" rad(long double value) noexcept { return RadianLD{ value }; }

		constexpr RoundLD operator"" rnd(long double value) noexcept { return RoundLD{ value }; }

		constexpr DegreeLD operator"" deg(long double value) noexcept { return DegreeLD{ value }; }

		constexpr ArcMinuteLD operator"" amin(long double value) noexcept { return ArcMinuteLD{ value }; }

		constexpr ArcSecondLD operator"" asec(long double value) noexcept { return ArcSecondLD{ value }; }


		constexpr DegreeLL operator"" deg(unsigned long long value) noexcept { return DegreeLL{ value }; }

		constexpr ArcMinuteLL operator"" amin(unsigned long long value) noexcept { return ArcMinuteLL{ value }; }

		constexpr ArcSecondLL operator"" asec(unsigned long long value) noexcept { return ArcSecondLL{ value }; }
	}
}

namespace std
{
	template<
		typename _Type,
		typename _Ratio>
	constexpr long double cos(const LCN::Units::Unit<LCN::Units::Angle, _Type, _Ratio>& angle)
	{
		using UnitLD = LCN::Units::Unit<LCN::Units::Angle, long double, _Ratio>;

		return std::cos(LCN::Units::RadianLD(UnitLD(angle)).Count());
	}

	template<
		typename _Type,
		typename _Ratio>
	constexpr long double sin(const LCN::Units::Unit<LCN::Units::Angle, _Type, _Ratio>& angle)
	{
		using UnitLD = LCN::Units::Unit<LCN::Units::Angle, long double, _Ratio>;

		return std::sin(LCN::Units::RadianLD(UnitLD(angle)).Count());
	}

	template<
		typename _Type,
		typename _Ratio>
	constexpr long double tan(const LCN::Units::Unit<LCN::Units::Angle, _Type, _Ratio>& angle)
	{
		using UnitLD = LCN::Units::Unit<LCN::Units::Angle, long double, _Ratio>;

		return std::tan(LCN::Units::RadianLD(UnitLD(angle)).Count());
	}
}