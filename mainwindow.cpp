#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    ui->image->setStyleSheet("background-color: #FF0000;");

    QLabel *label = new QLabel(ui->image);
    QPixmap pixmap("./Image_satellite_test.png");

    label->setPixmap(pixmap);
    ui->image->setFixedSize(pixmap.width(), pixmap.height());
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(label);
    ui->image->setLayout(layout);
}

MainWindow::~MainWindow(){
    delete ui;
}


void MainWindow::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton) {
        QRect rect = ui->image->geometry();
        if (rect.contains(event->pos())) {

            QPoint p = ui->image->mapFromParent(event->pos());

            QImage Image = ui->image->grab().toImage();

            QPainter painter(&Image);
            QPen pen;
            pen.setWidth(15);
            pen.setColor(Qt::red);
            painter.setPen(pen);
            painter.drawPoint(p);
            painter.end();

            QLabel *label = new QLabel(ui->image);
            label->setPixmap(QPixmap::fromImage(Image));

            QHBoxLayout *layout = new QHBoxLayout();
            layout->addWidget(label);

            QLayout *oldLayout = ui->image->layout();
            if (oldLayout != nullptr) {
                delete oldLayout;
            }
            ui->image->setLayout(layout);

            ui->cooSouris->setText("x : " + QString::number(p.rx()) + "     y : " + QString::number(p.ry()));
        }
    }
}
