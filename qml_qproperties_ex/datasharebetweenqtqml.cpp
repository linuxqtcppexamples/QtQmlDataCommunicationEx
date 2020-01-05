#include "datasharebetweenqtqml.h"
#include <QDebug>

DataShareBetweenQtQML::DataShareBetweenQtQML(QObject *parent) : QObject(parent)
{
  m_strUserName= "User";
}

QString DataShareBetweenQtQML::strUserName()
{
    return m_strUserName;
}

void DataShareBetweenQtQML::setStrUserName(QString i_strUsername)
{
   m_strUserName = i_strUsername;
   qDebug() << m_strUserName;
}
