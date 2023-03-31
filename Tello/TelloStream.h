#ifndef TELLOSTREAM_H
#define TELLOSTREAM_H

#include <QObject>
#include <QtConcurrent/QtConcurrent>
#include <QDebug>
#include <QPixmap>
#include <opencv2/opencv.hpp>
#include <Tello/TelloEnumTypes.h>

using namespace cv;

class TelloStream : public QObject
{
    Q_OBJECT

public:
    TelloStream(QString a, quint16 p);
    ~TelloStream();
    bool isCameraEnabled(){ return capture->isOpened(); };
    QPixmap mat2pixmap(Mat);
    QSize size2qsize(cv::Size);

private:
    VideoCapture *capture;
    String url;
    QString address;
    quint16 port;
    QSize size;
    bool openRequest, releaseRequest;

    QFuture<void> future;
    bool runConcurrent;

public slots:
    void enableStream();
    void disableStream();

signals:
    void newFrame(QPixmap);
};

#endif // TELLOSTREAM_H
