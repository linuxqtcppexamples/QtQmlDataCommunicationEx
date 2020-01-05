#ifndef QMLREGISTRYEX_H
#define QMLREGISTRYEX_H

#include <QObject>

class qmlregistryex : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString strUserName READ strUserName WRITE setStrUserName NOTIFY strUserNameChanged)
public:
    explicit qmlregistryex(QObject *parent = nullptr);
    QString strUserName();
    void setStrUserName(QString i_strUsername);
signals:
    void strUserNameChanged();
public slots:
private:
    QString m_strUserName;
};

#endif // QMLREGISTRYEX_H
