cmake_minimum_required(VERSION 3.24)

if (NOT DEFINED CMAKE_INSTALL_LIBDIR)
  set(CMAKE_INSTALL_LIBDIR "lib")
endif()

# Networking

set(FLNET_INCLUDE
    fl/net/Base.hpp
    fl/net/AsyncListener.hpp
    fl/net/Endpoint.hpp
    fl/net/Socket.hpp
)
set(FLNET_SOURCE
    ${FLNET_INCLUDE}
    fl/net/AsyncListener.cpp
    fl/net/Endpoint.cpp
    fl/net/Socket.cpp
)

# HTTP

set(FLHTTP_INCLUDE
    fl/net/http/HttpRequest.hpp
    fl/net/http/HttpResponse.hpp
)
set(FLHTTP_SOURCE
    ${FLHTTP_SOURCE}
    fl/net/http/HttpRequest.cpp
    fl/net/http/HttpResponse.cpp
)

# Auth

set(FLAUTH_INCLUDE
    fl/net/auth/JsonWebToken.hpp
)
set(FLAUTH_SOURCE
    ${FLAUTH_INCLUDE}
    fl/net/auth/JsonWebToken.cpp
)

if(NOT TARGET jwt-cpp)
    find_package(jwt-cpp REQUIRED)
endif()

if(NOT TARGET Boost)
    find_package(Boost 1.81 REQUIRED system COMPONENTS)
endif()

add_library(net STATIC 
    ${FLNET_SOURCE} 
    ${FLHTTP_SOURCE} 
    ${FLAUTH_SOURCE}
)

target_include_directories(net 
PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(net
PUBLIC    
    Boost::system
    Fl::utils
    jwt-cpp::jwt-cpp
)

install(FILES ${FLNET_INCLUDE}  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/fl/net)
install(FILES ${FLAUTH_INCLUDE} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/fl/net/auth)
install(FILES ${FLAUTH_INCLUDE} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/fl/net/http)

install(TARGETS net
    EXPORT fl-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)