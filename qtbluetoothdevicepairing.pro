macos:CONFIG -= app_bundle

QT += core quick bluetooth

CONFIG += c++11
DEFINES += QT_DEPRECATED_WARNINGS

HEADERS += \
        device.h \
        bluetooth.h \

SOURCES += \
        device.cpp \
        bluetooth.cpp \
        main.cpp

RESOURCES += qml.qrc
