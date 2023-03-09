#ifndef TELLO_H
#define TELLO_H

#include <QObject>
#include <QDebug>
#include <QThread>

#include <TelloEnumTypes.h>
#include <TelloCommand.h>
#include <TelloState.h>
#include <TelloStream.h>

class Tello: public QObject
{
    Q_OBJECT

public:
    Tello();
    ~Tello();
    void start();
    void stop();
    void enableStream();
    void disableStream();
    bool isStarted(){ return started; };

    TelloCommand *tello_command;
    TelloState *tello_state;
    TelloStream *tello_stream;

private:
    bool started, camera_enabled;

    QThread *thread_command;
    QThread *thread_state;
    QThread *thread_stream;

    QString address_str = "192.168.10.1";
    //QString address_str = "192.168.1.57";
    quint16 port_command = 8889;
    quint16 port_state = 8890;
    quint16 port_stream = 11111;

private slots:
    void cameraAvailable();
};

#endif // TELLO_H
