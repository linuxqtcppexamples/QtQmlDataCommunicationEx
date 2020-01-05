#ifndef REVISIONEX_H
#define REVISIONEX_H

#include <QObject>

class RevisionEx : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int normalProperty READ normalProperty)
    Q_PROPERTY(int newProperty READ newProperty REVISION 1)
    Q_PROPERTY(int newerProperty READ newerProperty REVISION 2)

public:
    RevisionEx();
public slots:
    int normalProperty();
    Q_REVISION(1) int newProperty();
    Q_REVISION(2) int newerProperty();
};

#endif // REVISIONEX_H
