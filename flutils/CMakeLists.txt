cmake_minimum_required(VERSION 3.24)

if (NOT DEFINED CMAKE_INSTALL_LIBDIR)
  set(CMAKE_INSTALL_LIBDIR "lib")
endif()

set(FLUTILS_DIR ${CMAKE_INSTALL_INCLUDEDIR}/fl/utils)

set(FLUTILS_INCLUDE 
    fl/utils/converters.hpp
    fl/utils/TsQueue.hpp
    fl/utils/StringBuilder.hpp
)
set(FLUTILS_SOURCE
    ${FLUTILS_INCLUDE}
    fl/utils/converters.cpp
    fl/utils/TsQueue.cpp
    fl/utils/StringBuilder.cpp
)

set(FLLOG_INCLUDE
    fl/utils/debug/logger.hpp
    fl/utils/debug/log.hpp
)
set(FLLOG_SOURCE
    ${FLLOG_INCLUDE}
    fl/utils/debug/logger.cpp
    fl/utils/debug/log.cpp
)

if (FL_INCLUDE_BENCHMARK_COMPILE)
    if (FL_ENABLE_BENCHMARK)
        add_subdirectory(bench)
    endif()

    if (FL_ENABLE_TESING)
        add_subdirectory(test)
    endif()
endif()

add_library(utils STATIC ${FLUTILS_SOURCE} ${FLLOG_SOURCE})
target_include_directories(utils 
PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(utils
PUBLIC
    Qt6::Core
)

install(FILES ${FLUTILS_INCLUDE}  DESTINATION ${FLUTILS_DIR})
install(FILES ${FLLOG_INCLUDE}    DESTINATION ${FLUTILS_DIR}/debug)

install(TARGETS utils EXPORT fl-targets
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
)