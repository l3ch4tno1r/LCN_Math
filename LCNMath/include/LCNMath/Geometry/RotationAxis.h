#include "LCNMath/Algebra/MatrixExpression.h"

namespace LCN::Math
{
	template<typename T>
	class RotationAxis : public MatrixExpression<RotationAxis<T>>
	{

	};

	template<typename T>
	class Traits<RotationAxis<T>>
	{
		enum
		{
			LineAtCT   = 3,
			ColumnAtCT = 3,
			SizeAtCT   = true
		};
	};
}