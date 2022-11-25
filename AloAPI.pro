#---------------------------------------------------#
#                                                   #
#                                                   #
#                                                   #
#---------------------------------------------------#

TARGET = aloAPI
TEMPLATE = lib
VERSION = 0.0.1

# components to include

QT -= gui
QT += core network sql

# project type

DEFINES += ALO_EXPORT
CONFIG += staticlib

# COMPILE LOG OUTPUT

CONFIG(release, release | debug) {
    OBJECTS_DIR =   ../Compiles/Msg-API/Build/release/obj
    MOC_DIR =       ../Compiles/Msg-API/Build/release/moc
}
else:CONFIG(debug, release | debug) {
    OBJECTS_DIR =   ../Compiles/Msg-API/Build/debug/obj
    MOC_DIR =       ../Compiles/Msg-API/Build/debug/moc
}

# PLATFORM SPECIFIC

win32:CONFIG(release, release | debug) {
    DESTDIR =   ../Compiles/Msg-API/win/release
}
else:win32:CONFIG(debug, release | debug) {
    DESTDIR =   ../Compiles/Msg-API/win/debug
}
else:unix:CONFIG(release, release | debug) {
    DESTDIR =   ../Compiles/Msg-API/unix/release
}
else:unix:CONFIG(debug, release | debug) {
    DESTDIR =   ../Compiles/Msg-API/unix/debug
}

# PROJECT CHECKERS

!isEmpty(target.path):
    INSTALLS += target
    
# DIR VARS

MAIN =      $$PWD/src/

UTILS =     $$MAIN/utils
DB =        $$MAIN/db
DTYPES =    $$MAIN/datatypes
PARSER =    $$MAIN/parsers
NET    =    $$MAIN/net

# FILE INCLUDES

HEADERS += \
    $$MAIN/aloAPI.h \
    $$MAIN/aloAPI_global.h \
    \
    $$UTILS/log.h \
    $$UTILS/converters.h \
    $$UTILS/thpool.h \
    \
    $$DTYPES/Account.h \
    $$DTYPES/Channel.h \
    $$DTYPES/Chat.h \
    $$DTYPES/Message.h \
    $$DTYPES/MessageForward.h \

SOURCES += \
    $$UTILS/log.cpp \
    $$UTILS/converters.cpp \
    $$UTILS/thpool.cpp \
    \
    $$DTYPES/Account.cpp \
    $$DTYPES/Channel.cpp \
    $$DTYPES/Chat.cpp \
    $$DTYPES/Message.cpp \
    $$DTYPES/MessageForward.cpp \

INCLUDEPATH += \
    $$MAIN
DEPENDPATH += \
    $$MAIN

# OTHER

DISTFILES += \
    $$PWD/aloAPI.pro \
    $$PWD/aloAPI.pri \
