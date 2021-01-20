#pragma once

#include <algorithm>
#include <type_traits>

#include "MatrixExpression.h"

namespace LCN
{
	template<class Derived>
	class MatrixBase : public MatrixExpression<Derived>
	{
	public:
		using ValType = typename Traits<Derived>::ValType;
		using RefType = ValType&;

		inline RefType operator()(size_t i, size_t j) { return this->Derived()(i, j); }

		template<class E>
		Derived& operator=(const MatrixExpression<E>& expr)
		{
			using t_Derived = Traits<Derived>;
			using t_Expr    = MatrixExpression<E>;

			if constexpr (t_Derived::SizeAtCT && t_Expr::SizeAtCT)
				static_assert((t_Derived::LineAtCT == t_Expr::LineAtCT) && (t_Derived::ColumnAtCT == t_Expr::ColumnAtCT));
			else
				ASSERT((this->Line() == expr.Line()) && (this->Column() == expr.Column()));

			for (size_t i = 0; i < expr.Line(); ++i)
				for (size_t j = 0; j < expr.Column(); ++j)
					(*this)(i, j) = expr(i, j);

			return this->Derived();
		}

		Derived& operator=(const std::initializer_list<ValType>& values)
		{
			if constexpr (!Traits<Derived>::SizeAtCT)
				ASSERT(this->Line() > 0 && this->Column() > 0);

			size_t Idx = 0;

			for (ValType e : values)
			{
				if (Idx > this->Line() * this->Column())
					break;

				size_t i = Idx / this->Column();
				size_t j = Idx % this->Column();

				(*this)(i, j) = e;

				++Idx;
			}

			return this->Derived();
		}

		/////////////////
		//-- Methods --//
		/////////////////

		void SwapLines(size_t i, size_t j)
		{
			if (i >= this->Line() || j >= this->Line())
				throw std::out_of_range("Index out of range.");

			ValType temp;

			for (int k = 0; k < this->Column(); k++)
			{
				temp = this->Derived()(i, k);

				this->Derived()(i, k) = this->Derived()(j, k);
				this->Derived()(j, k) = temp;
			}
		}

		void ScaleLine(size_t idx, ValType scalefactor)
		{
			if (idx >= this->Line())
				throw std::out_of_range("Index out of range.");

			for (size_t j = 0; j < this->Column(); j++)
				this->Derived()(idx, j) *= scalefactor;
		}

		void CombineLines(size_t idx1, ValType factor1, size_t idx2, ValType factor2)
		{
			if (idx1 >= this->Line() || idx2 >= this->Line())
				throw std::out_of_range("Index out of range.");

			for (size_t j = 0; j < this->Column(); j++)
				this->Derived()(idx1, j) = factor1 * this->Derived()(idx1, j) + factor2 * this->Derived()(idx2, j);
		}

		ValType GaussElimination()
		{
			static_assert(std::is_floating_point<ValType>::value, "ValType must be a floating point type.");

			size_t  linepivot    = 0;
			size_t  permutations = 0;
			ValType pseudodet(1);

			size_t L = this->Line();
			size_t C = this->Column();

			for (size_t j = 0; j < std::min(L, C); j++)
			{
				// Recherche du pivot
				ValType max = 0;
				int     maxpos = 0;

				for (size_t i = linepivot; i < L; i++)
				{
					if (std::abs(this->Derived()(i, j)) > max)
					{
						max = abs(this->Derived()(i, j));
						maxpos = i;
					}
				}

				// maxpos est le pivot
				if (this->Derived()(maxpos, j) == 0)
					return ValType(0);

				pseudodet *= this->Derived()(maxpos, j);

				ScaleLine(maxpos, ValType(1) / (this->Derived()(maxpos, j)));

				if (maxpos != j)
				{
					SwapLines(maxpos, linepivot);
					permutations++;
				}

				for (size_t i = 0; i < L; i++)
					if (i != linepivot)
						CombineLines(i, ValType(1), linepivot, -(this->Derived()(i, j)));

				linepivot++;
			}

			return (permutations % 2 == 0 ? ValType(1) : ValType(-1)) * pseudodet;
		}

		////////////////////////////////////////
		//-- Square matrix specific methods --//
		////////////////////////////////////////

		inline bool IsSquareMatrix() const { return this->Line() == this->Column(); }

		inline void AssertSquareMatrix() const { this->Derived().AssertSquareMatrix(); }

		ValType Trace() const
		{
			this->AssertSquareMatrix();

			ValType result(0);

			for (size_t i = 0; i < this->Line(); i++)
				result += this->Derived()(i, i);

			return result;
		}

		ValType Det() const
		{
			this->AssertSquareMatrix();

			Derived temp = *this;

			return temp.GaussElimination();
		}

		Derived Invert() const
		{
			this->AssertSquareMatrix();

			auto temp = Derived::Matrix2C();

			size_t L = this->Line();
			size_t C = this->Column();

			for (size_t i = 0; i < L; i++)
				for (size_t j = 0; j < C; j++)
					temp(i, j) = this->Derived()(i, j);

			for (size_t i = 0; i < L; i++)
				for (size_t j = C; j < 2 * C; j++)
					temp(i, j) = (i == j - C ? ValType(1) : ValType(0));

			ValType pseudodet = temp.GaussElimination();

			if (std::abs(pseudodet) < ValType(0.0001))
				throw std::exception("This matrix cannot be inverted.");

			Derived result;

			for (size_t i = 0; i < L; i++)
				for (size_t j = 0; j < C; j++)
					result(i, j) = temp(i, j + C);

			return result;
		}
	};
}