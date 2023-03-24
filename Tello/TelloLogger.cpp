#include "TelloLogger.h"

TelloLogger::TelloLogger(){
}

void TelloLogger::write2log(QString str){
    QFile outFile("Tello_log.txt");
    QString dateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << dateTime << " -> " << str << "\n";
}
