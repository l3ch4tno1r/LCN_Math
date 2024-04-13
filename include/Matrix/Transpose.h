#pragma once

#include <type_traits>

#include "MatrixBase.h"

namespace LCN
{
	template<class SubjectMatrix>
	class Transpose : public MatrixBase<Transpose<SubjectMatrix>>
	{
#pragma region Static Asserts
		static_assert(!std::is_reference_v<SubjectMatrix>);
		static_assert(!std::is_pointer_v<SubjectMatrix>);

		static_assert(Traits<SubjectMatrix>::SizeAtCT, "Size of subject matrix must be known at compile time.");
#pragma endregion
	public:
		using ValType = typename Traits<SubjectMatrix>::ValType;

		Transpose(const SubjectMatrix& subject) :
			m_Subject(subject)
		{}

		inline constexpr size_t Line()   const { return Traits<SubjectMatrix>::ColumnAtCT; }
		inline constexpr size_t Column() const { return Traits<SubjectMatrix>::LineAtCT; }

		ValType operator()(size_t i, size_t j) const { return m_Subject(j, i); }

	private:
		const SubjectMatrix& m_Subject;
	};

	template<class SubjectMatrix>
	class Traits<Transpose<SubjectMatrix>>
	{
	public:
		enum
		{
			SizeAtCT   = true,
			LineAtCT   = Traits<SubjectMatrix>::ColumnAtCT,
			ColumnAtCT = Traits<SubjectMatrix>::LineAtCT
		};

		using ValType = typename Traits<SubjectMatrix>::ValType;
	};
}