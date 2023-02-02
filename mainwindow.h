#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QMouseEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    /*void mousePressEvent(QMouseEvent *e);*/
    QPoint PositionSouris();

private:
    Ui::MainWindow *ui;

private slots:

};
#endif // MAINWINDOW_H
