TEMPLATE = app
TARGET = fluentd
QT += widgets fluentd
CONFIG += c++11
SOURCES = main.cpp

HEADERS += \
    widget.h

SOURCES += \
    widget.cpp

FORMS += \
    widget.ui
