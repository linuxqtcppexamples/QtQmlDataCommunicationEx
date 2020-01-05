#include "InvokeEx.h"
#include <QDebug>
InvokeEx::InvokeEx(QObject *parent) : QObject(parent)
{

}

void InvokeEx::cppMethod(const QString &msg) {
    qDebug() << "Called the C++ method with" << msg;
}

void InvokeEx::cppSlot(QString number) {
    qDebug() << "Called the C++ slot with" << number;
}
