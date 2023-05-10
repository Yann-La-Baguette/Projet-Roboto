#include "database.h"

database::database(){

    db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName("./robot_project.sqlite");

    if (!db.open()) {
        QString error = QString("Impossible d'ouvrir la base de données: %1").arg(db.lastError().text());
        qDebug() << error;
    }

    QSqlQuery query;
    query.prepare("CREATE TABLE robot_project_data (id INTEGER PRIMARY KEY AUTOINCREMENT, Height INTEGER, Pitch REAL, Roll REAL, Yaw REAL, Battery INTEGER, FOREIGN KEY (id) REFERENCES robot_project(id))");
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
    query.prepare("INSERT INTO robot_project_data (Height, Pitch, Roll, Yaw, Battery) VALUES (:height, :pitch, :roll, :yaw, :battery, :picture)");
    query.bindValue(":height", height);
    query.bindValue(":pitch", pitch);
    query.bindValue(":roll", roll);
    query.bindValue(":yaw", yaw);
    query.bindValue(":battery", battery);

    if(!query.exec()){
        qDebug() << "Erreur lors de l'insertion : " << query.lastError().text();
    }
}

void database::insertPicture(QString picturePath){
    QSqlQuery query;
    query.prepare("INSERT INTO robot_project_pictures (:picture)");
    query.bindValue(":picture", picturePath);
}
