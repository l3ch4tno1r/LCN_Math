#pragma once

#include "MatrixBase.h"

///////////////////////////////
//-- Heap allocated matrix --//
///////////////////////////////

template<typename T>
class MatrixN : public MatrixBase<MatrixN<T>>
{
public:
	using Base = MatrixBase<MatrixN<T>>;

	using ValType = T;
	using RefType = ValType&;
	using PtrType = ValType*;

	//////////////
	//-- ctor --//
	//////////////

	MatrixN() = default;

	MatrixN(size_t line, size_t column) :
		m_Line(line),
		m_Column(column)
	{
		m_Data = new ValType[m_Line * m_Column];
	}

	MatrixN(const MatrixN& other) :
		m_Line(other.Line()),
		m_Column(other.Column())
	{
		m_Data = new ValType[m_Line * m_Column];

		Base::operator=(other);
	}

	MatrixN(MatrixN&& other) :
		m_Line(other.Line()),
		m_Column(other.Column())
	{
		m_Data = other.m_Data;

		other.m_Data   = nullptr;
		other.m_Line   = 0;
		other.m_Column = 0;
	}

	MatrixN(size_t line, size_t column, const std::initializer_list<ValType>& values) :
		MatrixN(line, column)
	{
		Base::operator=(values);
	}

	template<class E>
	MatrixN(const MatrixExpression<E>& expr) :
		m_Line(expr.Line()),
		m_Column(expr.Column())
	{
		m_Data = new ValType[m_Line * m_Column];

		Base::operator=(expr);
	}

	//////////////
	//-- dtor --//
	//////////////

	~MatrixN()
	{
		if (m_Data)
			delete[] m_Data;

		m_Data = nullptr;
	}

	///////////////////////////////
	//-- Override base methods --//
	///////////////////////////////

	inline ValType operator()(size_t i, size_t j) const { return m_Data[i * m_Column + j]; }
	inline RefType operator()(size_t i, size_t j) { return m_Data[i * m_Column + j]; }

	inline size_t Line()   const { return m_Line; }
	inline size_t Column() const { return m_Column; }

	void AssertSquareMatrix() const
	{
		if (m_Line != m_Column)
			throw std::exception("This is not a square matrix.");
	}

	///////////////////////////////
	//-- Assignement operators --//
	///////////////////////////////

	MatrixN& operator=(const MatrixN& other)
	{
		Base::operator=(other);
	}

	/////////////////
	//-- Methods --//
	/////////////////

	void Resize(size_t line, size_t column)
	{
		size_t currentsize = m_Line * m_Column;
		size_t newsize     = line * column;

		if (newsize > currentsize)
		{
			if (m_Data)
				delete[] m_Data;

			m_Data = new ValType[newsize];
		}

		m_Line   = line;
		m_Column = column;
	}

private:
	PtrType m_Data = nullptr;

	size_t m_Line   = 0;
	size_t m_Column = 0;
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
