#pragma once

#include <cmath>

#include "../_Matrix/StaticMatrix.h"

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
	using RVectorType = VectorND<T, N, RegularVector>;
	using HVectorType = VectorND<T, N, HomogeneousVector>;
	using BaseType    = StaticMatrix<T, (VecType == RegularVector ? N : N + 1), 1>;

	using ValType = T;
	using PtrType = T * ;
	using RefType = T & ;

public:
	VectorND() = default;

	VectorND(const std::initializer_list<T>& list) :
		BaseType(list)
	{}

	template<class E>
	VectorND(const MatrixExpression<E, ValType>& e) :
		BaseType(e)
	{}

	VectorND(const RVectorType& other, T w)
	{
		static_assert(VecType == HomogeneousVector);

		for (int i = 0; i < other.Line(); i++)
			(*this)[i] = other[i];

		this->w() = w;
	}

	RefType operator[](size_t i) { return (*this)(i, 0); }
	ValType operator[](size_t i) const { return (*this)(i, 0); }

	ValType x() const { static_assert(cm_X_Accessible); return (*this)[0]; }
	RefType x()	{ static_assert(cm_X_Accessible); return (*this)[0]; }

	ValType y() const { static_assert(cm_Y_Accessible); return (*this)[1]; }
	RefType y()	{ static_assert(cm_Y_Accessible); return (*this)[1]; }

	ValType z() const { static_assert(cm_Z_Accessible); return (*this)[2]; }
	RefType z() { static_assert(cm_Z_Accessible); return (*this)[2]; }

	ValType w() const { static_assert(cm_W_Accessible); return (*this)[N]; }
	RefType w() { static_assert(cm_W_Accessible); return (*this)[N]; }

	ValType SquareNorm() const
	{
		static_assert(VecType == RegularVector, "Cannot perform norm calculation on homogeneous vector.");

		T norm = 0;

		for (size_t i = 0; i < N; ++i)
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

public:
	static const bool cm_X_Accessible = N >= 1;
	static const bool cm_Y_Accessible = N >= 2;
	static const bool cm_Z_Accessible = N >= 3;
	static const bool cm_W_Accessible = VecType == HomogeneousVector;
};

/////////////////////
//-- Dot product --//
/////////////////////

template<typename T, size_t N>
T operator|(const VectorND<T, N>& a, const VectorND<T, N>& b)
{
	T dotproduct = 0;

	for (size_t i = 0; i < N; ++i)
		dotproduct += a[i] * b[i];

	return dotproduct;
}

typedef VectorND<float,   3, RegularVector>      Vector3Df;
typedef VectorND<float,   3, HomogeneousVector> HVector3Df;
typedef VectorND<double , 3, RegularVector>      Vector3Dd;
typedef VectorND<double , 3, HomogeneousVector> HVector3Dd;

typedef VectorND<float,   2, RegularVector>      Vector2Df;
typedef VectorND<float,   2, HomogeneousVector> HVector2Df;
typedef VectorND<double , 2, RegularVector>      Vector2Dd;
typedef VectorND<double , 2, HomogeneousVector> HVector2Dd;

///////////////////////
//-- Cross product --//
///////////////////////

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