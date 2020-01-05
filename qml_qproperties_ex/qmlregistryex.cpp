#include "qmlregistryex.h"
#include <QDebug>
qmlregistryex::qmlregistryex(QObject *parent) : QObject(parent)
{
    m_strUserName= "Data";

}
QString qmlregistryex::strUserName()
{
    return m_strUserName;
}

void qmlregistryex::setStrUserName(QString i_strUsername)
{
   m_strUserName = i_strUsername;
   qDebug() << m_strUserName;
}
