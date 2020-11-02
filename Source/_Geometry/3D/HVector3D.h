#pragma once

#include "../VectorBase.h"

template<typename T>
struct HVector3D : public VectorND<HVector3D<T>, T, 3>
{
	union
	{
		struct
		{
			T x, y, z, s;
		};
		T mat[4]
	};

	constexpr size_t Line()   const { return 4; }
	constexpr size_t Column() const { return 1; }
};