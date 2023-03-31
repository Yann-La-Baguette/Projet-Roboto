#ifndef TELLO_H
#define TELLO_H

#include <QObject>
#include <QDebug>

#include <Tello/TelloCommand.h>
#include <Tello/TelloState.h>
#include <Tello/TelloStream.h>
#include <Tello/TelloEnumTypes.h>

class Tello: public QObject
{
    Q_OBJECT

public:
    Tello();
    ~Tello();
    void start();
    void stop();
    bool isStarted(){ return started; };

    TelloCommand *tello_command;
    TelloState *tello_state;
    TelloStream *tello_stream;

private:
    bool started = false;

    QString address_str = "192.168.10.1";
    quint16 port_command = 8889;
    quint16 port_state = 8890;
    quint16 port_stream = 11111;
};

#endif // TELLO_H
