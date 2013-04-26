IMPORT_VERSION = 0.1

QT = qml fluentd
LIBS += -L$$QT.fluentd.libs

SOURCES += main.cpp

load(qml_plugin)

OTHER_FILES = plugins.qmltypes qmldir
