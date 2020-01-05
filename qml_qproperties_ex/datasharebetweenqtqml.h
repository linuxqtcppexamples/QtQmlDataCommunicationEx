#ifndef DATASHAREBETWEENQTQML_H
#define DATASHAREBETWEENQTQML_H

#include <QObject>

class DataShareBetweenQtQML : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString strUserName READ strUserName WRITE setStrUserName NOTIFY strUserNameChanged)
public:
    explicit DataShareBetweenQtQML(QObject *parent = nullptr);
    QString strUserName();
    void setStrUserName(QString i_strUsername);
signals:
    void strUserNameChanged();
public slots:
private:
    QString m_strUserName;
};

#endif // DATASHAREBETWEENQTQML_H
