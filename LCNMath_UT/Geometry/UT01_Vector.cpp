#include <gtest/gtest.h>

#include "LCNMath/Geometry/Vector.h"

namespace LCN::Math::UnitTests
{
    TEST(Vector, Nominal)
    {
        using HVectorType = HVectorND<int, 2>;

        HVectorType vec1{ 1, 2, 3 };
        HVectorType vec2{ 3, 2, 1 };
        
        EXPECT_NE(vec1, vec2);

        auto sum = vec1 + vec2;
        
        EXPECT_EQ(sum, (HVectorType{ 4, 4, 4 }));
    }
}