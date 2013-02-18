TEMPLATE = subdirs
SUBDIRS = fluentd

!isEmpty(QT.qml.name) {
    src_imports.subdir = imports
    src_imports.depends = fluentd
    SUBDIRS += src_imports
}
