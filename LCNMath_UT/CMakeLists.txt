set(THIS LCNMath_UT)

set(CMAKE_CXX_STANDARD 17)

find_package(GTest REQUIRED)

add_executable(${THIS}
    src/UT01_Constructor.cpp
    src/UT02_DataAccess.cpp
    src/UT03_Modifiers.cpp
    src/UT04_MatrixComparision.cpp)

target_link_libraries(${THIS} PRIVATE
    GTest::gtest
    GTest::gtest_main
    LCNMathLib)