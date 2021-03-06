#pragma once

#include <initializer_list>

#include "../../Matrix/Stack/SqrSMatrix.h"
#include "../../Utilities/Angles.h"

namespace LCNMath {
	namespace Geometry {
		namespace Dim2 {

			template<typename T>
			using SMatrix33 = LCNMath::Matrix::StaticMatrix::Matrix<T, 3, 3>;

			template<typename T>
			using SqrSMatrix33 = LCNMath::Matrix::StaticMatrix::SqrMatrix<T, 3>;

			template<typename T>
			union Transform2D
			{
				struct
				{
					T Rux, Rvx, Tx,
					  Ruy, Rvy, Ty;

					// Representation of :
					// [ Rux, Rvx, Tx ]
					// [ Ruy, Rvy, Ty ]
					// [  0,   0,   1 ]
			;	};

				SqrSMatrix33<T> mat;

				Transform2D() :
					mat(true)
				{}

				Transform2D(const SMatrix33<T>& _mat) :
					mat(_mat)
				{}

				Transform2D(const SqrSMatrix33<T>& _mat) :
					mat(_mat)
				{}

				Transform2D(T x, T y, T a) :
					mat(true)
				{
					SetTranslation(x, y);
					SetRotationAngle(a);
				}

				Transform2D(const std::initializer_list<T>& list)
				{
					size_t i = 0;

					for (const T& e : list)
					{
						if (i > mat.Lines() * mat.Columns())
							return;

						size_t I = i / mat.Columns();
						size_t J = i % mat.Columns();

						mat(I, J) = e;

						++i;
					}
				}

				Transform2D& operator=(const Transform2D& other)
				{
					mat = other.mat;

					return *this;
				}

				void SetRotationAngle(float a)
				{
					Rux =  std::cos(TORAD(a));
					Ruy =  std::sin(TORAD(a));
					Rvx = -std::sin(TORAD(a));
					Rvy =  std::cos(TORAD(a));
				}

				void SetTranslation(T x, T y)
				{
					Tx = x;
					Ty = y;
				}
			};
		}
	}
}

