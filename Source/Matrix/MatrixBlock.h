#pragma once

#include <type_traits>

#include "MatrixBase.h"

namespace LCN
{
	//////////////////////
	//-- Matrix Block --//
	//////////////////////

	template<class SubjectMatrix, size_t L, size_t C>
	class MatrixBlock : public MatrixBase<MatrixBlock<SubjectMatrix, L, C>>
	{
#pragma region Static Asserts
		static_assert(!std::is_reference_v<SubjectMatrix>);
		static_assert(!std::is_pointer_v<SubjectMatrix>);

		static_assert(Traits<SubjectMatrix>::SizeAtCT, "Size of subject matrix must be known at compile time.");

		static_assert(L <= Traits<SubjectMatrix>::LineAtCT);
		static_assert(C <= Traits<SubjectMatrix>::ColumnAtCT);

		//static_assert(m_IOffset < Traits<SubjectMatrix>::LineAtCT);
		//static_assert(m_JOffset < Traits<SubjectMatrix>::ColumnAtCT);
		//
		//static_assert(m_IOffset + L <= Traits<SubjectMatrix>::LineAtCT);
		//static_assert(m_JOffset + C <= Traits<SubjectMatrix>::ColumnAtCT);

#pragma endregion

	public:
		using Base    = MatrixBase<MatrixBlock<SubjectMatrix, L, C>>;
		using ValType = typename Traits<SubjectMatrix>::ValType;
		using RefType = ValType&;

		MatrixBlock(SubjectMatrix& subject, size_t ioffset, size_t joffset) :
			m_Subject(subject),
			m_IOffset(ioffset),
			m_JOffset(joffset)
		{}

		///////////////////
		//-- Accessors --//
		///////////////////

		inline constexpr size_t Line()   const { return L; }
		inline constexpr size_t Column() const { return C; }

		RefType operator()(size_t i, size_t j)       { return m_Subject(i + m_IOffset, j + m_JOffset); }
		ValType operator()(size_t i, size_t j) const { return m_Subject(i + m_IOffset, j + m_JOffset); }

		/////////////////////
		//-- Assignement --//
		/////////////////////

		MatrixBlock& operator=(const MatrixBlock&) = default;

		template<class E>
		inline MatrixBlock& operator=(const MatrixExpression<E>&expr)
		{
			return Base::operator=(expr);
		}

		inline MatrixBlock& operator=(const std::initializer_list<ValType>&values)
		{
			return Base::operator=(values);
		}

	private:
		SubjectMatrix& m_Subject;
		size_t m_IOffset;
		size_t m_JOffset;
	};

	template<class SubjectMatrix, size_t L, size_t C>
	class ConstMatrixBlock : public MatrixBase<ConstMatrixBlock<SubjectMatrix, L, C>>
	{
	public:
		using ValType = typename Traits<SubjectMatrix>::ValType;

		ConstMatrixBlock(const SubjectMatrix& subject, size_t ioffset, size_t joffset) :
			m_Subject(subject),
			m_IOffset(ioffset),
			m_JOffset(joffset)
		{}

		///////////////////
		//-- Accessors --//
		///////////////////

		inline constexpr size_t Line()   const { return L; }
		inline constexpr size_t Column() const { return C; }

		ValType operator()(size_t i, size_t j) const { return m_Subject(i + m_IOffset, j + m_JOffset); }

	private:
		const SubjectMatrix& m_Subject;
		size_t m_IOffset;
		size_t m_JOffset;
	};

	////////////////
	//-- Traits --//
	////////////////

	template<class SubjectMatrix, size_t L, size_t C>
	class Traits<MatrixBlock<SubjectMatrix, L, C>>
	{
	public:
		enum
		{
			SizeAtCT   = true,
			LineAtCT   = L,
			ColumnAtCT = C
		};

		using ValType = typename Traits<SubjectMatrix>::ValType;
	};

	template<class SubjectMatrix, size_t L, size_t C>
	class Traits<ConstMatrixBlock<SubjectMatrix, L, C>>
	{
	public:
		enum
		{
			SizeAtCT   = true,
			LineAtCT   = L,
			ColumnAtCT = C
		};

		using ValType = typename Traits<SubjectMatrix>::ValType;
	};

	///////////////////////
	//-- Column Vector --//
	///////////////////////

	template<class SubjectMatrix>
	class ColVectors
	{
	public:
		using ColVectorType = MatrixBlock<
			SubjectMatrix,
			Traits<SubjectMatrix>::LineAtCT,
			1>;

		ColVectors(SubjectMatrix& subject) :
			m_Subject(subject)
		{}

		inline ColVectorType operator[](size_t i) { return ColVectorType(m_Subject, 0, i); }

	private:
		SubjectMatrix& m_Subject;
	};

	template<class SubjectMatrix>
	class ConstColVectors
	{
	public:
		using ConstColVectorType = ConstMatrixBlock<
			SubjectMatrix,
			Traits<SubjectMatrix>::LineAtCT,
			1>;

		ConstColVectors(const SubjectMatrix& subject) :
			m_Subject(subject)
		{}

		inline ConstColVectorType operator[](size_t i) const { return ConstColVectorType(m_Subject, 0, i); }

	private:
		const SubjectMatrix& m_Subject;
	};

	////////////////////
	//-- Row Vector --//
	////////////////////

	template<class SubjectMatrix>
	class RowVectors
	{
	public:
		using RowVectorType = MatrixBlock<
			SubjectMatrix,
			1,
			Traits<SubjectMatrix>::ColumnAtCT>;

		RowVectors(SubjectMatrix& subject) :
			m_Subject(subject)
		{}

		inline RowVectorType operator[](size_t i) { return RowVectorType(m_Subject, i, 0); }

	private:
		SubjectMatrix& m_Subject;
	};

	template<class SubjectMatrix>
	class ConstRowVectors
	{
	public:
		using ConstRowVectorType = ConstMatrixBlock<
			SubjectMatrix,
			1,
			Traits<SubjectMatrix>::ColumnAtCT>;

		ConstRowVectors(const SubjectMatrix& subject) :
			m_Subject(subject)
		{}

		inline ConstRowVectorType operator[](size_t i) const { return ConstRowVectorType(m_Subject, i, 0); }

	private:
		const SubjectMatrix& m_Subject;
	};
}
