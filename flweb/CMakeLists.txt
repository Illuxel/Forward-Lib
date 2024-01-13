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

set(FLWEB_INCLUDE
    fl/web/Core.hpp

    fl/web/HttpUrlParser.hpp
    fl/web/HttpQuery.hpp
    fl/web/HttpUrl.hpp

    fl/web/HttpRequest.hpp
    fl/web/HttpResponse.hpp

    fl/web/WebFileSystem.hpp
    fl/web/HttpRouter.hpp
    fl/web/HttpResponder.hpp
    fl/web/HttpSession.hpp
    fl/web/HttpServer.hpp

    fl/web/HttpClient.hpp
)
set(FLWEB_SOURCE
    ${FLWEB_INCLUDE}   

    fl/web/HttpUrlParser.cpp
    fl/web/HttpQuery.cpp
    fl/web/HttpUrl.cpp

    fl/web/WebFileSystem.cpp
    fl/web/HttpRouter.cpp
    fl/web/HttpResponder.cpp
    fl/web/HttpSession.cpp
    fl/web/HttpServer.cpp

    fl/web/HttpClient.cpp
)

#--------------< Library Forward.Web >--------------#

add_library(Forward.Web STATIC ${FLWEB_SOURCE})

target_include_directories(Forward.Web 
PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(Forward.Web
PUBLIC    
    Forward::Net
)

install(FILES ${FLWEB_INCLUDE} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/fl/web)
install(TARGETS Forward.Web
    EXPORT fl-targets
    LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
