#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    ui->image->setStyleSheet("background-color: #FF0000;");
    QLabel *label = new QLabel(ui->image);
    QPixmap pixmap("./Image_satellite_test.png");
    if (pixmap.isNull()) {
        qDebug() << "Could not load image";
    } else {
        label->setPixmap(pixmap);
        QHBoxLayout *layout = new QHBoxLayout();
        layout->addWidget(label);
        ui->image->setLayout(layout);
    }
}

MainWindow::~MainWindow(){
    delete ui;
}


void MainWindow::PositionSouris(){
    QPoint p = ui->cooSouris->mapFromGlobal(QCursor::pos());
    ui->cooSouris->setText("x : " + QString::number(p.rx()) + "     y : " + QString::number(p.ry()));
    qDebug()<<p.rx();
    qDebug()<<p.ry();
}

void MainWindow::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton) {
        QRect rect = ui->image->geometry();
        if (rect.contains(event->pos())) {
            QPoint p = ui->image->mapFromParent(event->pos());
            ui->cooSouris->setText("x : " + QString::number(p.rx()) + "     y : " + QString::number(p.ry()));
            qDebug()<<p.rx();
            qDebug()<<p.ry();
        }


    }
}
