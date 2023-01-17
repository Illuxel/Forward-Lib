#---------------------------------------------------#
#                                                   #
#                                                   #
#                                                   #
#---------------------------------------------------#

TARGET = forwardLib
TEMPLATE = lib
VERSION = 0.0.1

# components to include

QT -= gui
QT += core network sql

# project type

CONFIG += staticlib

# COMPILE LOG OUTPUT

CONFIG(release, release | debug) {
    OBJECTS_DIR =   $$PWD/build-qmake/Msg-API/qmake/Build/release/obj
    MOC_DIR =       $$PWD/build-qmake/Msg-API/qmake/Build/release/moc
}
else:CONFIG(debug, release | debug) {
    OBJECTS_DIR =   $$PWD/build-qmake/Msg-API/qmake/Build/debug/obj
    MOC_DIR =       $$PWD/build-qmake/Msg-API/qmake/Build/debug/moc
}

# PLATFORM SPECIFIC

win32:CONFIG(release, release | debug) {
    DESTDIR =   $$PWD/build-qmake/Msg-API/win/release
}
else:win32:CONFIG(debug, release | debug) {
    DESTDIR =   $$PWD/build-qmake/Msg-API/win/debug
}
else:unix:CONFIG(release, release | debug) {
    DESTDIR =   $$PWD/build-qmake/Msg-API/unix/release
}
else:unix:CONFIG(debug, release | debug) {
    DESTDIR =   $$PWD/build-qmake/Forward-API/unix/debug
}

# PROJECT CHECKERS

!isEmpty(target.path):
    INSTALLS += target
    
# DIR VARS

MAIN =      $$PWD/src

DB  =       $$MAIN/db
DTYPES =    $$MAIN/datatypes
PARSER =    $$MAIN/parsers
NET    =    $$MAIN/net

UTILS =     $$MAIN/utils

# FILE INCLUDES

HEADERS += \
    $$MAIN/aloAPI.hpp \
    $$MAIN/alo-common.hpp \
    \
    $$DTYPES/Account.hpp \
    $$DTYPES/Channel.hpp \
    $$DTYPES/Chat.hpp \
    $$DTYPES/Message.hpp \
    $$DTYPES/MessageForward.hpp \
    \
    $$UTILS/debug/log.hpp \
    $$UTILS/debug/logger.hpp \
    $$UTILS/converters.hpp \
    $$UTILS/thpool.hpp 

SOURCES += \
    $$DTYPES/Account.cpp \
    $$DTYPES/Channel.cpp \
    $$DTYPES/Chat.cpp \
    $$DTYPES/Message.cpp \
    $$DTYPES/MessageForward.cpp \
    \
    $$UTILS/debug/log.cpp \
    $$UTILS/debug/logger.cpp \
    $$UTILS/converters.cpp \
    $$UTILS/thpool.cpp 

#

INCLUDEPATH += $$MAIN
DEPENDPATH  += $$MAIN
   
# OTHER

DISTFILES += \
    $$PWD/flAPI.pro \
    $$PWD/flAPI.pri 

# methods

# defineHeaderSource(path) {
# 
# }