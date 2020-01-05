#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlComponent>
#include <QMetaObject>
#include <QDebug>

#include "datasharebetweenqtqml.h"
#include "qmlregistryex.h"
#include "InvokeEx.h"
#include "revisionex.h"
int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    //IUserData for to import in qml
    //UserData is compoent to use qml
    //Syntax: qmlRegisterType(const QUrl &url, const char *uri, int versionMajor, int versionMinor, const char *qmlName)
    qmlRegisterType<qmlregistryex>("IUserData",1,0,"CUserData");
//    qmlRegisterType<InvokeEx>("IInvokeEx",1,0,"CInvokeEx");


    qmlRegisterType<RevisionEx, 1>("IRevisionEx", 1, 1, "CRevisionEx");
    qmlRegisterType<RevisionEx, 2>("IRevisionEx", 1, 2, "CRevisionEx");

    QQmlApplicationEngine engine;
    DataShareBetweenQtQML dataShare;
    InvokeEx idInvokeEx;

    QQmlContext* qmlContext = engine.rootContext();
    qmlContext->setContextProperty("UserData",&dataShare);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    QObject *invokeExObj = engine.rootObjects().first();

    QVariant returnedValue;
    QVariant msg = "Hello from C++";

    //myQmlFunction in qml
    QMetaObject::invokeMethod(invokeExObj, "myQmlFunction",Qt::DirectConnection,
            Q_RETURN_ARG(QVariant, returnedValue),
            Q_ARG(QVariant, msg));

    QObject::connect(invokeExObj, SIGNAL(qmlSignal(QString)),
                      &idInvokeEx, SLOT(cppSlot(QString)));

    qDebug() << "QML function returned:" << returnedValue.toString();

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
