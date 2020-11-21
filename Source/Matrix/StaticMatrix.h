#pragma once

#include <initializer_list>

#include "StaticMatrixBase.h"

template<typename T, size_t L, size_t C>
class StaticMatrix : public StaticMatrixBase<StaticMatrix<T, L, C>, L, C>
{
public:
	using ValType = T;
	using PtrType = T*;
	using RefType = T&;

public:
	StaticMatrix() = default;

	StaticMatrix(const std::initializer_list<ValType>& list)
	{
		size_t Idx = 0;

		for (ValType e : list)
		{
			if (Idx > L * C)
				break;

			size_t i = Idx / C;
			size_t j = Idx % C;

			m_Tab[i][j] = e;

			++Idx;
		}
	}

	template<class E>
	StaticMatrix(const MatrixExpression<E>& other)
	{
		ASSERT((this->Line() == other.Line()) && (this->Column() == other.Column()));

		for (size_t i = 0; i < other.Line(); ++i)
			for (size_t j = 0; j < other.Column(); ++j)
				m_Tab[i][j] = other(i, j);
	}

	template<class E>
	StaticMatrix& operator=(const MatrixExpression<E>& other)
	{
		ASSERT((this->Line() == other.Line()) && (this->Column() == other.Column()));

		for (size_t i = 0; i < other.Line(); ++i)
			for (size_t j = 0; j < other.Column(); ++j)
				m_Tab[i][j] = other(i, j);

		return *this;
	}

	template<class EL, class ER>
	StaticMatrix& operator=(const ProductOperation<EL, ER>& prod)
	{
		ASSERT((this->Line() == prod.Line()) && (this->Column() == prod.Column()));

		if(prod.ContainsRefTo(*this))
		{
			StaticMatrix temp = prod;

			(*this) = temp;
		}
		else
		{
			for (size_t i = 0; i < prod.Line(); ++i)
				for (size_t j = 0; j < prod.Column(); ++j)
					m_Tab[i][j] = prod(i, j);
		}

		return *this;
	}

	RefType operator()(size_t i, size_t j) { return m_Tab[i][j]; }
	ValType operator()(size_t i, size_t j) const { return m_Tab[i][j]; }

	static StaticMatrix<ValType, L, 2 * C> Matrix2C()
	{
		return StaticMatrix<ValType, L, 2 * C>();
	}

	template<class _E>
	constexpr bool ContainsRefTo(const _E& other) const { return false; }

private:
	ValType m_Tab[L][C];
};

template<typename T, size_t L, size_t C>
class Traits<StaticMatrix<T, L, C>> : public Traits<StaticMatrixBase<StaticMatrix<T, L, C>, L, C>>
{
public:
	using ValType = T;
};