#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QHBoxLayout>
#include <QTimer>
#include "qcgaugewidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QVector<QPoint> points;
    bool valeurDispo = true;

    QcGaugeWidget *mAirspeedGauge;
    QcNeedleItem *mAirspeedNeedle;

    QcGaugeWidget * mAttitudeGauge;
    QcNeedleItem * mAttitudeNeedle;
    QcAttitudeMeter *mAttMeter;

    QcGaugeWidget * mCompassGauge;
    QcNeedleItem *mCompassNeedle;

private slots:
    void loop();
    void mousePressEvent(QMouseEvent *event);
    void reset();
    void on_verticalSlider_valueChanged(int value);
    void on_verticalSlider_2_valueChanged(int value);
    void on_horizontalSlider_valueChanged(int value);
    void on_horizontalSlider_2_valueChanged(int value);
};
#endif // MAINWINDOW_H
