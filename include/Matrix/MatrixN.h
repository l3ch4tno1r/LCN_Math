#pragma once

#include "MatrixBase.h"

namespace LCN
{
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
			m_Column(column),
			m_Capacity(line * column)
		{
			m_Data = new ValType[m_Capacity];
		}

		MatrixN(const MatrixN& other) :
			MatrixN(other.m_Line, other.m_Column)
		{
			Base::operator=(other);
		}

		MatrixN(MatrixN&& other) :
			m_Line(other.Line()),
			m_Column(other.Column()),
			m_Capacity(other.m_Capacity),
			m_Data(other.m_Data)
		{
			other.m_Line     = 0;
			other.m_Column   = 0;
			other.m_Capacity = 0;
			other.m_Data     = nullptr;
		}

		MatrixN(size_t line, size_t column, const std::initializer_list<ValType>& values) :
			MatrixN(line, column)
		{
			Base::operator=(values);
		}

		template<class E>
		MatrixN(const MatrixExpression<E>& expr) :
			MatrixN(expr.Line(), expr.Column())
		{
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

		inline MatrixN& operator=(const MatrixN& other)
		{
			this->Resize(other.m_Line, other.m_Column);

			return Base::operator=(static_cast<const MatrixExpression<MatrixN>&>(other));
		}

		inline MatrixN& operator=(MatrixN&& other)
		{
			if (this == &other)
				return *this;

			if (m_Data)
				delete[] m_Data;

			m_Line     = other.m_Line;
			m_Column   = other.m_Column;
			m_Capacity = other.m_Capacity;
			m_Data     = other.m_Data;

			other.m_Line     = 0;
			other.m_Column   = 0;
			other.m_Capacity = 0;
			other.m_Data     = nullptr;

			return *this;
		}

		template<class E>
		inline MatrixN& operator=(const MatrixExpression<E>& expr)
		{
			this->Resize(expr.Line(), expr.Column());

			return Base::operator=(expr);
		}

		///////////////////////////////////
		//-- Memory management methods --//
		///////////////////////////////////

		void Resize(size_t line, size_t column)
		{
			size_t newsize = line * column;

			if (newsize > m_Capacity)
			{
				if (m_Data)
					delete[] m_Data;

				m_Data = new ValType[newsize];

				m_Capacity = newsize;
			}

			m_Line   = line;
			m_Column = column;
		}

		void ShrinkToFit()
		{
			size_t size = m_Line * m_Column;

			if (m_Capacity <= size)
				return;

			PtrType temp = m_Data;

			m_Data = new ValType[size];

			std::memcpy(m_Data, temp, size * sizeof(ValType));

			delete[] temp;

			m_Capacity = size;
		}

		size_t Capacity() const { return m_Capacity; }

	private:
		PtrType m_Data = nullptr;

		size_t m_Line     = 0;
		size_t m_Column   = 0;
		size_t m_Capacity = 0;
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
}