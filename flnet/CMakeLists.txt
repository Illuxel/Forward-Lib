cmake_minimum_required(VERSION 3.24)

if (NOT DEFINED CMAKE_INSTALL_LIBDIR)
  set(CMAKE_INSTALL_LIBDIR "lib")
endif()

# Networking

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

set(HTTP_INCLUDE
    fl/net/http/HttpUrl.hpp

    fl/net/http/HttpRequest.hpp
    fl/net/http/HttpResponse.hpp

    fl/net/http/WebFilesSystem.hpp
    fl/net/http/HttpRouter.hpp
)
set(HTTP_SOURCE
    ${HTTP_INCLUDE}   

    fl/net/http/HttpUrl.cpp

    fl/net/http/HttpRequest.cpp
    fl/net/http/HttpResponse.cpp

    fl/net/http/WebFilesSystem.cpp
    fl/net/http/HttpRouter.cpp
)

# Auth

set(AUTH_INCLUDE
    fl/net/auth/JsonWebToken.hpp
)
set(AUTH_SOURCE
    ${AUTH_INCLUDE}
    fl/net/auth/JsonWebToken.cpp
)

if(NOT TARGET Benchmark)
    find_package(Benchmark CONFIG REQUIRED)
endif()

if (FL_INCLUDE_BENCHMARK_COMPILE)
    if (FL_ENABLE_BENCHMARK)
        # add_subdirectory(bench)
    endif()
    
    if (FL_ENABLE_TESING)
        add_subdirectory(test)
    endif()
endif()

if(NOT TARGET jwt-cpp)
    find_package(jwt-cpp REQUIRED)
endif()

if(NOT TARGET Boost)
    find_package(Boost 1.81 REQUIRED system COMPONENTS)
endif()

add_library(net STATIC ${AUTH_SOURCE} ${HTTP_SOURCE} ${NET_SOURCE})

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

install(FILES ${NET_INCLUDE}  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/fl/net)
install(FILES ${AUTH_INCLUDE} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/fl/net/auth)
install(FILES ${HTTP_INCLUDE} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/fl/net/http)

install(TARGETS net
    EXPORT fl-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)