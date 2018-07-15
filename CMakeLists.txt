cmake_minimum_required(VERSION 3.10)
project(mex_metawear)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(Matlab REQUIRED)
find_package(Qt5 COMPONENTS Bluetooth REQUIRED)

if(NOT Matlab_FOUND)
    message(FATAL_ERROR "Matlab not found")
endif()


#add_subdirectory(3rdparty/libblepp)
add_subdirectory(library)
add_subdirectory(library/matlab)