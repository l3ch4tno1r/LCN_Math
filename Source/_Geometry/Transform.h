#pragma once

#include "Vector.h"

template<typename T, size_t Dim>
class Transform
{};

template<typename T>
class Transform<T, 3> : public StaticMatrixBase<Transform<T, 3>, T, 4, 4>
{
public:
	using ValType = T;
	using PtrType = T*;
	using RefType = T&;

	using HVector3DT = VectorND<T, 3, HomogeneousVector>;

public:
	Transform() = default;

	Transform(const std::initializer_list<T>& list)
	{
		size_t idx = 0;

		for (auto e : list)
		{
			size_t i = idx / 4;
			size_t j = idx % 4;

			m_Vectors[j][i] = e;

			++idx;
		}
	}

	RefType operator()(size_t i, size_t j) { return m_Vectors[j][i]; }
	ValType operator()(size_t i, size_t j) const { return m_Vectors[j][i]; }

	HVector3DT& operator[](size_t i) { return m_Vectors[i]; }
	const HVector3DT& operator[](size_t i) const { return m_Vectors[i]; }

private:
	HVector3DT m_Vectors[4];
};