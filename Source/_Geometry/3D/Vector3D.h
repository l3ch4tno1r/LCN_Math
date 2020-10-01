#pragma once

#include "_Geometry/VectorBase.h"

template<typename T>
struct Vector3D : public VectorBase<Vector3D<T>, T, 3>
{
	union
	{
		struct
		{
			T x, y, z;
		};
		T mat[3];
	};

	Vector3D(T x, T y, T z) :
		x(x), y(y), z(z)
	{}

	template<class E>
	Vector3D(const MatrixExpression<E, T>& other)
	{
		ASSERT((this->Line() == other.Line()) && (this->Column() == other.Column()));

		for (size_t i = 0; i < 3; i++)
			mat[i] = other(i, 0);
	}

	template<class E>
	Vector3D& operator=(const MatrixExpression<E, T>& other)
	{
		ASSERT((this->Line() == other.Line()) && (this->Column() == other.Column()));

		for (size_t i = 0; i < 3; i++)
			mat[i] = other(i, 0);

		return *this;
	}

	T operator()(size_t i, size_t) const { return mat[i]; }
	T operator[](size_t i) const { return mat[i]; }

	constexpr size_t Line()   const { return 3; }
	constexpr size_t Column() const { return 1; }
};

template<class EL, class ER, typename T>
class Vector3DCrossProduct : public VectorBase<Vector3DCrossProduct<EL, ER, T>, T, 3>
{
private:
	const EL& el;
	const ER& er;

	Vector3DCrossProduct(const EL& el, const ER& er) :
		el(el),
		er(er)
	{}

	template<class EL, class ER, typename T>
	friend Vector3DCrossProduct<EL, ER, T> operator^(const StaticMatrixBase<EL, T, 3, 1>&, const StaticMatrixBase<ER, T, 3, 1>&);

	template<class EL, class ER, typename T>
	friend Vector3DCrossProduct<EL, ER, T> operator^(const MatrixExpression<EL, T>&, const MatrixExpression<ER, T>&);

public:
	T operator[](size_t i) const
	{
		size_t ip1 = (i + 1) % 3;
		size_t ip2 = (i + 2) % 3;

		return el(ip1, 0) * er(ip2, 0) - el(ip2, 0) * er(ip1, 0);
	}

	T operator()(size_t i, size_t) const
	{
		return (*this)[i];
	}
};

template<class EL, class ER, typename T>
inline Vector3DCrossProduct<EL, ER, T> operator^(const StaticMatrixBase<EL, T, 3, 1>& el, const StaticMatrixBase<ER, T, 3, 1>& er)
{
	return Vector3DCrossProduct<EL, ER, T>(static_cast<const EL&>(el), static_cast<const ER&>(er));
}

template<class EL, class ER, typename T>
inline Vector3DCrossProduct<EL, ER, T> operator^(const MatrixExpression<EL, T>& el, const MatrixExpression<ER, T>& er)
{
	ASSERT(el.Line() == 3 && er.Line() == 3 && el.Column() == 1 && er.Column() == 1);

	return Vector3DCrossProduct<EL, ER, T>(static_cast<const EL&>(el), static_cast<const ER&>(er));
}
