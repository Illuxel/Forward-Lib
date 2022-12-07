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
    OBJECTS_DIR =   $$PWD/../Compiles/Msg-API/qmake/Build/release/obj
    MOC_DIR =       $$PWD/../Compiles/Msg-API/qmake/Build/release/moc
}
else:CONFIG(debug, release | debug) {
    OBJECTS_DIR =   $$PWD/../Compiles/Msg-API/qmake/Build/debug/obj
    MOC_DIR =       $$PWD/../Compiles/Msg-API/qmake/Build/debug/moc
}

# PLATFORM SPECIFIC

win32:CONFIG(release, release | debug) {
    DESTDIR =   $$PWD/../Compiles/Msg-API/qmake/win/release
}
else:win32:CONFIG(debug, release | debug) {
    DESTDIR =   $$PWD/../Compiles/Msg-API/qmake/win/debug
}
else:unix:CONFIG(release, release | debug) {
    DESTDIR =   $$PWD/../Compiles/Msg-API/qmake/unix/release
}
else:unix:CONFIG(debug, release | debug) {
    DESTDIR =   $$PWD/../Compiles/Msg-API/qmake/unix/debug
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
    $$MAIN/aloAPI.h \
    $$MAIN/alo-common.h \
    \
    $$DTYPES/Account.h \
    $$DTYPES/Channel.h \
    $$DTYPES/Chat.h \
    $$DTYPES/Message.h \
    $$DTYPES/MessageForward.h \
    \
    $$UTILS/debug/log.h \
    $$UTILS/debug/logger.h \
    $$UTILS/converters.h \
    $$UTILS/thpool.h 

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
    $$PWD/aloAPI.pro \
    $$PWD/aloAPI.pri 

# methods

# defineHeaderSource(path) {
# 
# }