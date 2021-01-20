#pragma once

#include "MatrixBase.h"

namespace LCN
{
	template<class SubjectMatrix, size_t IOffset, size_t JOffset, size_t L, size_t C>
	class MatrixBlock : public MatrixBase<MatrixBlock<SubjectMatrix, IOffset, JOffset, L, C>>
	{
		static_assert(Traits<SubjectMatrix>::SizeAtCT, "Size of subject matrix must be known at compile time.");
		// TODO : static_assert the validity of the block

	public:
		using ValType = typename Traits<SubjectMatrix>::ValType;
		using RefType = ValType&;

		MatrixBlock(SubjectMatrix& subject) :
			m_Subject(subject)
		{}

		inline constexpr size_t Line()   const { return L; }
		inline constexpr size_t Column() const { return C; }

		RefType operator()(size_t i, size_t j)       { return m_Subject(i + IOffset, j + JOffset); }
		ValType operator()(size_t i, size_t j) const { return m_Subject(i + IOffset, j + JOffset); }

	private:
		SubjectMatrix& m_Subject;
	};

	template<class SubjectMatrix, size_t IOffset, size_t JOffset, size_t L, size_t C>
	class Traits<MatrixBlock<SubjectMatrix, IOffset, JOffset, L, C>>
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
}
