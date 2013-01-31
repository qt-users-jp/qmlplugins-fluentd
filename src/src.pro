TEMPLATE = lib
CONFIG += plugin

QT = core network qml

TARGET = fluentd-qml
TARGETPATH = me/qtquick/qmlplugins

HEADERS += \
    fluentd.h \
    qml-fluentd-plugin.h

SOURCES += \
    fluentd.cpp

target.path = $$[QT_INSTALL_QML]/$$TARGETPATH

qmldir.files = qmldir
qmldir.path = $$[QT_INSTALL_QML]/$$TARGETPATH

INSTALLS = target qmldir
