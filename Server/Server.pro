QT += sql

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        book.cpp \
        controller.cpp \
        main.cpp \
        sqldatabase.cpp \
        user.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    book.h \
    controller.h \
    sqldatabase.h \
    tcpsocket.h \
    user.h

win32: LIBS += -L$$PWD/../Dependencies/TCPSocket/lib/ -lTCPSocket

INCLUDEPATH += $$PWD/../Dependencies/TCPSocket/include
DEPENDPATH += $$PWD/../Dependencies/TCPSocket/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../Dependencies/TCPSocket/lib/TCPSocket.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../Dependencies/TCPSocket/lib/libTCPSocket.a

win32: LIBS += -L$$PWD/../Dependencies/TCPSocket/lib/ -lTCPSocketRelease

INCLUDEPATH += $$PWD/../Dependencies/TCPSocket/include
DEPENDPATH += $$PWD/../Dependencies/TCPSocket/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../Dependencies/TCPSocket/lib/TCPSocketRelease.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../Dependencies/TCPSocket/lib/libTCPSocketRelease.a

win32: LIBS += -L$$PWD/../Dependencies/OpenSSL/lib/ -llibcrypto

INCLUDEPATH += $$PWD/../Dependencies/OpenSSL/include
DEPENDPATH += $$PWD/../Dependencies/OpenSSL/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../Dependencies/OpenSSL/lib/libcrypto.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../Dependencies/OpenSSL/lib/liblibcrypto.a

win32: LIBS += -L$$PWD/../Dependencies/OpenSSL/lib/ -llibssl

INCLUDEPATH += $$PWD/../Dependencies/OpenSSL/include
DEPENDPATH += $$PWD/../Dependencies/OpenSSL/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../Dependencies/OpenSSL/lib/libssl.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../Dependencies/OpenSSL/lib/liblibssl.a
