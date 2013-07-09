#include "widget.h"
#include <QtWidgets/QApplication>
#include <QtCore/QJsonObject>
#include <QtFluentd>

QFluentd fluentd;

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &message)
{
    QJsonObject object;
    switch (type) {
    case QtDebugMsg:
    case QtWarningMsg:
    case QtCriticalMsg:
        object.insert(QStringLiteral("file"), QString::fromUtf8(context.file));
        object.insert(QStringLiteral("line"), context.line);
        object.insert(QStringLiteral("function"), QString::fromUtf8(context.function));
        object.insert(QStringLiteral("message"), message);
        fluentd.send(context.category, object);
        break;
    case QtFatalMsg:
        // TODO
        abort();
    }
}


int main(int argc, char *argv[])
{
    qInstallMessageHandler(myMessageOutput);
    QApplication a(argc, argv);
    Widget w;
    w.show();
    
    return a.exec();
}
