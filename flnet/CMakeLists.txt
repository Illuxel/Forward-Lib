cmake_minimum_required(VERSION 3.24)

if (NOT DEFINED CMAKE_INSTALL_LIBDIR)
  set(CMAKE_INSTALL_LIBDIR "lib")
endif()

if (FL_ENABLE_BENCHMARK)
    add_subdirectory(bench)
endif()

if (FL_ENABLE_TESING)
    add_subdirectory(test)
endif()

# Networking

set(NET_INTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR}/fl/net)
set(NET_INCLUDE
    fl/net/Core.hpp

    fl/net/Endpoint.hpp
    fl/net/AsyncListener.hpp
    fl/net/SslServer.hpp
)
set(NET_SOURCE
    ${NET_INCLUDE}

    fl/net/Endpoint.cpp
    fl/net/AsyncListener.cpp
    fl/net/SslServer.cpp
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
)
set(HTTP_SOURCE
    ${HTTP_INCLUDE}   

    fl/net/http/HttpQuery.cpp
    fl/net/http/HttpUrl.cpp

    fl/net/http/WebFileSystem.cpp
    fl/net/http/HttpRouter.cpp
    fl/net/http/HttpResponder.cpp
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

install(TARGETS net
    EXPORT fl-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)