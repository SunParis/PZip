cmake_minimum_required(VERSION 3.21)
# project info
project(PZip)

# CTest
enable_testing()

# settings
set(CPP_MODE ON)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)

# disable warnings
add_compile_options(-Wall -Wno-register -Wno-unused-function -Wno-unused-variable -g)

# project include directories
include_directories(include)

# all of C/C++ source files
file(GLOB_RECURSE C_SOURCES "src/*.c")
file(GLOB_RECURSE CXX_SOURCES "src/*.cpp")
file(GLOB_RECURSE CC_SOURCES "src/*.cc")
set(SOURCES ${C_SOURCES} ${CXX_SOURCES} ${CC_SOURCES})

# executable
add_executable(PZip ${SOURCES})
set_target_properties(PZip PROPERTIES C_STANDARD 11 CXX_STANDARD 20)
