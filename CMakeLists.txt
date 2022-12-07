cmake_minimum_required(VERSION 3.24)

project(aloAPI 
    VERSION 0.0.1 
    LANGUAGES CXX C
    DESCRIPTION "Alo API built for messaging" 
)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_PREFIX_PATH $ENV{QT})
set(CMAKE_AUTOMOC ON)

#--------------C++ settings--------------#

include(cxx-common)

#--------------Platform--------------#

if (WIN32) 
    
elseif(APPLE)
elseif(UNIX)
endif()

#--------------Project sources and dirs--------------#

set(namespace alo)

set(ALOLIB_INCLUDE

    include/aloAPI/aloAPI.hpp
    include/aloAPI/alo-common.hpp
    include/aloAPI/aloapi

    # include/aloAPI/net/
    # include/aloAPI/net/auth/
    # include/aloAPI/net/crypto/

    include/aloAPI/db/Database.hpp

    include/aloAPI/parsers/MessageParser.hpp

    include/aloAPI/datatypes/Account.hpp
    include/aloAPI/datatypes/Channel.hpp
    include/aloAPI/datatypes/Chat.hpp
    include/aloAPI/datatypes/Message.hpp
    include/aloAPI/datatypes/MessageForwarded.hpp

    include/aloAPI/utils/converters.hpp
    include/aloAPI/utils/thpool.hpp
    include/aloAPI/utils/debug/log.hpp
    include/aloAPI/utils/debug/logger.hpp
)
set(ALOLIB_SOURCES
    # src/net/
    # src/net/auth/
    # src/net/crypto/

    src/db/Database.cpp

    src/datatypes/Account.cpp
    src/datatypes/Channel.cpp
    src/datatypes/Chat.cpp
    src/datatypes/Message.cpp
    src/datatypes/MessageForwarded.cpp

    src/parsers/MessageParser.cpp

    src/utils/converters.cpp
    src/utils/thpool.cpp
    src/utils/debug/log.cpp
    src/utils/debug/logger.cpp
)

set(ALOLIB_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(ALOLIB_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)

#

include(project-target)

target_compile_definitions(${PROJECT_NAME}
PRIVATE
    ALO_EXPORT
)
target_sources(${PROJECT_NAME}
PRIVATE
    ${ALOLIB_SOURCES}
)
target_include_directories(${PROJECT_NAME}
PRIVATE
    ${ALOLIB_SOURCE_DIR}
PUBLIC
    # where top-level project will look for the library's public headers
    $<BUILD_INTERFACE:${ALOLIB_INCLUDE_DIR}>
    # where external projects will look for the library's public headers
    $<INSTALL_INTERFACE:${ALOLIB_INCLUDE_DIR}>
)

#--------------Qt Settings--------------#

include(qt-common)

find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Network)
find_package(Qt6 REQUIRED COMPONENTS Sql)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Network)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Sql)
