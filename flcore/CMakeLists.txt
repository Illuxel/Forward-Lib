#--------------< Building tests and examples >--------------#

if (FL_ENABLE_EXAMPLE)
    add_subdirectory(example)
endif()

if (FL_ENABLE_BENCHMARK)
    add_subdirectory(bench)
endif()

if (FL_ENABLE_TEST)
    add_subdirectory(test)
endif()

#--------------< Variables >--------------#

set(FLCORE_INCLUDE
    fl/core/Exception.hpp
    fl/core/Memory.hpp
    fl/core/Flags.hpp

    fl/core/TypedArg.hpp
    fl/core/StringArg.hpp
    fl/core/StringArgParser.hpp
    fl/core/StringBuilder.hpp

    fl/core/DateTime.hpp
    fl/core/MimeType.hpp

    fl/core/Timer.hpp

    fl/core/LoggerStream.hpp
    fl/core/Logger.hpp
    fl/core/Log.hpp
)
set(FLCORE_SOURCE
    ${FLCORE_INCLUDE}

    fl/core/Exception.cpp
    
    fl/core/StringArgParser.cpp
    fl/core/StringBuilder.cpp

    fl/core/DateTime.cpp
    fl/core/MimeType.cpp

    fl/core/LoggerStream.cpp
    fl/core/Logger.cpp
    fl/core/Log.cpp
)

add_library(Forward.Core STATIC ${FLCORE_SOURCE})

target_include_directories(Forward.Core 
PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

install(FILES ${FLCORE_INCLUDE} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/fl/core)
install(
    TARGETS  Forward.Core 
    EXPORT   fl-targets
    LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)