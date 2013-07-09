TEMPLATE = subdirs
!isEmpty(QT.fluentd.name) {
	!isEmpty(QT.widgets.name): SUBDIRS += cpp log
	!isEmpty(QT.quick.name): SUBDIRS += qml
}
