#---------------------------------------------------#
#                                                   #
#                                                   #
#                                                   #
#---------------------------------------------------#

TARGET = AloAPI
TEMPLATE = lib
VERSION = 0.0.1

# component includes

QT -= gui
QT += core network sql

# project type

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
#win32-g++:
    #error("Could not compile this lib with g++ compiler. Consider using mcvs instead.")

# DIR VARS

MAIN =      $$PWD/aloAPI

UTILS =     $$PWD/aloAPI/utils

DB =        $$PWD/aloAPI/db
DTYPES =    $$PWD/aloAPI/datatypes
PARSER =    $$PWD/aloAPI/parsers

NET    =    $$PWD/aloAPI/net
CRYPTO =    $$PWD/aloAPI/crypto

# FILE INCLUDES

HEADERS += \
    $$MAIN/AloAPI.h \
    $$MAIN/AloAPI_global.h \
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

# OTHER

DEFINES += \
    ALO_API_LIB \

DISTFILES += \
    $$PWD/AloAPI.pro \
    $$PWD/AloAPI.pri \
