#include "TelloStream.h"

TelloStream::TelloStream(QString a, quint16 p): address(a),port(p){
    capture = new VideoCapture;
    url = "udp://" + address.toStdString() + ":" + QString::number(port).toStdString() + "?overrun_nonfatal=1&fifo_size=50000000";

    openRequest = false;
    releaseRequest = false;
    runConcurrent = true;

    future = QtConcurrent::run([this]() {
        Mat frame;
        if(TELLO_DEBUG_OUTPUT){
            qDebug() << "StreamThread" << QThread::currentThread();
        }
        while(runConcurrent){

            if(openRequest){
                openRequest = false;
                capture->open(url);
                //capture->open(0);
                qDebug() << "Stream opened on " << QString::fromStdString(url);
            }

            while(capture->isOpened() && runConcurrent){
                capture->read(frame);
                if(frame.empty() || frame.rows==0 || frame.cols==0)
                    break;
                if(releaseRequest){
                    releaseRequest = false;
                    capture->release();
                    if(TELLO_DEBUG_OUTPUT)
                        qDebug() << "Stream released";
                }
                emit newFrame(mat2pixmap(frame));
            }
        }
    });
}

TelloStream::~TelloStream(){
    runConcurrent = false;
    future.waitForFinished();
    delete capture;
}

void TelloStream::enableStream(){
    if(!capture->isOpened()){
        openRequest = true;
        if(TELLO_DEBUG_OUTPUT)
            qDebug() << "Stream open requested";
    }
    else{
        qDebug() << "Stream already opened";
        return;
    }
}

void TelloStream::disableStream(){
    if(capture->isOpened()){
        releaseRequest = true;
        if(TELLO_DEBUG_OUTPUT)
            qDebug() << "Stream release requested";
    }
    else{
        qDebug() << "Stream not opened";
    }
}

QPixmap TelloStream::mat2pixmap(Mat bgr_img){
    Mat rgb_img;
    cvtColor(bgr_img, rgb_img, cv::COLOR_BGR2RGB);
    return QPixmap::fromImage(QImage(rgb_img.data, rgb_img.cols, rgb_img.rows, QImage::Format_RGB888));
}

QSize TelloStream::size2qsize(cv::Size s){
    return QSize(s.width, s.height);
}
