#include "tello.h"
#include <QTimer>

Tello::Tello(){
    socket = new QUdpSocket(this);
    socketState = new QUdpSocket(this);
    timerSNR = new QTimer(this);
    timerLinkCheck = new QTimer(this);
    connect(timerSNR, &QTimer::timeout, this, QOverload<>::of(&Tello::requestSNR));
    connect(timerLinkCheck, &QTimer::timeout, this, QOverload<>::of(&Tello::linkCheck));

    mid=0, x=0, y=0, z=0, pitch=0, roll=0, yaw=0, templ=0, temph=0, tof=0, h=0, bat=0, snrValue=0;
    vgx=0, vgy=0, vgz=0, baro=0, time=0, agx=0, agy=0, agz=0;
}

Tello::~Tello(){}

void Tello::init(QHostAddress _ip, quint16 cmd, quint16 state, quint16 stream){

    ip = _ip;
    udp_cmd_port = cmd;
    udp_state_port = state;
    udp_videostream_port = stream;

    if (socket->isOpen()) {
        socket->abort();
    }

    if (!socket->bind(udp_cmd_port)) {
        qInfo() << socket->errorString();
    }
    else{
        qInfo() << "Local bind ready on " << socket->localAddress() << ":" << socket->localPort();
        connect(socket, &QUdpSocket::readyRead,this, &Tello::readCommandResponse);
    }


    if (socketState->isOpen()) {
        socket->abort();
    }
    if (!socketState->bind(8890)) {
        qInfo() << socket->errorString();
    }
    else{
        qInfo() << "Local bind ready on " << socket->localAddress() << ":" << socket->localPort();
        connect(socketState, &QUdpSocket::readyRead,this, &Tello::readStateResponse);
    }


    sendCommand_generic("command");
    waitingConnectionCheck = true;

    timerSNR->start(TIMER_SNR_UPDATE_RATE);
    timerLinkCheck->start(TIMER_LINK_CHECK_UPDATE_RATE);

    timeOfLastSentCommand = QDateTime::currentSecsSinceEpoch();
    timeOfLastReponse = QDateTime::currentSecsSinceEpoch();

    connectionStatus = Status::NO_RESPONSE;
    emit connectionStatusChanged();
}

void Tello::close(){
    //sendCommand_generic("reboot");

    socket->close();
    socketState->close();

    timerSNR->stop();
    timerLinkCheck->stop();

    //qDebug() << "Command & State Disconnected";

    mid=0, x=0, y=0, z=0, pitch=0, roll=0, yaw=0, templ=0, temph=0, tof=0, h=0, bat=0;
    vgx=0, vgy=0, vgz=0, baro=0, time=0, agx=0, agy=0, agz=0;
    emit newStateDataAvailable();

    connectionStatus = Status::CLOSED;
    emit connectionStatusChanged();
}

void Tello::sendCommand_generic(QByteArray cmd){
    qDebug() << "Sent: " << cmd;
    lastCommandUsed = cmd;
    timeOfLastSentCommand = QDateTime::currentSecsSinceEpoch();
    socket->writeDatagram(cmd, ip, udp_cmd_port);
}

void Tello::enableCamera(){
    sendCommand_generic("streamon");
    waitingCameraResponse = true;
}

void Tello::disableCamera(){
    sendCommand_generic("streamoff");
}

cv::VideoCapture Tello::cameraStream(){
    QString url = "udp://@" + ip.toString() + ":" + QString::number(udp_videostream_port);
    //cv::VideoCapture stream("udp://@192.168.10.1:11111");
    //cv::VideoCapture stream(0);
    cv::VideoCapture *stream = new cv::VideoCapture();
    stream->open(url.toStdString());
    return *stream;
}

void Tello::move(QString pos, unsigned length){

    if(length < 20 || length > 500){
        return;
    }

    if(pos == "up"){
        sendCommand_generic("up" + QByteArray::number(length));
    }
    else if(pos == "down"){
        sendCommand_generic("down " + QByteArray::number(length));
    }
    else if(pos == "left"){
        sendCommand_generic("left " + QByteArray::number(length));
    }
    else if(pos == "right"){
        sendCommand_generic("right " + QByteArray::number(length));
    }
    else if(pos == "forward"){
        sendCommand_generic("forward " + QByteArray::number(length));
    }
    else if(pos == "back"){
        sendCommand_generic("back " + QByteArray::number(length));
    }
}

void Tello::requestSNR(){
    sendCommand_generic("wifi?");
    waitingSNRvalue = true;
    //qDebug() << "snr";
}

void Tello::readCommandResponse(){
    QHostAddress sender;
    quint16 port;

    while (socket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(socket->pendingDatagramSize());
        socket->readDatagram(datagram.data(),datagram.size(),&sender,&port);

        qDebug() << sender.toString() << ":" << port << " -> " << datagram;

        if(waitingConnectionCheck){
            waitingConnectionCheck = false;
            connectionStatus = Status::CONNECTED;
            emit connectionStatusChanged();
        }
        else if(waitingCameraResponse){
            waitingCameraResponse = false;
            emit cameraStreamAvailable();
        }
        else if(waitingSNRvalue){
            waitingSNRvalue = false;
            snrValue = datagram.toInt();
        }

        timeOfLastReponse = QDateTime::currentSecsSinceEpoch();
        commandReponseData = datagram;
        emit newCommandReponseAvailable();
    }
}

void Tello::readStateResponse(){
    QHostAddress sender;
    quint16 port;

    while (socketState->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(socketState->pendingDatagramSize());
        socketState->readDatagram(datagram.data(),datagram.size(),&sender,&port);
        //qDebug() <<"Message From :: " << sender.toString();
        //qDebug() <<"Port From :: "<< port;
        //qDebug() <<"Message :: " << datagram;
        parseStatus(datagram);
    }
}

void Tello::parseStatus(QString str){
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

    emit newStateDataAvailable();
}

void Tello::linkCheck(){
    int diff = int(timeOfLastReponse - timeOfLastSentCommand);
    if(abs(diff) >= 15){
        qDebug() << "Ne reponse since 15s";
        timeOfLastReponse = timeOfLastSentCommand;
        connectionStatus = Status::NO_RESPONSE_TIMEOUT;
        emit connectionStatusChanged();
    }
    else{
        //connectionStatus = Status::CONNECTED;
        //emit connectionStatusChanged();
    }
}
