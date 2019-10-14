#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "adas_filter.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    qmlRegisterType<ADASFilter>("opencv.filter.adas", 1, 0, "ADASFilter");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    return app.exec();
}
