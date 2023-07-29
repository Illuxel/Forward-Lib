cmake_minimum_required(VERSION 3.24)

if (NOT DEFINED CMAKE_INSTALL_LIBDIR)
  set(CMAKE_INSTALL_LIBDIR "lib")
endif()

if (FL_ENABLE_BENCHMARK)
    add_subdirectory(bench)
endif()

if (FL_ENABLE_TESTING)
    add_subdirectory(test)
endif()

set(FLUTILS_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR}/fl/utils)

set(FLUTILS_INCLUDE
    fl/utils/Exception.hpp
    fl/utils/Memory.hpp
    fl/utils/Flags.hpp

    fl/utils/TypedArg.hpp
    fl/utils/StringArg.hpp
    fl/utils/StringArgParser.hpp
    fl/utils/StringBuilder.hpp

    fl/utils/DateTime.hpp
    fl/utils/MimeType.hpp

    fl/utils/Timer.hpp
    fl/utils/Logger.hpp
    fl/utils/Log.hpp
)
set(FLUTILS_SOURCE
    ${FLUTILS_INCLUDE}

    fl/utils/Exception.cpp
    fl/utils/Flags.cpp
    

    fl/utils/StringArgParser.cpp
    fl/utils/StringBuilder.cpp

    fl/utils/DateTime.cpp
    fl/utils/MimeType.cpp

    fl/utils/Logger.cpp
    fl/utils/Log.cpp
)

add_library(utils STATIC ${FLUTILS_SOURCE})

target_include_directories(utils 
PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

install(FILES ${FLUTILS_INCLUDE}  DESTINATION ${FLUTILS_INSTALL_DIR})
install(TARGETS utils 
    EXPORT fl-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)