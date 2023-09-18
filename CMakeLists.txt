cmake_minimum_required(VERSION 3.6.0)
project(qp VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

add_executable(qp main.cpp qp.cpp qp.hpp)
set_property(TARGET qp PROPERTY CXX_STANDARD 23)
set_property(TARGET qp PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET qp PROPERTY CXX_EXTENSIONS OFF)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
