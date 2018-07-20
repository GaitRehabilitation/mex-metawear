SET(SOURCE
        src/MetaMotionBluetoothLinux.cpp
        src/MetamotionWrapper.cpp
        src/MetaMotion.cpp
        src/MexUtility.cpp
        )
SET(INCLUDE
        include/MetaMotionBluetoothLinux.h
        include/MetamotionWrapper.h
        include/MexUtility.h
        include/MetaMotion.h
        )

INCLUDE_DIRECTORIES(${Matlab_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} )
INCLUDE_DIRECTORIES(${Metawear_INCLUDE_DIRS} )
INCLUDE_DIRECTORIES(${Libblepp_INCLUDE_DIRS} )

add_library(mex_metawear SHARED ${SOURCE} ${INCLUDE})
target_link_libraries(mex_metawear ${Matlab_LIBRARIES})
target_link_libraries(mex_metawear ble++ )
target_link_libraries(mex_metawear MetaWear-SDK)
