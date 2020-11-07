#pragma once

#include "Vector.h"

template<typename T, size_t N>
class Transform : public StaticMatrixBase<Transform<T, N>, T, N + 1, N + 1>
{
public:
	enum
	{
		Dim  = N,
		HDim = N + 1
	};

	using ValType = T;
	using PtrType = T*;
	using RefType = T&;

	using HVectorNDT = VectorND<ValType, Dim, HomogeneousVector>;

public:
	Transform()
	{
		for (size_t i = 0; i < this->Line(); ++i)
			for (size_t j = 0; j < this->Column(); j++)
				(*this)(i, j) = (i == j ? ValType(1) : ValType(0));
	}

	Transform(const std::initializer_list<ValType>& list)
	{
		size_t idx = 0;

		for (auto e : list)
		{
			size_t i = idx / HDim;
			size_t j = idx % HDim;

			m_Vectors[j][i] = e;

			++idx;
		}
	}

	template<class E>
	Transform(const MatrixExpression<E, ValType>& other)
	{
		ASSERT((this->Line() == other.Line()) && (this->Column() == other.Column()));

		for (size_t i = 0; i < this->Line(); ++i)
			for (size_t j = 0; j < this->Column(); j++)
				(*this)(i, j) = other(i, j);
	}

	template<class E>
	Transform& operator=(const MatrixExpression<E, ValType>& other)
	{
		ASSERT((this->Line() == other.Line()) && (this->Column() == other.Column()));

		for (size_t i = 0; i < this->Line(); ++i)
			for (size_t j = 0; j < this->Column(); j++)
				(*this)(i, j) = other(i, j);
	}

	RefType operator()(size_t i, size_t j) { return m_Vectors[j][i]; }
	ValType operator()(size_t i, size_t j) const { return m_Vectors[j][i]; }

	HVectorNDT& operator[](size_t i) { return m_Vectors[i]; }
	const HVectorNDT& operator[](size_t i) const { return m_Vectors[i]; }

public:
	static const Transform& Identity()
	{
		static Transform identity;
		return identity;
	}

	static StaticMatrix<ValType, HDim, 2 * HDim> Matrix2C()
	{
		return StaticMatrix<ValType, 4, 8>();
	}

private:
	HVectorNDT m_Vectors[HDim];
};

typedef Transform<float, 3>  Transform3Df;
typedef Transform<double, 3> Transform3Dd;