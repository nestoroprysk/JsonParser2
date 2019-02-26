# general

cmake_minimum_required(VERSION 3.12)

project(Playground)

set(CMAKE_CXX_STANDARD 17)

# testing

enable_testing()

find_package(GTest REQUIRED)

# parser library

set(JsonParserDir ${CMAKE_CURRENT_SOURCE_DIR}/Sources)
file(GLOB JsonParserSources ${JsonParserDir}/JsonParser.cpp
							${JsonParserDir}/JsonParser.hpp)
add_library(JsonParser STATIC ${JsonParserSources})

# testing executable

set(TestsDir ${CMAKE_CURRENT_SOURCE_DIR}/Tests)
file(GLOB TestsSources ${TestsDir}/BasicTests.cpp)
add_executable(Tests ${TestsSources})

target_link_libraries(Tests ${JsonParser})
target_link_libraries(Tests ${GTEST_BOTH_LIBRARIES})

target_include_directories(Tests PUBLIC ${JsonParserDir})
target_include_directories(Tests PUBLIC ${GTEST_INCLUDE_DIRS})
