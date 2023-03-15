#ifndef TELLO_H
#define TELLO_H

#include <QObject>
#include <QDebug>
#include <QThread>

#include <Tello/TelloEnumTypes.h>
#include <Tello/TelloCommand.h>
#include <Tello/TelloState.h>
#include <Tello/TelloStream.h>

#define TELLO_DEBUG_OUTPUT false

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
    bool started, camera_enabled;

    void enableStream();
    void disableStream();

    QThread *thread_command;
    QThread *thread_state;
    QThread *thread_stream;

    QString address_str = "192.168.10.1";
    quint16 port_command = 8889;
    quint16 port_state = 8890;
    quint16 port_stream = 11111;

private slots:
    void cameraAvailable();
};

#endif // TELLO_H
