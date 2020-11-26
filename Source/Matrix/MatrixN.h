#pragma once

#include "MatrixBase.h"

///////////////////////////////
//-- Heap allocated matrix --//
///////////////////////////////

template<typename T>
class MatrixN : public MatrixBase<MatrixN<T>>
{
public:
	using ValType = T;
	using RefType = ValType & ;
	using PtrType = ValType * ;

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
