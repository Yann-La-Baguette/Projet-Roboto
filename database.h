#ifndef DATABASE_H
#define DATABASE_H

#include <QString>
#include <QtSql/qsqldatabase.h>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>

class database
{
public:
    database();
    ~database();

    void insertData(int height, double pitch, double roll, double yaw, int battery);
    void insertPicture(QString picturePath);
private:
    QSqlDatabase db;
};

#endif // DATABASE_H
