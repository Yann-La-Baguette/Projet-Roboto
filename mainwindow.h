#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QMouseEvent>
#include <QWidget>
#include <QHBoxLayout>

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
    void PositionSouris();


private:
    Ui::MainWindow *ui;
    QWidget *uwu;

private slots:
    void mousePressEvent(QMouseEvent *event);
};
#endif // MAINWINDOW_H
