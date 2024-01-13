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

#--------------< Includes >--------------#

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

#--------------< Variables >--------------#

set(FLAPI_INCLUDE
    fl/forward/Account.hpp
    fl/forward/Channel.hpp
    fl/forward/Chat.hpp
    fl/forward/ForwardMessage.hpp
    fl/forward/Message.hpp
)
set(FLAPI_SOURCE
    ${FLAPI_INCLUDE}

    fl/forward/Account.cpp
    fl/forward/Channel.cpp
    fl/forward/Chat.cpp
    fl/forward/ForwardMessage.cpp
    fl/forward/Message.cpp
)

set(FLCLIENT_INCLUDE
    fl/app/Client.hpp
)
set(FLCLIENT_SOURCE
    ${FLCLIENT_INCLUDE}

    fl/app/Client.cpp
)

#--------------< Projects >--------------#

add_library(Forward.API SHARED ${FLAPI_SOURCE})

target_include_directories(Forward.API
PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(Forward.API
PUBLIC
    Forward::Core
    Forward::Net
    Forward::Web
    Forward::Database
)

add_library(Forward.Client SHARED ${FLCLIENT_SOURCE})

target_include_directories(Forward.Client
PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(Forward.Client PUBLIC Forward::API)

#--------------< API versions >--------------#

set_target_properties(Forward.API 
PROPERTIES
    VERSION ${ForwardLib_VERSION} 
    SOVERSION ${ForwardLib_VERSION}
)
set_target_properties(Forward.Client 
PROPERTIES 
    VERSION ${ForwardLib_VERSION}
    SOVERSION ${ForwardLib_VERSION}
)

#--------------< Exporting targets >--------------#

install(FILES ${FLAPI_INCLUDE} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/fl/forward)
install(FILES ${FLCLIENT_INCLUDE} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/fl/app)
install(
    TARGETS  Forward.API Forward.Client
    EXPORT   fl-targets
    LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
