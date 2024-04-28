#include <gtest/gtest.h>

#include "LCNMath/Matrix/Matrix.h"

TEST(DataAccess, NominalRead)
{
    const LCN::Matrix<float, 2, 3> mat{
        1.0f, 2.0f, 3.0f,
        4.0f, 5.0f, 6.0f
    };

    ASSERT_EQ(1.0f, mat(0, 0));
    ASSERT_EQ(2.0f, mat(0, 1));
    ASSERT_EQ(3.0f, mat(0, 2));

    ASSERT_EQ(4.0f, mat(1, 0));
    ASSERT_EQ(5.0f, mat(1, 1));
    ASSERT_EQ(6.0f, mat(1, 2));
}

TEST(DataAccess, NominalWrite)
{
    LCN::Matrix<float, 2, 3> mat{};

    ASSERT_EQ(0.0f, mat(0, 1));

    mat(0, 1) = 8.0f;

    ASSERT_EQ(8.0f, mat(0, 1));
}