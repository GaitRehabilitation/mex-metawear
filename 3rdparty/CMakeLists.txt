file(GLOB_RECURSE Mbient_SOURCE MetaWear-SDK-Cpp/src/metawear/**/*.cpp)
file(GLOB_RECURSE Mbient_HEADERS MetaWear-SDK-Cpp/src/metawear/**/*.h)

add_library(extern_metawear ${Mbient_SOURCE} ${Mbient_HEADERS})
target_include_directories(extern_metawear PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/MetaWear-SDK-Cpp/src)


file(GLOB_RECURSE Mbient_Warble_SOURCE Warble/src/warble/**/*.cpp)
file(GLOB_RECURSE Mbient_Warble_HEADERS Warble/src/warble/**/*.h)

add_library(extern_warble ${Mbient_Warble_SOURCE} ${Mbient_Warble_HEADERS})
target_include_directories(extern_warble PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Warble/src)
