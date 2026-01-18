cmake_minimum_required(VERSION 3.16)

project(StringUtilsTest VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Test)

set(SOURCES
    ../StringUtils.cpp
    main.cpp
)

set(HEADERS
    ../StringUtils.h
)

add_executable(stringutils_test ${SOURCES} ${HEADERS})

target_link_libraries(stringutils_test Qt6::Core Qt6::Test)

enable_testing()
add_test(NAME StringUtilsTest COMMAND stringutils_test)