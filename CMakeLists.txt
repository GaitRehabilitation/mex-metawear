cmake_minimum_required(VERSION 3.8.2)
project(mex_metawear)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_BUILD_TYPE Debug)

find_package(Matlab REQUIRED MX_LIBRARY ENG_LIBRARY )
ADD_DEFINITIONS(-DMATLAB_MEX_FILE)

if(NOT Matlab_FOUND)
    message(FATAL_ERROR "Matlab not found")
endif()


add_subdirectory(3rdparty)
add_subdirectory(library)
add_subdirectory(library/matlab)