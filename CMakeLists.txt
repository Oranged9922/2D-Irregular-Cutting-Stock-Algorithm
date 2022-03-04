cmake_minimum_required(VERSION 3.10)

# project name
project(2D-Irregular-Cutting-Stock-Algorithm)

SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "C:/local/boost_1_78_0/")
SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "C:/local/boost_1_78_0/boost")

find_package( Boost REQUIRED )


if ( NOT Boost_FOUND )
  message(STATUS "This project requires the Boost library, and will not be compiled.")
  return()  
endif()
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
ADD_DEFINITIONS( "-DHAS_BOOST" )

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# executable file
add_executable(run main.cpp)

# headers implementations
add_subdirectory(src)

target_link_libraries(
    run
    bin_packing
)

target_include_directories(
    run
    PUBLIC
    "${PROJECT_SOURCE_DIR}/include"
)

# Enable testing globally. Individual tests may be found in the `tests`
# subdirectory of the repository.
enable_testing()
add_subdirectory(tests)