#include <gtest/gtest.h>

#include "LCNMath/Geometry/Homogeneous.h"

template<typename _ValType, size_t _Dim, LCN::Math::Details::ObjectType _Type>
std::ostream&
operator<<(
    std::ostream& os,
    const LCN::Math::Details::Object<_ValType, _Dim, _Type>& obj)
{
    os << '(' << obj(0, 0);

    for(int i = 1; i < obj.Line(); ++i)
        os << ", " << obj(i, 0);
    
    os << ')';

    return os;
}

namespace LCN::Math::UnitTests
{
    TEST(Homogeneous, Nominale)
    {
        const Vector<float, 2> vec;
        const Point<float, 2> pt;

        std::cout << vec << std::endl;
        std::cout << pt  << std::endl;

        auto r = pt + vec + pt;

        std::cout << r << std::endl;
    }
}