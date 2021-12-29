QT += core gui sql

CONFIG += c++11 console
CONFIG -= app_bundle
LIBS += -lws2_32

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        book.cpp \
        main.cpp \
        sqldatabase.cpp \
        tcpsocket.cpp \
        user.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    book.h \
    sqldatabase.h \
    tcpsocket.h \
    user.h
