#include "Tello.h"

Tello::Tello(){
    started = false;

    QThread::currentThread()->setObjectName("Tello Main Thread");
    qDebug() << "Starting" << QThread::currentThread();

    //Init Tello Command Object and bind to Thread
    tello_command = new TelloCommand(QHostAddress(address_str), port_command);
    thread_command = new QThread();
    thread_command->setObjectName("Tello Command Thread");
    tello_command->moveToThread(thread_command);
    connect(thread_command,&QThread::started,tello_command,&TelloCommand::run);

    //Init Tello State Object and bind to Thread
    tello_state = new TelloState(QHostAddress(address_str), port_state);
    thread_state = new QThread();
    thread_state->setObjectName("Tello State Thread");
    tello_state->moveToThread(thread_state);
    connect(thread_state,&QThread::started,tello_state,&TelloState::run);

    //Init Tello Command Object and Thread
    tello_stream = new TelloStream(address_str, port_stream);
    thread_stream = new QThread();

    connect(tello_command,&TelloCommand::cameraEnabled,this,&Tello::cameraAvailable);
}

Tello::~Tello(){
    delete tello_state;
    delete tello_command;
    delete tello_stream;

    delete thread_command;
    delete thread_state;
    delete thread_stream;
}

void Tello::start(){
    if(started){
        qDebug() << "Tello Already running";
        return;
    }
    started = true;
    thread_command->start();
    tello_command->running(true);
    thread_state->start();
}

void Tello::stop(){
    if(started == false){
        qDebug() << "Not started";
        return;
    }
    started = false;

    tello_command->running(false);
    thread_command->quit();
    thread_command->wait();

    thread_state->quit();
    thread_state->wait();
}

void Tello::enableStream(){
    tello_command->sendCommand_generic("streamon");
}

void Tello::disableStream(){
    if(!camera_enabled){
        qDebug() << "Stream not enabled";
        return;
    }
    thread_stream->quit();
    thread_stream->wait();
}

void Tello::cameraAvailable(){
    camera_enabled = true;
    thread_stream->setObjectName("Tello Stream Thread");
    tello_stream->moveToThread(thread_stream);
    connect(thread_stream,&QThread::started,tello_stream,&TelloStream::run);
    thread_stream->start();
    qDebug() << "Camera Thread Started";
}
