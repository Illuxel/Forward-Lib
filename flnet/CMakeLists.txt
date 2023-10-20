if (NOT DEFINED CMAKE_INSTALL_LIBDIR)
  set(CMAKE_INSTALL_LIBDIR "lib")
endif()

if (FL_ENABLE_BENCHMARK)
    add_subdirectory(bench)
endif()

if (FL_ENABLE_TESTING)
    add_subdirectory(test)
endif()

set(NET_INCLUDE
    fl/net/Core.hpp

    fl/net/Endpoint.hpp

    fl/net/TcpServer.hpp
    fl/net/SslServer.hpp

    fl/net/TcpSocket.hpp
    fl/net/SslSocket.hpp
)
set(NET_SOURCE
    ${NET_INCLUDE}

    fl/net/Endpoint.cpp

    fl/net/TcpServer.cpp
    fl/net/SslServer.cpp

    fl/net/TcpSocket.cpp
    fl/net/SslSocket.cpp
)

add_library(Forward.Net STATIC ${NET_SOURCE})

target_include_directories(Forward.Net 
PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(Forward.Net
PUBLIC    
    OpenSSL::Crypto
    OpenSSL::SSL
    Boost::system 
    Boost::json
    Forward::Utils
)

install(FILES ${NET_INCLUDE} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/fl/net)

install(TARGETS Forward.Net
    EXPORT fl-targets
    LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
