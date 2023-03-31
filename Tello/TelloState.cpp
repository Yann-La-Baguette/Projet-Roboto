#include "TelloState.h"

TelloState::TelloState(QHostAddress a, quint16 p): ip(a), port(p)
{
    resetVariables();

    isRunning = false;

    socket = new QUdpSocket;

    if(!socket->bind(port)){
        qDebug() << socket->errorString();
    }
    else{
        if(TELLO_DEBUG_OUTPUT)
            qDebug() << "Local bind ready on " << socket->localAddress() << ":" << socket->localPort();
        connect(socket, &QUdpSocket::readyRead, this, &TelloState::readResponse, Qt::DirectConnection);

        timer = new QTimer;
        lastTimeResponseReceived = 0;
        connect(timer, &QTimer::timeout, this, &TelloState::timeoutCheck);
        timer->start(100);
    }
}

TelloState::~TelloState(){
    delete socket;
}

void TelloState::resetVariables()
{
    mid=0;
    x=0;
    y=0;
    z=0;
    pitch=0;
    roll=0;
    yaw=0;
    templ=0;
    temph=0;
    tof=0;
    h=0;
    bat=0;
    snrValue=0;

    mpry[0] = 0;
    mpry[1] = 0;
    mpry[2] = 0;

    vgx=0;
    vgy=0;
    vgz=0;
    baro=0;
    time=0;
    agx=0;
    agy=0;
    agz=0;

}

void TelloState::readResponse(){
    QHostAddress sender;
    quint16 port;

    while (socket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(socket->pendingDatagramSize());
        socket->readDatagram(datagram.data(),datagram.size(),&sender,&port);
        if(TELLO_STATE_OUTPUT)
            qDebug() << datagram;
        parseStatus(datagram);
    }
}

void TelloState::parseStatus(QString str){
    // Split the string into a list of key-value pairs
    QStringList keyValueList = str.split(';');

    // Iterate through the key-value pairs and assign the values to the variables
    foreach (const QString& keyValue, keyValueList) {
        QStringList pair = keyValue.split(':');
        if (pair.size() == 2) {
            QString key = pair[0];
            QString value = pair[1];

            if (key == "mid") {
                mid = value.toInt();
            }
            else if (key == "x") {
                x = value.toInt();
            }
            else if (key == "y") {
                y = value.toInt();
            }
            else if (key == "z") {
                z = value.toInt();
            }
            else if (key == "mpry") {
                QStringList mpryValues = value.split(',');
                if (mpryValues.size() == 3) {
                    mpry[0] = mpryValues[0].toDouble();
                    mpry[1] = mpryValues[1].toDouble();
                    mpry[2] = mpryValues[2].toDouble();
                }
            }
            else if (key == "pitch") {
                pitch = value.toInt();
            }
            else if (key == "roll") {
                roll = value.toInt();
            }
            else if (key == "yaw") {
                yaw = value.toInt();
            }
            else if (key == "vgx") {
                vgx = value.toDouble();
            }
            else if (key == "vgy") {
                vgy = value.toDouble();
            }
            else if (key == "vgz") {
                vgz = value.toDouble();
            }
            else if (key == "templ") {
                templ = value.toInt();
            }
            else if (key == "temph") {
                temph = value.toInt();
            }
            else if (key == "tof") {
                tof = value.toInt();
            }
            else if (key == "h") {
                h = value.toInt();
            }
            else if (key == "bat") {
                bat = value.toInt();
            }
            else if (key == "baro") {
                baro = value.toDouble();
            }
            else if (key == "time") {
                time = value.toInt();
            }
            else if (key == "agx") {
                agx = value.toDouble();
            }
            else if (key == "agy") {
                agy = value.toDouble();
            }
            else if (key == "agz") {
                agz = value.toDouble();
            }
        }
    }

    lastTimeResponseReceived = QDateTime::currentSecsSinceEpoch();
    emit dataAvailable();
}

void TelloState::timeoutCheck(){
    if(QDateTime::currentSecsSinceEpoch() > (lastTimeResponseReceived + 1)){
        emit status(TelloStatus::TELLO_DISCONNECTED);
    }
    else{
        emit status(TelloStatus::TELLO_CONNECTED);
    }
}
