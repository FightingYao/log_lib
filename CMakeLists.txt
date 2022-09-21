cmake_minimum_required(VERSION 3.0)
# The Project name
project(log_lib)
# The output directory of binary.
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# set compile flag.
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS )
set(CMAKE_C_FLAGS -g -O3 -Wl,-rpath='../lib')

# set -I
include_directories(./code)
include_directories(./include)

# set -L
link_directories(./lib)

# cpp depended files
aux_source_directory(./code MAIN_CODE)

add_executable(${PROJECT_NAME}
        ${MAIN_CODE})

# The library that the project dependents to.
set(LINK_LIB
        pthread
#        libutility.a
        )

#
target_link_libraries(${PROJECT_NAME}
        ${LINK_LIB}
        )
