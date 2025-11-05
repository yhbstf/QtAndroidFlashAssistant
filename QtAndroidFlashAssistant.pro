TEMPLATE = app
CONFIG += qt warn_on release c++17
QT += widgets

DESTDIR = bin
OBJECTS_DIR = build/obj
MOC_DIR = build/moc
RCC_DIR = build/rcc
UI_DIR = build/ui

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/processrunner.cpp \
    src/adbclient.cpp \
    src/fastbootclient.cpp

HEADERS += \
    src/mainwindow.h \
    src/processrunner.h \
    src/adbclient.h \
    src/fastbootclient.h

RESOURCES += resources.qrc \
    resources.qrc

DISTFILES += \
    tools/adb.exe \
    tools/fastboot.exe



