cmake_minimum_required(VERSION 3.12)
project(mex_metawear)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(Matlab REQUIRED MX_LIBRARY ENG_LIBRARY )
ADD_DEFINITIONS(-DMATLAB_MEX_FILE)


if(NOT Matlab_FOUND)
    message(FATAL_ERROR "Matlab not found")
endif()


if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /ZW")
endif (MSVC)

add_subdirectory(3rdparty)
add_subdirectory(library)
add_subdirectory(library/matlab)