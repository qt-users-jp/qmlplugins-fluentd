#include <QtQuick>
#include <QtFluentd>

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    QQuickView view(QUrl("qrc:/fluentd.qml"));
    QObject::connect(view.engine(), &QQmlEngine::quit, &view, &QQuickView::close);
    view.show();

    return app.exec();
}

