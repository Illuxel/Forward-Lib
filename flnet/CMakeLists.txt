<<<<<<< Updated upstream
cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

if (NOT DEFINED CMAKE_INSTALL_LIBDIR)
  set(CMAKE_INSTALL_LIBDIR "lib")
endif()

if (FL_ENABLE_BENCHMARK)
    add_subdirectory(bench)
endif()

if (FL_ENABLE_TESTING)
    add_subdirectory(test)
endif()

# Networking

set(NET_INTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR}/fl/net)
set(NET_INCLUDE
    fl/net/Core.hpp

    fl/net/Endpoint.hpp

    fl/net/TcpServer.hpp
    fl/net/SecureLayer.hpp
    fl/net/SslServer.hpp

    fl/net/TcpSocket.hpp
    fl/net/SslSocket.hpp
)
set(NET_SOURCE
    ${NET_INCLUDE}

    fl/net/Endpoint.cpp

    fl/net/TcpServer.cpp
    fl/net/SecureLayer.cpp
    fl/net/SslServer.cpp

    fl/net/TcpSocket.cpp
    fl/net/SslSocket.cpp
)

# HTTP

set(HTTP_INTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR}/fl/net/http)
set(HTTP_INCLUDE
    fl/net/http/Core.hpp

    fl/net/http/HttpQuery.hpp
    fl/net/http/HttpUrl.hpp

    fl/net/http/HttpRequest.hpp
    fl/net/http/HttpResponse.hpp

    fl/net/http/WebFileSystem.hpp
    fl/net/http/HttpRouter.hpp
    fl/net/http/HttpResponder.hpp
    fl/net/http/HttpSession.hpp
    fl/net/http/HttpServer.hpp

    fl/net/http/HttpClient.hpp
)
set(HTTP_SOURCE
    ${HTTP_INCLUDE}   

    fl/net/http/HttpQuery.cpp
    fl/net/http/HttpUrl.cpp

    fl/net/http/WebFileSystem.cpp
    fl/net/http/HttpRouter.cpp
    fl/net/http/HttpResponder.cpp
    fl/net/http/HttpSession.cpp
    fl/net/http/HttpServer.cpp

    fl/net/http/HttpClient.cpp
)

add_library(net STATIC ${HTTP_SOURCE} ${NET_SOURCE})

target_include_directories(net 
PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(net
PUBLIC    
    OpenSSL::Crypto
    OpenSSL::SSL
    Boost::system 
    Boost::json
    Fl::utils
)

install(FILES ${NET_INCLUDE}  DESTINATION ${NET_INTALL_DIR})
install(FILES ${HTTP_INCLUDE} DESTINATION ${HTTP_INTALL_DIR})

install(TARGETS net jwt-cpp
    EXPORT fl-targets
    LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
=======
cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

if (NOT DEFINED CMAKE_INSTALL_LIBDIR)
  set(CMAKE_INSTALL_LIBDIR "lib")
endif()

if (FL_ENABLE_BENCHMARK)
    add_subdirectory(bench)
endif()

if (FL_ENABLE_TESTING)
    add_subdirectory(test)
endif()

# Networking

set(NET_INTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR}/fl/net)
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

# HTTP

set(HTTP_INTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR}/fl/net/http)
set(HTTP_INCLUDE
    fl/net/http/Core.hpp

    fl/net/http/HttpQuery.hpp
    fl/net/http/HttpUrl.hpp

    fl/net/http/HttpRequest.hpp
    fl/net/http/HttpResponse.hpp

    fl/net/http/WebFileSystem.hpp
    fl/net/http/HttpRouter.hpp
    fl/net/http/HttpResponder.hpp
    fl/net/http/HttpSession.hpp
    fl/net/http/HttpServer.hpp

    fl/net/http/HttpClient.hpp
)
set(HTTP_SOURCE
    ${HTTP_INCLUDE}   

    fl/net/http/HttpQuery.cpp
    fl/net/http/HttpUrl.cpp

    fl/net/http/WebFileSystem.cpp
    fl/net/http/HttpRouter.cpp
    fl/net/http/HttpResponder.cpp
    fl/net/http/HttpSession.cpp
    fl/net/http/HttpServer.cpp

    fl/net/http/HttpClient.cpp
)

add_library(net STATIC ${HTTP_SOURCE} ${NET_SOURCE})

target_include_directories(net 
PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(net
PUBLIC    
    OpenSSL::Crypto
    OpenSSL::SSL
    Boost::system 
    Boost::json
    Fl::utils
)

install(FILES ${NET_INCLUDE}  DESTINATION ${NET_INTALL_DIR})
install(FILES ${HTTP_INCLUDE} DESTINATION ${HTTP_INTALL_DIR})

install(TARGETS net jwt-cpp
    EXPORT fl-targets
    LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
>>>>>>> Stashed changes
