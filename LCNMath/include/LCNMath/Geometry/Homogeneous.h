#include "LCNMath/Algebra/Matrix.h"

namespace LCN::Math
{
    namespace Details
    {
        enum class ObjectType
        {
            Vector,
            Point
        };

        template<typename _ValType, size_t _Dim, ObjectType _Type>
        class Object : public Algebra::Matrix<_ValType, _Dim + 1, 1>
        {
        public:
            using ValType  = _ValType;
            using BaseType = Algebra::Matrix<_ValType, _Dim + 1, 1>;

            constexpr
            Object()
            {
                for(int i = 0; i < _Dim; ++i)
                    (*this)(i, 0) = ValType{ 0 };
                
                (*this)(_Dim, 0) = static_cast<ValType>(_Type);
            }
        };
    }

    template<typename _ValType, size_t _Dim>
    using Vector = Details::Object<_ValType, _Dim, Details::ObjectType::Vector>;

    template<typename _ValType, size_t _Dim>
    using Point = Details::Object<_ValType, _Dim, Details::ObjectType::Point>;

    template<typename _ValType, size_t _Dim>
    auto operator+(const Point<_ValType, _Dim>&, const Point<_ValType, _Dim>&) = delete;
}