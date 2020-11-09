#pragma once

#include <iostream>

#include "../Utilities/ErrorHandling.h"

//////////////////////
//-- Traits class --//
//////////////////////

template<class ObjType>
class Traits {};

#pragma region MatrixExpression
///////////////////////////
//-- Matrix Expression --//
///////////////////////////

template<class E>
class MatrixExpression
{
public:
	enum
	{
		LineAtCT   = Traits<E>::LineAtCT,
		ColumnAtCT = Traits<E>::ColumnAtCT,
		DimAtCT    = Traits<E>::DimAtCT,
	};

	using ValType = typename Traits<E>::ValType;

	ValType operator()(size_t i, size_t j) const { return this->Derived()(i, j); }

	size_t Line()   const { return this->Derived().Line(); }
	size_t Column() const { return this->Derived().Column(); }

protected:
	inline E& Derived() { return static_cast<E&>(*this); }
	inline const E& Derived() const { return static_cast<const E&>(*this); }
};

template<class E>
std::ostream& operator<<(std::ostream& stream, const MatrixExpression<E>& mat)
{
	for (size_t i = 0; i < mat.Line(); ++i)
	{
		for (size_t j = 0; j < mat.Column(); ++j)
			stream << mat(i, j) << ' ';

		stream << std::endl;
	}

	return stream;
}
#pragma endregion

#pragma region MatrixOperations
///////////////////////////
//-- Matrix Operations --//
///////////////////////////

template<class EL, class ER>
class BinaryOpReturnType
{
	static_assert(std::is_same<typename Traits<EL>::ValType, typename Traits<ER>::ValType>::value, "Types must match.");

public:
	using ValType = typename Traits<EL>::ValType;
};

template<class Derived, class EL, class ER>
class BinaryOperation : public MatrixExpression<Derived>
{
public:
	using ValType = typename BinaryOpReturnType<EL, ER>::ValType;

protected:
	const EL& el;
	const ER& er;

	BinaryOperation(const EL& el, const ER& er) :
		el(el),
		er(er)
	{}
};

template<class Derived, class EL, class ER>
class Traits<BinaryOperation<Derived, EL, ER>>
{
public:
	enum
	{
		DimAtCT = Traits<EL>::DimAtCT && Traits<EL>::DimAtCT
	};

	using ValType = typename BinaryOpReturnType<EL, ER>::ValType;
};

#pragma region Addition
//////////////////
//-- Addition --//
//////////////////

template<class EL, class ER>
class AddOperation : public BinaryOperation<AddOperation<EL, ER>, EL, ER>
{
public:
	using ThisType = AddOperation<EL, ER>;
	using Base     = BinaryOperation<ThisType, EL, ER>;
	using ValType  = typename Base::ValType;

	ValType operator()(size_t i, size_t j) const { return this->el(i, j) + this->er(i, j); }

	size_t Line()   const { return this->el.Line(); }
	size_t Column() const { return this->el.Column(); }

private:
	AddOperation(const EL& el, const ER& er) :
		Base(el, er)
	{}

	template<class _EL, class _ER>
	friend AddOperation<_EL, _ER> operator+(const MatrixExpression<_EL>& el, const MatrixExpression<_ER>& er);
};

template<class EL, class ER>
class Traits<AddOperation<EL, ER>> : public Traits<BinaryOperation<AddOperation<EL, ER>, EL, ER>>
{
public:
	enum
	{
		LineAtCT   = (Traits<EL>::LineAtCT   == Traits<ER>::LineAtCT   ? Traits<EL>::LineAtCT   : -1),
		ColumnAtCT = (Traits<EL>::ColumnAtCT == Traits<ER>::ColumnAtCT ? Traits<EL>::ColumnAtCT : -1)
	};
};

template<class _EL, class _ER>
inline AddOperation<_EL, _ER> operator+(const MatrixExpression<_EL>& el, const MatrixExpression<_ER>& er)
{
	if constexpr (Traits<AddOperation<_EL, _ER>>::DimAtCT)
		static_assert(Traits<AddOperation<_EL, _ER>>::LineAtCT != -1 && Traits<AddOperation<_EL, _ER>>::ColumnAtCT != -1);
	else
		ASSERT(el.Line() == er.Line() && el.Column() == er.Column());

	return AddOperation<_EL, _ER>(static_cast<const _EL&>(el), static_cast<const _ER&>(er));
}
#pragma endregion

#pragma region Substraction
//////////////////////
//-- Substraction --//
//////////////////////

template<class EL, class ER>
class SubOperation : public BinaryOperation<SubOperation<EL, ER>, EL, ER>
{
public:
	using ThisType = SubOperation<EL, ER>;
	using Base     = BinaryOperation<ThisType, EL, ER>;
	using ValType  = typename Base::ValType;

	ValType operator()(size_t i, size_t j) const { return this->el(i, j) - this->er(i, j); }

	size_t Line()   const { return this->el.Line(); }
	size_t Column() const { return this->el.Column(); }

private:
	SubOperation(const EL& el, const ER& er) :
		Base(el, er)
	{}

	template<class _EL, class _ER>
	friend SubOperation<_EL, _ER> operator-(const MatrixExpression<_EL>& el, const MatrixExpression<_ER>& er);
};

template<class EL, class ER>
class Traits<SubOperation<EL, ER>> : public Traits<BinaryOperation<SubOperation<EL, ER>, EL, ER>>
{
public:
	enum
	{
		LineAtCT   = (Traits<EL>::LineAtCT   == Traits<ER>::LineAtCT   ? Traits<EL>::LineAtCT   : -1),
		ColumnAtCT = (Traits<EL>::ColumnAtCT == Traits<ER>::ColumnAtCT ? Traits<EL>::ColumnAtCT : -1)
	};
};

template<class _EL, class _ER>
inline SubOperation<_EL, _ER> operator-(const MatrixExpression<_EL>& el, const MatrixExpression<_ER>& er)
{
	if constexpr (Traits<SubOperation<_EL, _ER>>::DimAtCT)
		static_assert(Traits<SubOperation<_EL, _ER>>::LineAtCT != -1 && Traits<SubOperation<_EL, _ER>>::ColumnAtCT != -1);
	else
		ASSERT(el.Line() == er.Line() && el.Column() == er.Column());

	return SubOperation<_EL, _ER>(static_cast<const _EL&>(el), static_cast<const _ER&>(er));
}
#pragma endregion

#pragma region Multiplication
////////////////////////
//-- Multiplication --//
////////////////////////

template<class EL, class ER>
class MultOperation : public BinaryOperation<MultOperation<EL, ER>, EL, ER>
{
public:
	using ThisType = MultOperation<EL, ER>;
	using Base     = BinaryOperation<ThisType, EL, ER>;
	using ValType  = typename Base::ValType;

	ValType operator()(size_t i, size_t j) const
	{
		ValType result = 0;

		for (size_t k = 0; k < this->el.Column(); ++k)
			result += this->el(i, k) * this->er(k, j);

		return result;
	}

	size_t Line()   const { return this->el.Line(); }
	size_t Column() const { return this->er.Column(); }

private:
	MultOperation(const EL& el, const ER& er) :
		Base(el, er)
	{}

	template<class _EL, class _ER>
	friend MultOperation<_EL, _ER> operator*(const MatrixExpression<_EL>& el, const MatrixExpression<_ER>& er);
};

template<class EL, class ER>
class Traits<MultOperation<EL, ER>> : public Traits<BinaryOperation<MultOperation<EL, ER>, EL, ER>>
{
public:
	enum
	{
		LineAtCT   = Traits<EL>::LineAtCT,
		ColumnAtCT = Traits<ER>::ColumnAtCT
	};
};

template<class _EL, class _ER>
inline MultOperation<_EL, _ER> operator*(const MatrixExpression<_EL>& el, const MatrixExpression<_ER>& er)
{
	if constexpr (Traits<MultOperation<_EL, _ER>>::DimAtCT)
		static_assert(Traits<_EL>::ColumnAtCT == Traits<_ER>::LineAtCT);
	else
		ASSERT(el.Column() == er.Line());

	return MultOperation<_EL, _ER>(static_cast<const _EL&>(el), static_cast<const _ER&>(er));
}

/////////////////
//-- Scaling --//
/////////////////

template<class E>
class ScaleOperation : public MatrixExpression<ScaleOperation<E>>
{
public:
	using ValType = typename Traits<E>::ValType;

	ValType operator()(size_t i, size_t j) const
	{
		return scalefactor * e(i, j);
	}

	size_t Line()   const { return e.Line(); }
	size_t Column() const { return e.Column(); }

private:
	const E& e;
	ValType scalefactor;

	ScaleOperation(const E& e, ValType scalefactor) :
		e(e),
		scalefactor(scalefactor)
	{}

	template<class E>
	friend ScaleOperation<E> operator*(const MatrixExpression<E>&, typename Traits<E>::ValType);

	template<class E>
	friend ScaleOperation<E> operator*(typename Traits<E>::ValType, const MatrixExpression<E>&);

	template<class E>
	friend ScaleOperation<E> operator/(const MatrixExpression<E>&, typename Traits<E>::ValType);
};

template<class E>
inline ScaleOperation<E> operator*(const MatrixExpression<E>& e, typename Traits<E>::ValType scalefactor)
{
	return ScaleOperation<E>(static_cast<const E&>(e), scalefactor);
}

template<class E>
inline ScaleOperation<E> operator*(typename Traits<E>::ValType scalefactor, const MatrixExpression<E>& e)
{
	return ScaleOperation<E>(static_cast<const E&>(e), scalefactor);
}

template<class E>
inline ScaleOperation<E> operator/(const MatrixExpression<E>& e, typename Traits<E>::ValType scalefactor)
{
	using ValType = typename Traits<E>::ValType;

	return ScaleOperation<E>(static_cast<const E&>(e), ValType(1) / scalefactor);
}

template<class E>
class Traits<ScaleOperation<E>>
{
public:
	enum
	{
		DimAtCT    = Traits<E>::DimAtCT,
		LineAtCT   = Traits<E>::LineAtCT,
		ColumnAtCT = Traits<E>::ColumnAtCT
	};

	using ValType = typename Traits<E>::ValType;
};

#pragma endregion

#pragma endregion