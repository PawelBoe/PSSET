cmake_minimum_required(VERSION 3.10)
project(ECS)

set(CMAKE_CXX_STANDARD 11)

set(CATCH_DIR "tests/catch")
set(INCLUDE_DIR "PSSET/")
include_directories(${CATCH_DIR} ${INCLUDE_DIR})

set(SOURCE_FILES PSSET/sparse_map.h PSSET/sparse_set.h)

add_executable(TESTS
        tests/TestMain.cpp
        tests/TestCases.cpp
        ${SOURCE_FILES}
        )

add_executable(EXAMPLE
        example.cpp
        ${SOURCE_FILES}
        )
