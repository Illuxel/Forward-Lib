win32:CONFIG(release, release | debug) {
    LIBS += -L$$OUT_PWD/../Compiles/Msg-API/win/release -lAloAPI
}
else:win32:CONFIG(debug, release | debug) {
    LIBS += -L$$OUT_PWD/../Compiles/Msg-API/win/debug -lAloAPI
}
else:unix:CONFIG(release, release | debug) {
    LIBS += -L$$OUT_PWD/../Compiles/Msg-API/unix/release -lAloAPI
}
else:unix:CONFIG(debug, release | debug) {
    LIBS += -L$$OUT_PWD/../Compiles/Msg-API/unix/debug -lAloAPI
}

INCLUDEPATH += \
    $$PWD/../Msg-API/aloApi
DEPENDPATH += \
    $$PWD/../Msg-API/aloApi

win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Compiles/Msg-API/win/release/AloAPI.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Compiles/Msg-API/win/debug/AloAPI.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../Compiles/Msg-API/unix/libAloAPI.a
