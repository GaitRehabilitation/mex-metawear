set(Metawear_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/MetaWear-SDK-Cpp/src PARENT_SCOPE)
set(Libblepp_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/libblepp/ PARENT_SCOPE)

cmake_minimum_required(VERSION 2.8.3)
PROJECT(MetaWear-SDK)
file(GLOB_RECURSE Mbient_SOURCE MetaWear-SDK-Cpp/src/metawear/**/*.cpp)
file(GLOB_RECURSE Mbient_HEADERS MetaWear-SDK-Cpp/src/metawear/**/*.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/MetaWear-SDK-Cpp/src)
add_library(MetaWear-SDK SHARED ${Mbient_SOURCE} ${Mbient_HEADERS})

add_subdirectory(libblepp)