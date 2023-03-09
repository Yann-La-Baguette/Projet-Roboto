#include "TelloStream.h"

TelloStream::TelloStream(QString a, quint16 p): address(a),port(p){
    url = "udp://@" + address.toStdString() + ":" + QString::number(port).toStdString();
    camera_enabled = false;
}

TelloStream::~TelloStream(){
    delete capture;
}

bool TelloStream::startVideoCapture(){
    capture = new VideoCapture(url);
    //capture = new VideoCapture(0);
    return capture->isOpened();
}

void TelloStream::stopStream(){
    capture->release();
}

void TelloStream::run(){

    while(!startVideoCapture()){
        qDebug() << "Camera waiting";
        camera_enabled = false;
        delete capture;
        QThread::sleep(5);
    }
    qDebug() << "Camera ok";
    camera_enabled = true;
    Mat frame;
    while(camera_enabled){
        capture->read(frame);

        if(frame.empty() || frame.rows==0 || frame.cols==0){
            camera_enabled = false;
            break;
        }

        size = size2qsize(frame.size());
        emit newFrame(mat2pixmap(frame));
    }

}

QPixmap TelloStream::mat2pixmap(Mat bgr_img){
    Mat rgb_img;
    //resize(bgr_img, rgb_img, Size(400,400));
    cvtColor(bgr_img, rgb_img, cv::COLOR_BGR2RGB);
    return QPixmap::fromImage(QImage(rgb_img.data, rgb_img.cols, rgb_img.rows, QImage::Format_RGB888));
}

QSize TelloStream::size2qsize(cv::Size s){
    return QSize(s.width, s.height);
}
