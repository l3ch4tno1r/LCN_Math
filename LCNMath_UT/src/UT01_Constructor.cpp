#include <gtest/gtest.h>

#include "LCNMath/Matrix/Matrix.h"

TEST(Constructor, Default)
{
    LCN::Matrix<float, 3, 4> mat1;
    LCN::Matrix<float, 8, 6> mat2;

    ASSERT_EQ(3u, mat1.Line());
    ASSERT_EQ(4u, mat1.Column());

    ASSERT_EQ(8u, mat2.Line());
    ASSERT_EQ(6u, mat2.Column());

    LCN::Matrix<float, 2, 3> mat3(0.0f);

    for (unsigned int i = 0; i < 6; i++)
    {
        unsigned int I = i / 3;
        unsigned int J = i % 3;

        ASSERT_EQ(0.0f, mat3(I, J));
    }
    
    //ASSERT_EQ(LCN::Matrix<float, 2, 3>::Zero(), mat3);
}

TEST(Constructor, Copy)
{
    LCN::Matrix<float, 2, 3> mat1{
        1.0f, 2.0f, 3.0f,
        4.0f, 5.0f, 6.0f
    };

    ASSERT_EQ(2u, mat1.Line());
    ASSERT_EQ(3u, mat1.Column());

    for (unsigned int i = 0; i < 6; i++)
    {
        unsigned int I = i / 3;
        unsigned int J = i % 3;

        ASSERT_EQ((float)(i + 1), mat1(I, J));
    }

    auto mat2 = mat1;

    ASSERT_EQ(2u, mat2.Line());
    ASSERT_EQ(3u, mat2.Column());

    for (unsigned int i = 0; i < 6; i++)
    {
        unsigned int I = i / 3;
        unsigned int J = i % 3;

        ASSERT_EQ((float)(i + 1), mat2(I, J));
    }
}