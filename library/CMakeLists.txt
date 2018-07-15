SET(SOURCE
        src/MetaMotionLinuxWrapper.cpp
        src/MetamotionWrapper.cpp
        src/MetaMotion.cpp
        src/MexUtility.cpp
        )
SET(INCLUDE
        include/MetaMotionLinuxWrapper.h
        include/MetamotionWrapper.h
        include/MexUtility.h
        include/MetaMotion.h
        )

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/3rdparty/libblepp)
INCLUDE_DIRECTORIES(${Matlab_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} )

add_library(mex_metawear SHARED ${SOURCE} ${INCLUDE})
target_link_libraries(mex_metawear ${Matlab_LIBRARIES})
target_link_libraries(mex_metawear ble++)


