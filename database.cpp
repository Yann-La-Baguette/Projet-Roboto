#include "database.h"

database::database(){

    db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName("./robot_project.sqlite");

    if (!db.open()) {
        QString error = QString("Impossible d'ouvrir la base de données: %1").arg(db.lastError().text());
        qDebug() << error;
    }

    QSqlQuery query;
    query.prepare("CREATE TABLE robot_project_data (id INTEGER PRIMARY KEY AUTOINCREMENT, Height INTEGER, Pitch REAL, Roll REAL, Yaw REAL, Battery INTEGER, Timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP, Picture_id INTEGER, FOREIGN KEY (Picture_id) REFERENCES robot_project_pictures(id))");
    if (!query.exec()) {
        QString error = QString("Erreur lors de la création de la table robot_project_data: %1").arg(query.lastError().text());
        qDebug() << error;
    }

    query.prepare("CREATE TABLE robot_project_pictures (id INTEGER PRIMARY KEY AUTOINCREMENT, Picture TEXT, FOREIGN KEY (id) REFERENCES robot_project(id))");
    if (!query.exec()) {
        QString error = QString("Erreur lors de la création de la table robot_project_images: %1").arg(query.lastError().text());
        qDebug() << error;
    }


}
database::~database(){
    db.close();
}

void database::insertData(int height, double pitch, double roll, double yaw, int battery){
    QSqlQuery query;


    query.prepare("INSERT INTO robot_project_data (Height, Pitch, Roll, Yaw, Battery, Picture_id) VALUES (:height, :pitch, :roll, :yaw, :battery, :pictureId)");
    query.bindValue(":height", height);
    query.bindValue(":pitch", pitch);
    query.bindValue(":roll", roll);
    query.bindValue(":yaw", yaw);
    query.bindValue(":battery", battery);
    query.bindValue(":pictureId", pictureID);

    query.exec();


}

void database::insertPicture(QString picturePath){
    QSqlQuery query;
    query.prepare("INSERT INTO robot_project_pictures (Picture) VALUES (:picture)");
    query.bindValue(":picture", picturePath);

    query.exec();

    pictureID = query.lastInsertId().toInt();
}
