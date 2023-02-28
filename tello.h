#ifndef TELLO_H
#define TELLO_H

#include <opencv2\opencv.hpp>
#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QHostAddress>
#include <QTimer>
#include <QDateTime>

#define TIMER_SNR_UPDATE_RATE 5000
#define TIMER_LINK_CHECK_UPDATE_RATE 1000

class Tello: public QObject{
    Q_OBJECT

public:
    Tello();
    ~Tello();

    // Enum for connection status
    enum class Status {
        CONNECTED,
        CLOSED,
        NO_RESPONSE,
        NO_RESPONSE_TIMEOUT,
        ERROR
    };

    void init(QHostAddress _ip, quint16 cmd, quint16 state, quint16 stream);
    void close();

    // Commands Methods
    void sendCommand_generic(QByteArray cmd);
    Status getStatus(){ return connectionStatus; };
    int getWifiSNR(){ return snrValue; };
    QString getCommandResponseData(){ return commandReponseData; };
    QString getLastCommmandUsed(){ return lastCommandUsed; };
    void enableCamera();
    void disableCamera();
    cv::VideoCapture cameraStream();
    void move(QString pos, unsigned length);

    // Getter functions for int variables
    int getMid() { return mid; }
    int getX() { return x; }
    int getY() { return y; }
    int getZ() { return z; }
    int getPitch() { return pitch; }
    int getRoll() { return roll; }
    int getYaw() { return yaw; }
    int getTempLowest() { return templ; }
    int getTempHighest() { return temph; }
    int getDistanceTof() { return tof; }
    int getHeight() { return h; }
    int getBattery() { return bat; }

    // Getter functions for double variables
    double *getMpry() { return mpry; }
    double getVgx() { return vgx; }
    double getVgy() { return vgy; }
    double getVgz() { return vgz; }
    double getBaro() { return baro; }
    double getTime() { return time; }
    double getAgx() { return agx; }
    double getAgy() { return agy; }
    double getAgz() { return agz; }

signals:
    void connectionStatusChanged();
    void newCommandReponseAvailable();
    void newStateDataAvailable();
    void cameraStreamAvailable();

private:
    int mid, x, y, z, pitch, roll, yaw, templ, temph, tof, h, bat, snrValue;
    double mpry[3], vgx, vgy, vgz, baro, time, agx, agy, agz;

    QUdpSocket *socket;
    QUdpSocket *socketState;
    QTimer *timerSNR, *timerLinkCheck;
    Status connectionStatus;
    bool waitingConnectionCheck, waitingCameraResponse = false, waitingSNRvalue;

    QString commandReponseData, lastCommandUsed;
    quint64 timeOfLastSentCommand, timeOfLastReponse;

    QHostAddress ip;
    quint16 udp_cmd_port;
    quint16 udp_state_port;
    quint16 udp_videostream_port;

    void requestSNR();
    void linkCheck();
    void readCommandResponse();
    void readStateResponse();
    void parseStatus(QString data);
};



#endif // TELLO_H
