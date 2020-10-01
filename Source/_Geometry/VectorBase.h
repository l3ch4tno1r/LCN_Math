#pragma once

#include <cmath>

#include "_Matrix/StaticMatrixBase.h"

template<class Derived, typename T, size_t N>
class VectorBase : public StaticMatrixBase<Derived, T, N, 1>
{
public:
	T operator[](size_t i) const { return this->Derived()[i]; }

	T SquareNorm() const
	{
		T norm = 0;

		for (size_t i = 0; i < N; ++i)
			norm += (*this)[i] * (*this)[i];

		return norm;
	}

	T Norm() const
	{		
		return std::sqrt(this->SquareNorm());
	}
};

template<class E, typename T, size_t N>
T operator|(const VectorBase<E, T, N>& a, const VectorBase<E, T, N>& b)
{
	T dotproduct = 0;

	for (size_t i = 0; i < N; ++i)
		dotproduct += a[i] * b[i];

	return dotproduct;
}