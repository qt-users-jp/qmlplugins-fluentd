TARGET = QtFluentd
MODULE = fluentd
QT = core network qml

load(qt_module)

PUBLIC_HEADERS = qfluentd.h
HEADERS = $$PUBLIC_HEADERS
SOURCES = qfluentd.cpp

DEFINES += FLUENTD_LIBRARY
