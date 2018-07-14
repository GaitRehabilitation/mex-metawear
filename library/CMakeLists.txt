SET(SOURCE
        src/MetamotionLinuxWrapper.cpp
        src/MetamotionWrapper.cpp
        src/MetaMotion.cpp
        )
SET(INCLUDE
        include/MetamotionLinuxWrapper.h
        include/MetamotionWrapper.h
        include/MetaMotion.h
        )

INCLUDE_DIRECTORIES(${Matlab_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} )

add_library(mex_metawear SHARED ${SOURCE} ${INCLUDE})
target_link_libraries(mex_metawear ${Matlab_LIBRARIES})


