#include "TelloCommand.h"

TelloCommand::TelloCommand(QHostAddress a, quint16 p): ip(a), port(p){
    if(TELLO_COMMAND_DEBUG_OUTPUT)
        qDebug() << this << "Constructed on" << QThread::currentThread();
    isRunning = false;

    socket = new QUdpSocket;

    if(!socket->bind(port)){
        qDebug() << socket->errorString();
        emit alertSignal(TelloAlerts::SOCKET_CONNECTION_FAILED);
    }
    else{
        if(TELLO_COMMAND_DEBUG_OUTPUT)
            qDebug() << "Local bind ready on " << socket->localAddress() << ":" << socket->localPort();
        connect(socket, &QUdpSocket::readyRead, this, &TelloCommand::readResponse, Qt::DirectConnection);
        sdk_mode_enabled = false;
    }
}

TelloCommand::~TelloCommand(){
    delete socket;
    if(TELLO_COMMAND_DEBUG_OUTPUT)
        qDebug() << this << "Deconstructed on" << QThread::currentThread();
}

void TelloCommand::takeoff(){
    if(isRunning && !flying){
        flying = true;
        sendCommand_generic("takeoff");
    }
}

void TelloCommand::land(){
    if(isRunning && flying){
        flying = false;
        sendCommand_generic("land");
    }
}

void TelloCommand::emergency(){
    if(TELLO_COMMAND_DEBUG_OUTPUT)
        qDebug() << "Sent: EMERGENCY on" << QThread::currentThread();
    lastCommandUsed = "emergency";
    socket->writeDatagram("emergency", ip, port);
    lastTimeCommandSent = QDateTime::currentSecsSinceEpoch();
    generic_command_requested = true;
    flying = false;
}

void TelloCommand::setPosition(int a, int b, int c, int d){
    QByteArray cmd = "rc " + QByteArray::number(a) + " " + QByteArray::number(b) + " " + QByteArray::number(c) + " " + QByteArray::number(d);
    sendCommand_generic(cmd);
}

void TelloCommand::setSpeed(int speed){
    if(speed >= 10 && speed <= 100){
        sendCommand_generic("speed " + QByteArray::number(speed));
    }
}

void TelloCommand::sendCommand_generic(QByteArray cmd){
    if(isRunning){
        if(TELLO_COMMAND_DEBUG_OUTPUT)
            qDebug() << "Sent: " << cmd << "on" << QThread::currentThread();
        lastCommandUsed = cmd;
        lastTimeCommandSent = QDateTime::currentSecsSinceEpoch();
        socket->writeDatagram(cmd, ip, port);
        generic_command_requested = true;
    }
}

void TelloCommand::sendCommand_SNR(){
    socket->writeDatagram("wifi?", ip, port);
    lastTimeCommandSent = QDateTime::currentSecsSinceEpoch();
    snr_requested = true;
}

void TelloCommand::run(){
    sdk_mode_enabled = false;
    streamEnabled = false;
    quint16 responseDelay;
    while(isRunning){
        if(sdk_mode_enabled){
            if(!streamEnabled){
                sendCommand_generic("streamon");
            }
            responseDelay = lastTimeCommandSent - lastTimeResponseReceived;
            //qDebug() << "response delay: " << QString::number(responseDelay) << "s";
            if(responseDelay >= 10){
                emit alertSignal(TelloAlerts::TELLO_CONNECTION_TIMEOUT);
                QThread::sleep(5);
                sdk_mode_enabled = false;
                streamEnabled = false;
            }
            else if(responseDelay >= 5){
                emit alertSignal(TelloAlerts::TELLO_CONNECTION_NO_RESPONSE);
            }
            else{
                emit alertSignal(TelloAlerts::TELLO_CONNECTION_ESTABLISHED);
            }

            sendCommand_SNR();
        }
        else{
            sendCommand_generic("command");
            emit alertSignal(TelloAlerts::TELLO_CONNECTION_WAITING);
        }
        QThread::sleep(2);
    }
}

void TelloCommand::readResponse(){

    while (socket->hasPendingDatagrams())
    {
        QHostAddress sender;
        quint16 port;
        QByteArray datagram;
        datagram.resize(socket->pendingDatagramSize());
        socket->readDatagram(datagram.data(),datagram.size(),&sender,&port);

        if(TELLO_COMMAND_DEBUG_OUTPUT){
            if(snr_requested){
                qDebug() << "\"snr\"" << ":" << port << "->" << datagram;
            }
            else{
                qDebug() << lastCommandUsed << ":" << port << "->" << datagram;
            }
        }



        lastTimeResponseReceived = QDateTime::currentSecsSinceEpoch();
        if(generic_command_requested){
            generic_command_requested = false;
            if(lastCommandUsed == "streamon"){
                streamEnabled = true;
                emit cameraEnabled();
            }
        }


        if(datagram == "timeout"){ emit responseSignal(TelloResponse::TIMEOUT, datagram); }
        else if(datagram == "error"){ emit responseSignal(TelloResponse::ERROR, datagram); }
        else if(datagram == "ok"){
            if(!sdk_mode_enabled){
                sdk_mode_enabled = true;
            }
            emit responseSignal(TelloResponse::OK, datagram);
        }
        else if(datagram.contains("\r\n")){
            if(snr_requested){
                snr_requested = false;
                snr_value = datagram.toInt();
            }
            else{
                emit responseSignal(TelloResponse::VALUE, datagram);
            }
        }
        else if(datagram == "out of range"){ emit responseSignal(TelloResponse::OUT_OF_RANGE, datagram); }
        else{ emit responseSignal(TelloResponse::UNDEFINED, datagram); }
    }
}
