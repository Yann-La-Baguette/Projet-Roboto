#include "Tello.h"

Tello::Tello(){
    //Init Tello Command Object
    tello_command = new TelloCommand(QHostAddress(address_str), port_command);

    //Init Tello State Object
    tello_state = new TelloState(QHostAddress(address_str), port_state);
    connect(tello_state,&TelloState::status,tello_command,&TelloCommand::updateStatus);

    //Init Tello Stream Object
    tello_stream = new TelloStream(address_str, port_stream);
    connect(tello_command,&TelloCommand::cameraEnabled,tello_stream,&TelloStream::enableStream);
    connect(tello_command,&TelloCommand::cameraDisable,tello_stream,&TelloStream::disableStream);
}

Tello::~Tello(){
    delete tello_state;
    delete tello_command;
    delete tello_stream;
}

void Tello::start(){
    if(started){
        qDebug() << "Tello Already running";
        return;
    }
    started = true;
    qDebug() << "Tello Started";
    tello_command->running(true);
}

void Tello::stop(){
    if(started == false){
        qDebug() << "Tello Not started";
        return;
    }
    started = false;
    qDebug() << "Tello Stopped";
    tello_command->running(false);
    tello_stream->disableStream();
}
