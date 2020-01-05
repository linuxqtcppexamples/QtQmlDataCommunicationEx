#ifndef INVOKE_EX_H
#define INVOKE_EX_H

#include <QObject>

class InvokeEx : public QObject
{
    Q_OBJECT
public:
    explicit InvokeEx(QObject *parent = nullptr);
    Q_INVOKABLE void cppMethod(const QString &msg);
signals:

public slots:
    void cppSlot(QString number);
};

#endif // INVOKE_EX_H
