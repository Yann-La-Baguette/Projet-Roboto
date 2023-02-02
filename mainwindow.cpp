#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

/*void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::RightButton)
    {
        qDebug() << "Right mouse click!" << Qt::endl;
    }
}*/

QPoint MainWindow::PositionSouris(){
    QPoint p = ui->widget->mapFromGlobal(QCursor::pos());
    ui->label->setText(QString::number(p.rx()));
    ui->label->setText(QString::number(p.ry()));

    return p;
}

void QWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {

    }
}
