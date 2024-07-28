add_library(LCNMathLib INTERFACE)

target_include_directories(
    LCNMathLib
    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

install(
    DIRECTORY   include/
    DESTINATION include)

install(
    TARGETS  LCNMathLib
    EXPORT   LCNMathLibTargets
    INCLUDES DESTINATION include())

install(
    EXPORT LCNMathLibTargets
    FILE LCNMathLibTargets.cmake
    NAMESPACE LCNMath::
    DESTINATION lib/cmake/LCNMathLib)