cmake_minimum_required(VERSION 3.24)

if (NOT DEFINED CMAKE_INSTALL_LIBDIR)
  set(CMAKE_INSTALL_LIBDIR "lib")
endif()

# Auth

set(FLAUTH_INCLUDE
    fl/net/auth/JsonToken.hpp
)
set(FLAUTH_SOURCE
    ${FLAUTH_INCLUDE}
    fl/net/auth/JsonToken.cpp
)

# Networking

set(FLNET_INCLUDE
    fl/flnet.hpp
    fl/net/Socket.hpp
    fl/net/SocketThread.hpp
    fl/net/HttpRequest.hpp
    fl/net/HttpResponse.hpp
)
set(FLNET_SOURCE
    ${FLNET_INCLUDE}
    fl/net/Socket.cpp
    fl/net/SocketThread.cpp
    fl/net/HttpRequest.cpp
    fl/net/HttpResponse.cpp
)

add_library(net STATIC ${FLNET_SOURCE} ${FLAUTH_SOURCE})
target_include_directories(net 
PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(net
PUBLIC    
    Qt6::Network
    Qt6::HttpServer
    Fl::utils
)

install(FILES ${FLNET_INCLUDE}  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/fl)
install(FILES ${FLNET_INCLUDE}  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/fl/net)
install(FILES ${FLAUTH_INCLUDE} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/fl/net/auth)

install(TARGETS net
    EXPORT fl-targets
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
)