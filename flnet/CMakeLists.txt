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

set(FLNET_INCLUDE
    fl/net/Core.hpp

    fl/net/Endpoint.hpp

    fl/net/TcpServer.hpp
    fl/net/SslServer.hpp

    fl/net/TcpSocket.hpp
    fl/net/SslSocket.hpp
)
set(FLNET_SOURCE
    ${FLNET_INCLUDE}

    fl/net/Endpoint.cpp

    fl/net/TcpServer.cpp
    fl/net/SslServer.cpp

    fl/net/TcpSocket.cpp
    fl/net/SslSocket.cpp
)

add_library(Forward.Net STATIC ${FLNET_SOURCE})

target_include_directories(Forward.Net 
PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(Forward.Net
PUBLIC    
    Forward::Core
    OpenSSL::Crypto
    OpenSSL::SSL
    Boost::system 
    Boost::json
)

install(FILES ${FLNET_INCLUDE} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/fl/net)
install(TARGETS Forward.Net
    EXPORT fl-targets
    LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
