cmake_minimum_required(VERSION 3.15)
project(qp CXX)

option(QP_BUILD_TESTS "Build tests" ON)

if(QP_BUILD_TESTS)
    include(CTest)
    enable_testing()
endif()

add_library(qp src/qp_encode.cpp src/qp_decode.cpp)
target_include_directories(qp PUBLIC include)
set_property(TARGET qp PROPERTY CXX_STANDARD 20)
set_property(TARGET qp PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET qp PROPERTY CXX_EXTENSIONS OFF)
target_compile_definitions(qp PRIVATE EXPORT_QP_DLL)

if(QP_BUILD_TESTS)
    add_subdirectory(tests)
endif()

set_target_properties(qp PROPERTIES PUBLIC_HEADER "include/qp.hpp")
install(TARGETS qp)
