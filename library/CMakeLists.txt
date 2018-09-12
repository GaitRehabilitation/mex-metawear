SET(SOURCE
        src/MetawearWrapperBase.cpp
        src/MetawearMexFunction.cpp
        src/MexUtility.cpp
        src/ConnectionHandler.cpp
        src/FunctionWrapper.cpp
        src/ConfigurationHandler.cpp
        src/QueryHandler.cpp
        src/CaptureHandler.cpp
        src/StreamHandler.cpp)
SET(INCLUDE
        include/FunctionWrapper.h
        include/MetawearConfig.h
        include/MetawearWrapperBase.h
        include/MetawearMexFunction.h
        include/MexUtility.h
        include/StreamTypes.h
        include/handlers/ConfigurationHandler.h
        include/handlers/CaptureHandler.h
        include/handlers/QueryHandler.h
        include/handlers/ConnectionHandler.h
        include/StreamHandler.h)




if (UNIX)
    include_directories(platform/linux/include)
    set (PLATFORM_SOURCE
            platform/linux/src/MetawerWrapper.cpp
            )

    set(PLATFORM_HEADERS
            platform/linux/include/MetawearWrapper.h
            )
endif (UNIX)


if (MSVC)
    include_directories(platform/windows/include)
    set (PLATFORM_SOURCE
            platform/windows/src/MetawearWrapper.cpp
            )

    set(PLATFORM_HEADERS
            platform/windows/include/MetawearWrapper.h include/handlers/CaptureHandler.h)
    add_compile_options("/std:c++14")
     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /ZW")
endif(MSVC)

include_directories(${Matlab_INCLUDE_DIRS})
include_directories(include)

add_definitions(-DMATLAB_MEX_FILE)

source_group("Source Files" FILES ${SOURCE})
source_group("Header Files" FILES ${INCLUDE})

source_group("Platform Source Files" FILES ${PLATFORM_SOURCE})
source_group("Platform Header Files" FILES ${PLATFORM_HEADERS})


add_library(metawear SHARED ${SOURCE} ${INCLUDE} ${PLATFORM_SOURCE} ${PLATFORM_HEADERS})
target_link_libraries(metawear  ${Matlab_LIBRARIES})
target_link_libraries(metawear  extern_metawear)


# 32-bit or 64-bit mex
if(WIN32)
    if (CMAKE_CL_64)
        SET_TARGET_PROPERTIES(metawear PROPERTIES SUFFIX .mexw64)
    else(CMAKE_CL_64)
        SET_TARGET_PROPERTIES(metawear PROPERTIES SUFFIX .mexw32)
    endif(CMAKE_CL_64)
else(WIN32)
    if (CMAKE_SIZEOF_VOID_P MATCHES "8")
        SET_TARGET_PROPERTIES(metawear PROPERTIES SUFFIX .mexa64 PREFIX "")
    else(CMAKE_SIZEOF_VOID_P MATCHES "8")
        SET_TARGET_PROPERTIES(metawear PROPERTIES SUFFIX .mexglx PREFIX "")
    endif (CMAKE_SIZEOF_VOID_P MATCHES "8")
endif(WIN32)


if (MSVC)
    target_link_libraries(metawear wsock32 ws2_32)
endif(MSVC)

if(WIN32)
    if(MINGW)
        set(_matlab_lib_dir_for_search ${Matlab_EXTERN_LIBRARY_DIR}/mingw64)
    else()
        set(_matlab_lib_dir_for_search ${Matlab_EXTERN_LIBRARY_DIR}/microsoft)
    endif()
    set(_matlab_lib_prefix_for_search "lib")
else()
    set(_matlab_lib_dir_for_search ${Matlab_BINARIES_DIR})
    set(_matlab_lib_prefix_for_search "lib")
endif()


target_link_libraries(metawear ${_matlab_lib_dir_for_search}/libMatlabDataArray.lib)

