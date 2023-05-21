cmake_minimum_required(VERSION 3.24)

if (NOT DEFINED CMAKE_INSTALL_LIBDIR)
  set(CMAKE_INSTALL_LIBDIR "lib")
endif()

set(FLUTILS_DIR ${CMAKE_INSTALL_INCLUDEDIR}/fl/utils)

set(FLUTILS_INCLUDE
    fl/utils/Memory.hpp
    fl/utils/DateTime.hpp
    fl/utils/Enum.hpp
    fl/utils/Flags.hpp
    fl/utils/MimeType.hpp
    fl/utils/StringBuilder.hpp
    fl/utils/Logger.hpp
    fl/utils/Log.hpp
)
set(FLUTILS_SOURCE
    ${FLUTILS_INCLUDE}
    fl/utils/DateTime.cpp
    fl/utils/Enum.cpp
    fl/utils/Flags.cpp
    fl/utils/MimeType.cpp
    fl/utils/StringBuilder.cpp
    fl/utils/Logger.cpp
    fl/utils/Log.cpp
)

if(NOT TARGET Benchmark)
    find_package(Benchmark CONFIG REQUIRED)
endif()

if (FL_INCLUDE_BENCHMARK_COMPILE)
    if (FL_ENABLE_BENCHMARK)
        add_subdirectory(bench)
    endif()

    if (FL_ENABLE_TESING)
        add_subdirectory(test)
    endif()
endif()

add_library(utils 
STATIC 
    ${FLUTILS_SOURCE} 
    ${FLLOG_SOURCE}
)

target_include_directories(utils 
PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
# target_link_libraries(utils
# PUBLIC
#     # Qt6::Core
# PRIVATE
#     # Boost::system 
#     # Boost::thread
# )

install(FILES ${FLUTILS_INCLUDE}  DESTINATION ${FLUTILS_DIR})
install(FILES ${FLLOG_INCLUDE}    DESTINATION ${FLUTILS_DIR}/debug)

install(TARGETS utils 
    EXPORT fl-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)