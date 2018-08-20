
if (UNIX)
    include_directories(linux/include)
    set (PLATFORM_SOURCE
            linux/src/MetawerWrapper.cpp
            )

    set(PLATFORM_HEADERS
            linux/include/MetawearWrapper.h
            )
endif (UNIX)


if (MSVC)
    include_directories(windows/include)
    set (PLATFORM_SOURCE
            windows/src/MetawerWrapper.cpp
            )

    set(PLATFORM_HEADERS
            windows/include/MetawearWrapper.h
            )

endif(MSVC)

