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

if(NOT TARGET OPENSSL)
    find_package(OpenSSL 1.1.1 REQUIRED)
endif()

if(NOT TARGET Boost)

    if (NOT BUILD_SHARED_LIBS)
        set(Boost_USE_STATIC_RUNTIME ON)
    endif()

    list(APPEND CMAKE_PREFIX_PATH $ENV{BOOST})
    find_package(Boost REQUIRED system COMPONENTS)

endif()

if (NOT TARGET jwt-cpp)
    find_package(jwt-cpp REQUIRED)
endif()

# Networking

set(NET_INTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR}/fl/net)
set(NET_INCLUDE
    fl/net/Core.hpp

    fl/net/Endpoint.hpp
    fl/net/AsyncListener.hpp
)
set(NET_SOURCE
    ${NET_INCLUDE}

    fl/net/Endpoint.cpp
    fl/net/AsyncListener.cpp
)

# HTTP
set(HTTP_INTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR}/fl/net/http)
set(HTTP_INCLUDE
    fl/net/http/HttpQuery.hpp
    fl/net/http/HttpUrl.hpp

    fl/net/http/HttpRequest.hpp
    fl/net/http/HttpResponse.hpp

    fl/net/http/WebFilesSystem.hpp
    fl/net/http/HttpRouter.hpp
)
set(HTTP_SOURCE
    ${HTTP_INCLUDE}   

    fl/net/http/HttpQuery.cpp
    fl/net/http/HttpUrl.cpp

    fl/net/http/HttpRequest.cpp
    fl/net/http/HttpResponse.cpp

    fl/net/http/WebFilesSystem.cpp
    fl/net/http/HttpRouter.cpp
)

# Auth

set(AUTH_INTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR}/fl/net/auth)

set(AUTH_INCLUDE
    fl/net/auth/JsonWebToken.hpp
)
set(AUTH_SOURCE
    ${AUTH_INCLUDE}
    fl/net/auth/JsonWebToken.cpp
)

add_library(net STATIC ${AUTH_SOURCE} ${HTTP_SOURCE} ${NET_SOURCE})

target_include_directories(net 
PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(net
INTERFACE
    OpenSSL::SSL    
PUBLIC    
    Boost::system 
    jwt-cpp::jwt-cpp
    Fl::utils
)

install(FILES ${NET_INCLUDE}  DESTINATION ${NET_INTALL_DIR})
install(FILES ${HTTP_INCLUDE} DESTINATION ${HTTP_INTALL_DIR})
install(FILES ${AUTH_INCLUDE} DESTINATION ${AUTH_INTALL_DIR})

install(TARGETS net
    EXPORT fl-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)