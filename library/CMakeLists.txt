SET(SOURCE
        src/MetaMotion.cpp
        src/MexUtility.cpp
        src/MetawearWrapperBase.cpp)
SET(INCLUDE
        include/MetaMotion.h
        include/MetamotionWrapper.h
        include/MetawearDatastream.h
        include/MetawearWrapperBase.h
        include/MexUtility.h
        )


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
            platform/windows/include/MetawearWrapper.h
            )

endif(MSVC)

include_directories(${Matlab_INCLUDE_DIRS})
include_directories(include)

add_definitions(-DMATLAB_MEX_FILE)

source_group("Source Files" FILES ${SOURCE})
source_group("Header Files" FILES ${INCLUDE})

source_group("Platform Source Files" FILES ${PLATFORM_SOURCE})
source_group("Platform Header Files" FILES ${PLATFORM_HEADERS})


add_library(mex_metawear SHARED ${SOURCE} ${INCLUDE} ${PLATFORM_SOURCE} ${PLATFORM_HEADERS})
target_link_libraries(mex_metawear ${Matlab_LIBRARIES})
target_link_libraries(mex_metawear extern_metawear)


if(WIN32)
    # socket utilites for bitwise conversion of bluetooth address
    target_link_libraries(mex_metawear wsock32 ws2_32)

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

target_link_libraries(mex_metawear ${_matlab_lib_dir_for_search}/libMatlabDataArray.lib)

