#pragma once

#include "../Matrix/Matrix.h"
#include "Vector.h"

namespace LCN
{
	template<typename T, size_t N>
	class Transform : public Matrix<T, N + 1, N + 1>
	{
	public:
		enum
		{
			Dim  = N,
			HDim = N + 1
		};

		using ValType = T;
		using PtrType = T*;
		using RefType = T&;

		using Base = Matrix<T, N + 1, N + 1>;

		using RotationBlockType         = MatrixBlock<Transform, Dim, Dim>;
		using ConstRotationBlockType    = ConstMatrixBlock<Transform, Dim, Dim>;
		using TranslationBlockType      = MatrixBlock<Transform, Dim, 1>;
		using ConstTranslationBlockType = ConstMatrixBlock<Transform, Dim, 1>;

	public:
		Transform() :
			Base(true)
		{}

		Transform(const std::initializer_list<ValType>& list) :
			Base(list)
		{}

		template<class E>
		Transform(const MatrixExpression<E>& other) :
			Base(other)
		{}

		auto Ru()       { static_assert(cm_Ru_Accessible); return this->Columns()[0]; }
		auto Ru() const { static_assert(cm_Ru_Accessible); return this->Columns()[0]; }

		auto Rv()       { static_assert(cm_Rv_Accessible); return this->Columns()[1]; }
		auto Rv() const { static_assert(cm_Rv_Accessible); return this->Columns()[1]; }

		auto Rw()       { static_assert(cm_Rw_Accessible); return this->Columns()[2]; }
		auto Rw() const { static_assert(cm_Rw_Accessible); return this->Columns()[2]; }

		auto Tr()       { return this->Columns()[Dim]; }
		auto Tr() const { return this->Columns()[Dim]; }

		RotationBlockType      RotationBlock()       { return RotationBlockType(*this, 0, 0); }
		ConstRotationBlockType RotationBlock() const { return ConstRotationBlockType(*this, 0, 0); }

		TranslationBlockType      TranslationBlock()       { return TranslationBlockType(*this, 0, Dim); }
		ConstTranslationBlockType TranslationBlock() const { return ConstTranslationBlockType(*this, 0, Dim); }

		// Works only for transform because the rotation part is orthogonal
		Transform QuickInverse() const
		{
			Transform inverse;

			auto r = this->RotationBlock();
			auto t = this->TranslationBlock();

			auto ir = inverse.RotationBlock();
			auto it = inverse.TranslationBlock();

			ir = r.Transpose();
			it = -(ir * t);

			return inverse;
		}

	private:
		enum
		{
			cm_Ru_Accessible = N >= 1,
			cm_Rv_Accessible = N >= 2,
			cm_Rw_Accessible = N >= 3
		};
	};

	template<typename T, size_t N>
	class Traits<Transform<T, N>> : public Traits<Matrix<T, N + 1, N + 1>>
	{
	public:
		using ValType = T;
	};

	typedef Transform<float,  2> Transform2Df;
	typedef Transform<double, 2> Transform2Dd;
	typedef Transform<float,  3> Transform3Df;
	typedef Transform<double, 3> Transform3Dd;
}