#pragma once

#include "MatrixBase.h"

namespace LCN
{
	template<class Subject, size_t L, size_t C>
	class StaticMatrixView;

	template<class Derived, size_t L, size_t C>
	class StaticMatrixBase : public MatrixBase<Derived>
	{
	public:
		using ValType = typename Traits<Derived>::ValType;

		constexpr size_t Line()   const { return L; }
		constexpr size_t Column() const { return C; }

		static constexpr void AssertSquareMatrix() { static_assert(L == C, "This is not a square matrix."); }

		template<size_t L1, size_t C1>
		using ViewType = StaticMatrixView<Derived, L1, C1>;

		template<size_t L1, size_t C1>
		ViewType<L1, C1> View(size_t xoffset, size_t yoffset) const
		{
			ASSERT(xoffset + L1 - 1 < L); 
			ASSERT(yoffset + C1 - 1 < C);

			return ViewType<L1, C1>(this->Derived(), xoffset, yoffset);
		}
	};

	template<class Derived, size_t L, size_t C>
	class Traits<StaticMatrixBase<Derived, L, C>>
	{
	public:
		enum
		{
			SizeAtCT   = true,
			LineAtCT   = L,
			ColumnAtCT = C
		};
	};

	template<class Subject, size_t L, size_t C>
	class StaticMatrixView : public StaticMatrixBase<StaticMatrixView<Subject, L, C>, L, C>
	{
	public:
		using ValType = typename Traits<Subject>::ValType;
		using PtrType = ValType*;
		using RefType = ValType&;

	public:
		StaticMatrixView(const Subject& subject, size_t xoffset, size_t yoffset) :
			m_Subject(subject),
			m_X_Offset(xoffset),
			m_Y_Offset(yoffset)
		{}

		StaticMatrixView(const StaticMatrixView& other) :
			m_Subject(other.m_Subject),
			m_X_Offset(other.m_X_Offset),
			m_Y_Offset(other.m_Y_Offset)
		{}

		StaticMatrixView(StaticMatrixView&& other) :
			m_Subject(other.m_Subject),
			m_X_Offset(other.m_X_Offset),
			m_Y_Offset(other.m_Y_Offset)
		{}

		ValType operator()(size_t i, size_t j) const { return m_Subject(i + m_X_Offset, j + m_Y_Offset); }

	private:
		const Subject& m_Subject;
		size_t m_X_Offset;
		size_t m_Y_Offset;
	};

	template<class Subject, size_t L, size_t C>
	class Traits<StaticMatrixView<Subject, L, C>> : public Traits<StaticMatrixBase<StaticMatrixView<Subject, L, C>, L, C>>
	{
	public:
		using ValType = typename Traits<Subject>::ValType;
	};
}