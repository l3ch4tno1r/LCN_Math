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
		size_t Idx = 0;

		for (ValType e : values)
		{
			if (Idx > L * C)
				break;

			size_t i = Idx / C;
			size_t j = Idx % C;

			m_Data[i][j] = e;

			++Idx;
		}
	}

	template<class E>
	Matrix(const MatrixExpression<E>& expr)
	{
		ASSERT((this->Line() == expr.Line()) && (this->Column() == expr.Column()));

		for (size_t i = 0; i < expr.Line(); ++i)
			for (size_t j = 0; j < expr.Column(); ++j)
				m_Data[i][j] = expr(i, j);
	}

	//////////////////////////////
	//-- Overriding from base --//
	//////////////////////////////

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

public:
	inline static Matrix<T, L, 2 * C> Matrix2C() { return Matrix<T, L, 2 * C>(); }

	friend class Base;
	friend class Matrix<T, L, C / 2>;

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

///////////////////////////////
//-- Heap allocated matrix --//
///////////////////////////////

template<typename T>
class MatrixN : public MatrixBase<MatrixN<T>>
{
public:
	using ValType = T;
	using RefType = ValType&;
	using PtrType = ValType*;

	MatrixN() = default;

	MatrixN(size_t line, size_t column) :
		m_Line(line),
		m_Column(column)
	{
		m_Data = new ValType[m_Line * m_Column];
	}

	~MatrixN()
	{
		if (m_Data)
			delete[] m_Data;

		m_Data = nullptr;
	}

	inline ValType operator()(size_t i, size_t j) const { return m_Data[i * m_Column + j]; }
	inline RefType operator()(size_t i, size_t j) { return m_Data[i * m_Column + j]; }

	inline size_t Line()   const { return m_Line; }
	inline size_t Column() const { return m_Column; }

	void AssertSquareMatrix() const
	{
		if (m_Line != m_Column)
			throw std::exception("This is not a square matrix.");
	}

private:
	PtrType m_Data = nullptr;

	size_t m_Line   = 0;
	size_t m_Column = 0;

	void Resize(size_t line, size_t column)
	{
		size_t currentsize = m_Line * m_Column;
		size_t newsize     = line * column;

		if (newsize > currentsize)
		{
			delete[] m_Data;

			m_Data = new ValType[newsize];
		}

		m_Line   = line;
		m_Column = column;
	}
};

template<typename T>
class Traits<MatrixN<T>>
{
public:
	enum
	{
		SizeAtCT   = false,
		LineAtCT   = -1,
		ColumnAtCT = -1
	};

	using ValType = T;
};

/////////////////////////
//-- Type definition --//
/////////////////////////