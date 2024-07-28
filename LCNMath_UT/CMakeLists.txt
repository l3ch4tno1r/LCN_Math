set(THIS LCNMath_UT)

set(CMAKE_CXX_STANDARD 17)

find_package(GTest REQUIRED)

add_executable(${THIS}
    Algebra/UT01_Constructor.cpp
    Algebra/UT02_DataAccess.cpp
    Algebra/UT03_Modifiers.cpp
    Algebra/UT04_MatrixComparision.cpp
    Geometry/UT01_Vector.cpp
    Geometry/UT02_Transform.cpp
    .experimental/UT01_Homogeneous.cpp)

target_link_libraries(${THIS} PRIVATE
    GTest::gtest
    GTest::gtest_main
    LCNMathLib)