#ifndef TELLOSTREAM_H
#define TELLOSTREAM_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QPixmap>
#include <opencv2/opencv.hpp>

#define TELLO_STREAM_DEBUG_OUTPUT false

using namespace cv;

class TelloStream : public QObject
{
    Q_OBJECT

public:
    TelloStream(QString a, quint16 p);
    ~TelloStream();
    void run();
    bool isCameraEnabled(){ return camera_enabled; };
    void stopStream();

    static QPixmap mat2pixmap(Mat);
    QSize getCaptureSize(){ return size; };
    QSize size2qsize(cv::Size);

private:
    bool startVideoCapture();

    VideoCapture *capture;
    String url;
    QString address;
    quint16 port;
    QSize size;
    bool camera_enabled;


signals:
    void newFrame(QPixmap);
};

#endif // TELLOSTREAM_H
