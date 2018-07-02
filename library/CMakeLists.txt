SET(SOURCE ./src/library.cpp)

SET(INCLUDE ./include/library.h)




add_library(mex_metawear SHARED ${SOURCE} ${INCLUDE})