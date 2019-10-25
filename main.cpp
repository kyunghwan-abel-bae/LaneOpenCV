#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "adas_filter.h"

int main(int argc, char *argv[])
{
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    qmlRegisterType<ADASFilter>("opencv.filter.adas", 1, 0, "ADASFilter");

    QQmlApplicationEngine engine;
    //engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    engine.load(QUrl(QString("qrc:/%2").arg(MAIN_QML)));

    return app.exec();
}
