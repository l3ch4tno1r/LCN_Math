#pragma once

#include "MatrixBase.h"

////////////////////////////////
//-- Stack allocated matrix --//
////////////////////////////////

template<typename T, size_t L, size_t C>
class Matrix : public MatrixBase<Matrix<T, L, C>>
{
public:
	using Base = MatrixBase<Matrix<T, L, C>>;

	using ValType = T;
	using RefType = ValType&;

	Matrix() = default;
	Matrix(const Matrix& other) = default;

	Matrix(const std::initializer_list<ValType>& values)
	{
		Base::operator=(values);
	}

	template<class E>
	Matrix(const MatrixExpression<E>& expr)
	{
		Base::operator=(expr);
	}

	///////////////////////////////
	//-- Override base methods --//
	///////////////////////////////

	inline ValType operator()(size_t i, size_t j) const { return m_Data[i][j]; }
	inline RefType operator()(size_t i, size_t j) { return m_Data[i][j]; }

	inline constexpr size_t Line()   const { return L; }
	inline constexpr size_t Column() const { return C; }

	constexpr void AssertSquareMatrix() const { static_assert(L == C); }

	///////////////////////////////
	//-- Assignement operators --//
	///////////////////////////////

	Matrix& operator=(const Matrix&) = default;

	template<class E>
	inline Matrix& operator=(const MatrixExpression<E>& expr)
	{
		return Base::operator=(expr);
	}

	inline Matrix& operator=(const std::initializer_list<ValType>& values)
	{
		return Base::operator=(values);
	}

public:
	inline static Matrix<T, L, 2 * C> Matrix2C() { return Matrix<T, L, 2 * C>(); }

	friend class Base;
	friend class Matrix<T, L, C / 2>; // O_o

private:
	T m_Data[L][C];
};

template<typename T, size_t L, size_t C>
class Traits<Matrix<T, L, C>>
{
public:
	enum
	{
		SizeAtCT   = true,
		LineAtCT   = L,
		ColumnAtCT = C
	};

	using ValType = T;
};

/////////////////////////
//-- Type definition --//
/////////////////////////

typedef Matrix<float, 2, 2> Matrix2f;
typedef Matrix<float, 3, 3> Matrix3f;
typedef Matrix<float, 4, 4> Matrix4f;

typedef Matrix<double, 2, 2> Matrix2d;
typedef Matrix<double, 3, 3> Matrix3d;
typedef Matrix<double, 4, 4> Matrix4d;

typedef Matrix<int, 2, 2> Matrix2i;
typedef Matrix<int, 3, 3> Matrix3i;
typedef Matrix<int, 4, 4> Matrix4i;

typedef Matrix<unsigned int, 2, 2> Matrix2u;
typedef Matrix<unsigned int, 3, 3> Matrix3u;
typedef Matrix<unsigned int, 4, 4> Matrix4u;