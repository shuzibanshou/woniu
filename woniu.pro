QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    common.cpp \
    main.cpp \
    progress.cpp \
    receivefile.cpp \
    receivemsg.cpp \
    remoteitem.cpp \
    sendmsg.cpp \
    toast.cpp \
    woniu.cpp

HEADERS += \
    common.h \
    progress.h \
    receivefile.h \
    receivemsg.h \
    remoteitem.h \
    sendmsg.h \
    toast.h \
    woniu.h

FORMS += \
    progress.ui \
    receivefile.ui \
    receivemsg.ui \
    remoteitem.ui \
    sendmsg.ui \
    toast.ui \
    toast.ui \
    woniu.ui

TRANSLATIONS += \
    woniu_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    sysIcon.qrc \
    toast.qrc
RC_ICONS = icons/logo.ico
