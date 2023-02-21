#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    ui->image->setStyleSheet("background-color: #FF0000;");

    setWindowTitle("Roboto Collaboratif");
    setWindowIcon(QIcon("./icone"));


    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(loop()));
    timer->start((1/1000)*1000);
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::loop(){

    if(valeurDispo==true){

    QImage Image("./Image_satellite_test.png");

    QPainter painter(&Image);
    QPen pen;
    pen.setWidth(10);
    pen.setColor(Qt::blue);
    painter.setPen(pen);
    for(int i = 0 ; i<points.count() ; i++){
        QPoint point = points[i];
        painter.drawPoint(point);
    }
    painter.end();

    QLabel *label = new QLabel(ui->image);
    label->setPixmap(QPixmap::fromImage(Image));

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(label);

    delete ui->image->layout();
    ui->image->setFixedSize(Image.width(), Image.height());
    ui->image->setLayout(layout);
    valeurDispo = false;
    }
}


void MainWindow::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton) {
        QRect rect = ui->image->geometry();
        if (rect.contains(event->pos())){
            QPoint p = ui->image->mapFromParent(event->pos());
            points.append(p);

            ui->cooSouris->setText("Coordonnées : \nx : " + QString::number(p.rx()) + "\ny : " + QString::number(p.ry()));
            valeurDispo = true;
        }
    }
}

void MainWindow::reset(){
    points.clear();
    valeurDispo = true;
}
