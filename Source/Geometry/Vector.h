#pragma once

#include <cmath>

#include "../Matrix/StaticMatrix.h"

enum VectorType
{
	RegularVector,
	HomogeneousVector
};

////////////////////////
//-- VectorND class --//
////////////////////////

template<typename T, size_t N, VectorType VecType = RegularVector>
class VectorND : public StaticMatrix<T, (VecType == RegularVector ? N : N + 1), 1>
{
public:
	enum
	{
		Dim  = N,
		HDim = N + 1
	};

	using RVectorType = VectorND<T, Dim, RegularVector>;
	using HVectorType = VectorND<T, Dim, HomogeneousVector>;
	using BaseType    = StaticMatrix<T, (VecType == RegularVector ? Dim : HDim), 1>;
	using RVectorView = StaticMatrixView<HVectorType, Dim, 1>;

	using ValType = T;
	using PtrType = T*;
	using RefType = T&;

public:
	VectorND() = default;

	VectorND(size_t I)
	{
		for (size_t i = 0; i < this->Line(); ++i)
			(*this)[i] = (i == I ? ValType(1) : ValType(0));
	}

	VectorND(const std::initializer_list<T>& list) :
		BaseType(list)
	{}

	template<class E>
	VectorND(const MatrixExpression<E>& e) :
		BaseType(e)
	{}

	VectorND(const RVectorType& other, T w)
	{
		static_assert(VecType == HomogeneousVector);

		for (size_t i = 0; i < other.Line(); i++)
			(*this)[i] = other[i];

		this->w() = w;
	}

	RefType operator[](size_t i) { return (*this)(i, 0); }
	ValType operator[](size_t i) const { return (*this)(i, 0); }

	ValType x() const { static_assert(X_Accessible); return (*this)[0]; }
	RefType x()	{ static_assert(X_Accessible); return (*this)[0]; }

	ValType y() const { static_assert(Y_Accessible); return (*this)[1]; }
	RefType y()	{ static_assert(Y_Accessible); return (*this)[1]; }

	ValType z() const { static_assert(Z_Accessible); return (*this)[2]; }
	RefType z() { static_assert(Z_Accessible); return (*this)[2]; }

	ValType w() const { static_assert(W_Accessible); return (*this)[Dim]; }
	RefType w() { static_assert(W_Accessible); return (*this)[Dim]; }

	ValType SquareNorm() const
	{
		//static_assert(VecType == RegularVector, "Cannot perform norm calculation on homogeneous vector.");

		T norm = 0;

		for (size_t i = 0; i < Dim; ++i)
			norm += (*this)[i] * (*this)[i];

		return norm;
	}

	ValType Norm() const
	{		
		return std::sqrt(this->SquareNorm());
	}

	HVectorType Homogeneous(T w = T(1)) const
	{
		static_assert(VecType == RegularVector, "Cannot convert homogeneous vector.");

		return HVectorType(*this, w);
	}

	RVectorView VectorView() const
	{
		static_assert(VecType == HomogeneousVector, "Cannot use it with regular vector.");

		return RVectorView(*this, 0, 0);
	}

	RVectorType Vector() const
	{
		return RVectorType(this->VectorView());
	}

public:
	enum
	{
		X_Accessible = Dim >= 1,
		Y_Accessible = Dim >= 2,
		Z_Accessible = Dim >= 3,
		W_Accessible = VecType == HomogeneousVector
	};

	template<size_t i>
	static const VectorND& UnitVector()
	{
		static VectorND unitvector(i);
		return unitvector;
	}

	static const VectorND& X() { static_assert(X_Accessible); return UnitVector<0>(); }
	static const VectorND& Y() { static_assert(Y_Accessible); return UnitVector<1>(); }
	static const VectorND& Z() { static_assert(Z_Accessible); return UnitVector<2>(); }
};

template<typename T, size_t N>
using HVectorND = VectorND<T, N, HomogeneousVector>;

template<typename T, size_t N, VectorType VecType>
class Traits<VectorND<T, N, VecType>> : public Traits<StaticMatrix<T, (VecType == RegularVector ? N : N + 1), 1>>
{};

/////////////////////
//-- Dot product --//
/////////////////////

template<typename T, size_t N, VectorType VecType>
T operator|(const VectorND<T, N, VecType>& a, const VectorND<T, N, VecType>& b)
{
	T dotproduct(0);

	for (size_t i = 0; i < a.Dim; ++i)
		dotproduct += a[i] * b[i];

	return dotproduct;
}

typedef VectorND<float,  3, RegularVector>      Vector3Df;
typedef VectorND<float,  3, HomogeneousVector> HVector3Df;
typedef VectorND<double, 3, RegularVector>      Vector3Dd;
typedef VectorND<double, 3, HomogeneousVector> HVector3Dd;

typedef VectorND<float,  2, RegularVector>      Vector2Df;
typedef VectorND<float,  2, HomogeneousVector> HVector2Df;
typedef VectorND<double, 2, RegularVector>      Vector2Dd;
typedef VectorND<double, 2, HomogeneousVector> HVector2Dd;

///////////////////////
//-- Cross product --//
///////////////////////

/*
template<class EL, class ER, typename T>
class Vector3DCrossProduct : public StaticMatrixBase<Vector3DCrossProduct<EL, ER, T>, T, 3, 1>
{
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
*/

template<class EL, class ER>
Vector3Df operator^(const MatrixExpression<EL>& el, const MatrixExpression<ER>& er)
{
	static_assert(Traits<EL>::SizeAtCT && Traits<ER>::SizeAtCT, "Size must be known at compile time.");
	static_assert(Traits<EL>::LineAtCT   == 3 && Traits<ER>::LineAtCT   == 3);
	static_assert(Traits<EL>::ColumnAtCT == 1 && Traits<ER>::ColumnAtCT == 1);

	Vector3Df _el = el, _er = er;

	return Vector3Df({
		_el.y() * _er.z() - _el.z() * _er.y(),
		_el.z() * _er.x() - _el.x() * _er.z(),
		_el.x() * _er.y() - _el.y() * _er.x()
	});
}