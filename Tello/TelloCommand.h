#ifndef TELLOCOMMAND_H
#define TELLOCOMMAND_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QTimer>
#include <QDateTime>
#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QHostAddress>
#include <Tello/TelloEnumTypes.h>
#include <Tello/TelloLogger.h>

#define TELLO_COMMAND_DEBUG_OUTPUT true

class TelloCommand: public QObject
{
    Q_OBJECT

public:
    TelloCommand(QHostAddress a, quint16 p);
    ~TelloCommand();

    /*####### Commands #######*/
    void takeoff();
    void land();
    void emergency();
    void setPosition(int, int, int, int);
    void setSpeed(int);

    /*####### Network #######*/
    void sendCommand_generic(QByteArray cmd);

    /*####### Getter #######*/
    bool sdkModeEnabled(){ return sdk_mode_enabled; };
    bool isFlying(){ return flying; };
    QString getLastCommandUsed(){ return lastCommandUsed; };
    int getSNR(){ return snr_value;};

public slots:
    void running(bool r){ isRunning = r; };
    void updateStatus(TelloStatus n){ status = n; };

private:
    QUdpSocket *socket;
    QHostAddress ip;
    quint16 port;
    TelloStatus status;

    QTimer *timer;
    void timerLoop();

    bool isRunning, flying, streamEnabled, sdk_mode_enabled, snr_requested, generic_command_requested, wait_command_requested;
    int snr_value;

    qint64 lastTimeCommandSent, lastTimeSnrSent;
    QString lastCommandUsed;

private slots:
    void sendCommand_SNR();
    void readResponse();

signals:
    void responseSignal(TelloResponse, QString);
    void cameraEnabled();
    void cameraDisable();

};

#endif // TELLO_COMMANDWORKER_H
