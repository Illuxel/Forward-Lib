win32:CONFIG(release, release | debug) {
    LIBS += -L$$OUT_PWD/../Compiles/Msg-API/qmake/win/release -laloAPI
}
else:win32:CONFIG(debug, release | debug) {
    LIBS += -L$$OUT_PWD/../Compiles/Msg-API/qmake/win/debug -laloAPI
}
else:unix:CONFIG(release, release | debug) {
    LIBS += -L$$OUT_PWD/../Compiles/Msg-API/qmake/unix/release -laloAPI
}
else:unix:CONFIG(debug, release | debug) {
    LIBS += -L$$OUT_PWD/../Compiles/Msg-API/qmake/unix/debug -laloAPI
}

DEFINES = ALO_EXPORT

INCLUDEPATH +=  $$PWD/../Msg-API/include
DEPENDPATH +=   $$PWD/../Msg-API/include

win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $OUT_PWD/../Compiles/Msg-API/qmake/win/release/aloAPI.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Compiles/Msg-API/qmake/win/debug/aloAPI.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../Compiles/Msg-API/qmake/unix/libaloAPI.a
