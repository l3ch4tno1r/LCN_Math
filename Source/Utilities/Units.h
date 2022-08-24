#pragma once

#include <type_traits>

namespace LCN::Units
{
	template<typename _To, typename _From>
	constexpr _To UnitCast(const _From&);

	template<
		typename _MeasurementType,
		typename _Type,
		typename _Ratio,
		std::enable_if_t<std::is_arithmetic_v<_Type>, bool> = true>
	class Unit
	{
	public:
		using Type      = _MeasurementType;
		using ValueType = _Type;
		using RatioType = _Ratio;

	public:
		constexpr Unit() = default;

		template<typename _Type2>
		constexpr Unit(const _Type2& value) :
			m_Measure{ static_cast<_Type>(value) }
		{}

		template<typename _Type2, typename _Ratio2>
		constexpr Unit(const Unit<_MeasurementType, _Type2, _Ratio2>& other) :
			m_Measure{ UnitCast<Unit>(other).Count() }
		{}

		constexpr _Type Count() const { return m_Measure; }

		constexpr Unit operator+() const { return Unit(*this); }

		constexpr Unit operator-() const { return Unit(-m_Measure); }

		constexpr Unit& operator++()
		{
			++m_Measure;
			return *this;
		}

		constexpr Unit operator++(int) { return Unit(m_Measure++); }

		constexpr Unit& operator--()
		{
			--m_Measure;
			return *this;
		}

		constexpr Unit operator--(int) { return Unit(m_Measure--); }

		constexpr Unit& operator+=(const Unit& other)
		{
			m_Measure += other.m_Measure;
			return *this;
		}

		constexpr Unit& operator-=(const Unit& other)
		{
			m_Measure -= other.m_Measure;
			return *this;
		}

	private:
		_Type m_Measure{ _Type(0) };
	};

	template<
		typename _MeasurementType,
		typename _Type1,
		typename _Ratio1,
		typename _Type2,
		typename _Ratio2>
	struct std::common_type<
		Unit<_MeasurementType, _Type1, _Ratio1>,
		Unit<_MeasurementType, _Type2, _Ratio2>>
	{
		using type = LCN::Units::Unit<
			_MeasurementType,
			std::common_type_t<_Type1, _Type2>,
			std::ratio<
				std::_Gcd<_Ratio1::num, _Ratio2::num>::value,
				std::_Lcm<_Ratio1::den, _Ratio2::den>::value>>;
	};

	template<
		typename _To,
		typename _MeasurementType,
		typename _Type,
		typename _Ratio2>
	constexpr
	std::enable_if_t<std::is_same_v<_MeasurementType, typename _To::Type>, _To>
	UnitCast(
		const Unit<_MeasurementType, _Type, _Ratio2>& distance)
	{
		using FromRatio = _Ratio2;
		using ToRatio   = typename _To::RatioType;

		using NewRatio = std::ratio_divide<FromRatio, ToRatio>;

		return _To{ NewRatio::num * distance.Count() / NewRatio::den };
	}

	template<
		typename _MeasurementType,
		typename _Type1,
		typename _Ratio1,
		typename _Type2,
		typename _Ratio2>
	constexpr
	std::common_type_t<
		Unit<_MeasurementType, _Type1, _Ratio1>,
		Unit<_MeasurementType, _Type2, _Ratio2>>
	operator+(
		const Unit<_MeasurementType, _Type1, _Ratio1>& measure1,
		const Unit<_MeasurementType, _Type2, _Ratio2>& measure2)
	{
		using ReturnType = typename std::common_type<Unit<_MeasurementType, _Type1, _Ratio1>, Unit<_MeasurementType, _Type2, _Ratio2>>::type;

		return ReturnType(ReturnType(measure1).Count() + ReturnType(measure2).Count());
	}

	template<
		typename _MeasurementType,
		typename _Type1,
		typename _Ratio1,
		typename _Type2,
		typename _Ratio2>
	constexpr
	std::common_type_t<
		Unit<_MeasurementType, _Type1, _Ratio1>,
		Unit<_MeasurementType, _Type2, _Ratio2>>
	operator-(
		const Unit<_MeasurementType, _Type1, _Ratio1>& measure1,
		const Unit<_MeasurementType, _Type2, _Ratio2>& measure2)
	{
		using ReturnType = typename std::common_type<Unit<_MeasurementType, _Type1, _Ratio1>, Unit<_MeasurementType, _Type2, _Ratio2>>::type;

		return ReturnType(ReturnType(measure1).Count() - ReturnType(measure2).Count());
	}

	template<
		typename _MeasurementType,
		typename _Type1,
		typename _Type2,
		typename _Ratio2>
	constexpr
	Unit<_MeasurementType, std::common_type_t<_Type1, _Type2>, _Ratio2>
	operator*(
		const _Type1& factor,
		const Unit<_MeasurementType, _Type2, _Ratio2>& measure)
	{
		using CommonType = std::common_type_t<_Type1, _Type2>;
		using ReturnType = Unit<_MeasurementType, CommonType, _Ratio2>;

		return ReturnType(factor * ReturnType(measure).Count());
	}

	template<
		typename _MeasurementType,
		typename _Type1,
		typename _Ratio1,
		typename _Type2>
	inline
	constexpr
	Unit<_MeasurementType, std::common_type_t<_Type1, _Type2>, _Ratio1>
	operator*(
		const Unit<_MeasurementType, _Type1, _Ratio1>& measure,
		const _Type2& factor)
	{
		return factor * measure;
	}

	template<
		typename _MeasurementType,
		typename _Type1,
		typename _Ratio1,
		typename _Type2>
	constexpr
	Unit<_MeasurementType, std::common_type_t<_Type1, _Type2>, _Ratio1>
	operator/(
		const Unit<_MeasurementType, _Type1, _Ratio1>& measure,
		const _Type2& factor)
	{
		using CommonType = std::common_type_t<_Type1, _Type2>;
		using ReturnType = Unit<_MeasurementType, CommonType, _Ratio1>;

		return ReturnType(ReturnType(measure).Count() / factor);
	}

	template<
		typename _MeasurementType,
		typename _Type1,
		typename _Ratio1,
		typename _Type2>
	constexpr
	Unit<_MeasurementType, std::common_type_t<_Type1, _Type2>, _Ratio1>
	operator%(
		const Unit<_MeasurementType, _Type1, _Ratio1>& measure,
		const _Type2& divider)
	{
		using CommonType = std::common_type_t<_Type1, _Type2>;
		using ReturnType = Unit<_MeasurementType, CommonType, _Ratio1>;

		return ReturnType(ReturnType(measure).Count() % divider);
	}

	template<
		typename _MeasurementType,
		typename _Type1,
		typename _Ratio1,
		typename _Type2,
		typename _Ratio2>
	constexpr
	std::common_type_t<
		Unit<_MeasurementType, _Type1, _Ratio1>,
		Unit<_MeasurementType, _Type2, _Ratio2>>
	operator%(
		const Unit<_MeasurementType, _Type1, _Ratio1>& measure1,
		const Unit<_MeasurementType, _Type2, _Ratio2>& measure2)
	{
		using ReturnType = std::common_type_t<
			Unit<_MeasurementType, _Type1, _Ratio1>,
			Unit<_MeasurementType, _Type2, _Ratio2>>;

		return ReturnType(ReturnType(measure1).Count() % ReturnType(measure2).Count());
	}
}