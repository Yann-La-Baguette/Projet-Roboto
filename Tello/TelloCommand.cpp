#include "TelloCommand.h"

TelloCommand::TelloCommand(QHostAddress a, quint16 p): ip(a), port(p){

    isRunning = false;
    flying = false;
    streamEnabled = false;
    sdk_mode_enabled = false;
    snr_requested = false;
    generic_command_requested = false;
    wait_command_requested = false;
    snr_value = 0;
    lastTimeCommandSent = 0;
    lastTimeSnrSent = 0;

    socket = new QUdpSocket;
    if(!socket->bind(port)){
        qDebug() << "Tello Command bind error" << socket->errorString();
    }
    else{
        if(TELLO_DEBUG_OUTPUT)
            qDebug() << "Tello Command bind ready on " << socket->localAddress() << ":" << socket->localPort();
        connect(socket, &QUdpSocket::readyRead, this, &TelloCommand::readResponse, Qt::DirectConnection);

        timer = new QTimer;
        connect(timer, &QTimer::timeout, this, &TelloCommand::timerLoop);
        timer->start(2000);
    }
}

TelloCommand::~TelloCommand(){
    delete timer;
    delete socket;
}

/*####### Commands #######*/
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
    lastCommandUsed = "emergency";
    socket->writeDatagram("emergency", ip, port);
    lastTimeCommandSent = QDateTime::currentMSecsSinceEpoch();
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

void TelloCommand::flip(){
    if(isRunning && flying){
        sendCommand_generic("flip f");
    }
}

/*####### Network #######*/
void TelloCommand::sendCommand_generic(QByteArray cmd){
    if(isRunning){
        if(TELLO_DEBUG_OUTPUT)
            qDebug() << "Sent command:" << cmd;
        lastCommandUsed = cmd;
        lastTimeCommandSent = QDateTime::currentMSecsSinceEpoch();
        socket->writeDatagram(cmd, ip, port);
        generic_command_requested = true;
    }
    else{
        qDebug() << "Tello Command Try to send command but was not running";
    }
}

void TelloCommand::sendCommand_SNR(){
    socket->writeDatagram("wifi?", ip, port);
    lastTimeSnrSent = QDateTime::currentMSecsSinceEpoch();
    snr_requested = true;
}

void TelloCommand::readResponse(){

    while (socket->hasPendingDatagrams())
    {
        //Read response
        QHostAddress sender;
        quint16 port;
        QByteArray datagram;
        datagram.resize(socket->pendingDatagramSize());
        socket->readDatagram(datagram.data(),datagram.size(),&sender,&port);

        //Print debug
        if(TELLO_DEBUG_OUTPUT){
            quint16 responseDelay;
            if(snr_requested == true && generic_command_requested == false){
                responseDelay = abs(QDateTime::currentMSecsSinceEpoch() - lastTimeSnrSent);
                qDebug() << "Response from \"snr\" ->" << QString(datagram).trimmed() << "in" << QString::number(responseDelay) + "ms";
            }
            else{
                responseDelay = abs(QDateTime::currentMSecsSinceEpoch() - lastTimeCommandSent);
                qDebug() << "Response from" << lastCommandUsed <<  "->" << QString(datagram).trimmed() << "in" << QString::number(responseDelay) + "ms";
            }
        }



        if(generic_command_requested){
            generic_command_requested = false;

            if(lastCommandUsed == "command" && datagram == "ok"){
                sdk_mode_enabled = true;
            }
            else if(lastCommandUsed == "streamon" && datagram == "ok"){
                streamEnabled = true;
                emit cameraEnabled();
            }
        }


        //Send response signals
        if(datagram == "timeout"){ emit responseSignal(TelloResponse::TELLO_TIMEOUT, datagram); }
        else if(datagram == "error"){ emit responseSignal(TelloResponse::TELLO_ERROR, datagram); }
        else if(datagram == "ok"){ emit responseSignal(TelloResponse::TELLO_OK, datagram); }
        else if(datagram.contains("\r\n")){
            if(snr_requested){
               snr_requested = false;
               snr_value = datagram.toInt();
            }
            else{
                emit responseSignal(TelloResponse::TELLO_VALUE, datagram);
            }
        }
        else if(datagram == "out of range"){ emit responseSignal(TelloResponse::TELLO_OUT_OF_RANGE, datagram); }
        else{ emit responseSignal(TelloResponse::TELLO_UNDEFINED, datagram); }
    }
}

/*####### Algo #######*/
void TelloCommand::timerLoop(){

    if(!isRunning){
        return;
    }

    if(!sdk_mode_enabled){
        sendCommand_generic("command");
        //qDebug() << "Tello Command Waiting connection";
        return;
    }

    if(status == TelloStatus::TELLO_DISCONNECTED){
        sdk_mode_enabled = false;
        streamEnabled = false;
        emit cameraDisable();
        return;
    }

    if(!streamEnabled){
        sendCommand_generic("streamon");
        qDebug() << "Tello Command Waiting Video Stream";
    }

    sendCommand_SNR();
}
