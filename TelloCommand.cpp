#include "TelloCommand.h"

TelloCommand::TelloCommand(QHostAddress a, quint16 p): ip(a), port(p){
    qDebug() << this << "Constructed on" << QThread::currentThread();
    isRunning = false;

    socket = new QUdpSocket;

    if(!socket->bind(port)){
        qDebug() << socket->errorString();
        emit alertSignal(TelloAlerts::SOCKET_CONNECTION_FAILED);
    }
    else{
        qDebug() << "Local bind ready on " << socket->localAddress() << ":" << socket->localPort();
        connect(socket, &QUdpSocket::readyRead, this, &TelloCommand::readResponse, Qt::DirectConnection);
        sdk_mode_enabled = false;
    }
}

TelloCommand::~TelloCommand(){
    delete socket;
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
    qDebug() << "Sent: EMERGENCY on" << QThread::currentThread();
    lastCommandUsed = "emergency";
    socket->writeDatagram("emergency", ip, port);
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
        qDebug() << "Sent: " << cmd << "on" << QThread::currentThread();
        lastCommandUsed = cmd;
        socket->writeDatagram(cmd, ip, port);
        generic_command_requested = true;
    }
}

void TelloCommand::sendCommand_SNR(){
    socket->writeDatagram("wifi?", ip, port);
    snr_requested = true;
}

/*
void TelloCommand::sendCommand_waitResponse(QByteArray cmd){
    wait_command_requested = true;
    sendCommand_generic(cmd);
    QTimer::singleShot(200, this, &TelloCommand::waitResponse);
}

void TelloCommand::waitResponse(){
    if(generic_command_requested){
        qDebug() << "Pas repondu";
        //sendCommand_waitResponse(lastCommandUsed);
    }
    else{
        qDebug() << "Repondu";
    }
}
*/

void TelloCommand::run(){
    sdk_mode_enabled = false;
    while(isRunning){
        if(sdk_mode_enabled){
            sendCommand_SNR();
        }
        else{
            sendCommand_generic("command");
        }
        QThread::sleep(3);
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

        if(snr_requested){
            qDebug() << "\"snr\"" << ":" << port << "->" << datagram;
        }
        else{
            qDebug() << lastCommandUsed << ":" << port << "->" << datagram;
        }


        if(generic_command_requested){
            generic_command_requested = false;
            if(lastCommandUsed == "streamon"){
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
                emit wifiSnrSignal(datagram.toInt());
            }
            else{
                emit responseSignal(TelloResponse::VALUE, datagram);
            }
        }
        else if(datagram == "out of range"){ emit responseSignal(TelloResponse::OUT_OF_RANGE, datagram); }
        else{ emit responseSignal(TelloResponse::UNDEFINED, datagram); }
    }
}
