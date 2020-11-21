#pragma once

#include "MatrixExpression.h"

///////////////////////////////
//-- Matrix implementation --//
///////////////////////////////

template<class MatrixAllocationPolicy>
class MatrixImpl : public MatrixAllocationPolicy
{
public:
	using Base    = MatrixAllocationPolicy;
	using ValType = typename Traits<MatrixAllocationPolicy>::ValType;

	MatrixImpl() :
		Base()
	{}

	template<class E>
	MatrixImpl(const MatrixExpression<E>& other)
	{
		ASSERT((this->Line() == other.Line()) && (this->Column() == other.Column()));

		for (size_t i = 0; i < other.Line(); ++i)
			for (size_t j = 0; j < other.Column(); ++j)
				(*this)(i, j) = other(i, j);
	}

	MatrixImpl(const std::initializer_list<ValType>& initval)
	{
		size_t Idx = 0;

		for (ValType e : initval)
		{
			if (Idx > this->Line() * this->Column())
				break;

			size_t i = Idx / this->Column();
			size_t j = Idx % this->Column();

			(*this)(i, j) = e;

			++Idx;
		}
	}

	template<class E>
	MatrixImpl& operator=(const MatrixExpression<E>& other)
	{
		ASSERT((this->Line() == other.Line()) && (this->Column() == other.Column()));

		for (size_t i = 0; i < other.Line(); ++i)
			for (size_t j = 0; j < other.Column(); ++j)
				(*this)(i, j) = other(i, j);

		return *this;
	}

	template<class E>
	MatrixImpl& operator+=(const MatrixExpression<E>& other)
	{
		ASSERT((this->Line() == other.Line()) && (this->Column() == other.Column()));

		for (size_t i = 0; i < other.Line(); ++i)
			for (size_t j = 0; j < other.Column(); ++j)
				(*this)(i, j) += other(i, j);

		return *this;
	}
};

////////////////////////////////
//-- Stack allocated matrix --//
////////////////////////////////

template<typename T, size_t L, size_t C>
class StaticMatrix : public MatrixExpression<StaticMatrix<T, L, C>>
{
public:
	using ValType = T;
	using RefType = ValType&;

	inline ValType operator()(size_t i, size_t j) const { return m_Data[i][j]; }
	inline RefType operator()(size_t i, size_t j) { return m_Data[i][j]; }

	constexpr size_t Line()   const { return L; }
	constexpr size_t Column() const { return C; }

protected:
	StaticMatrix()                    = default;
	StaticMatrix(const StaticMatrix&) = default;
	StaticMatrix(StaticMatrix&&)      = default;

private:
	T m_Data[L][C];
};

template<typename T, size_t L, size_t C>
class Traits<StaticMatrix<T, L, C>>
{
public:
	enum
	{
		LineAtCT   = L,
		ColumnAtCT = C,
		SizeAtCT   = true
	};

	using ValType = T;
};

/////////////////////////
//-- Type definition --//
/////////////////////////

template<typename T, size_t L, size_t C>
using Matrix = MatrixImpl<StaticMatrix<T, L, C>>;