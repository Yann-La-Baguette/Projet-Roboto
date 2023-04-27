#ifndef TELLOLOGGER_H
#define TELLOLOGGER_H

#include <QObject>
#include <QDebug>
#include <QDateTime>
#include <QFile>

class TelloLogger: QObject
{
    Q_OBJECT
public:
    TelloLogger();

public slots:
    static void write2log(QString str);

private:
    //static QFile *file;
};

#endif // TELLOLOGGER_H
