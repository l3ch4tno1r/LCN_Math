set(THIS LCNMath_UT)

set(CMAKE_CXX_STANDARD 17)

find_package(GTest REQUIRED)

add_executable(${THIS}
    src/UT01_Constructor.cpp)

target_link_libraries(${THIS} PRIVATE
    GTest::gtest
    GTest::gtest_main
    LCNMathLib)