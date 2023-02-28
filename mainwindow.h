#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QHBoxLayout>
#include <QTimer>
#include <QVBoxLayout>
#include <QFileDialog>
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

    /**
     * @brief Affichage de la boussole
     */
    void affichageCompass();

    /**
     * @brief Affichage des valeurs de Pitch et Roll
     */
    void affichageAttitude();

    /**
     * @brief Affichage de la hauteur
     */
    void affichageHeight();

    /**
     * @brief Affichage de la qualite du signal Wi-Fi
     */
    void affichageWifi();

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


    bool forwardKeyPressed;
    bool backKeyPressed;
    bool leftKeyPressed;
    bool rightKeyPressed;
    bool tLeftKeyPressed;
    bool tRightKeyPressed;
    bool upKeyPressed;
    bool downKeyPressed;

private slots:

    /**
     * @brief Affichage de points pour le robot sur l'image
     */
    void loop();

    /**
     * @brief Récupération coordonnées des points lors de clic gauche
     * @param event
     */
    void mousePressEvent(QMouseEvent *event);

    /**
     * @brief Récupération des touches du clavier pressées
     * @param event
     */
    void keyPressEvent(QKeyEvent *event);

    /**
     * @brief Bouton pour réinitialiser les points
     */
    void reset();


    void on_verticalSlider_valueChanged(int value);
    void on_verticalSlider_2_valueChanged(int value);
    void on_horizontalSlider_valueChanged(int value);
    void on_horizontalSlider_2_valueChanged(int value);
    void on_horizontalSlider_3_valueChanged(int value);
    void on_upBtn_clicked();
    void on_downBtn_clicked();
    void on_rightBtn_clicked();
    void on_tRightBtn_clicked();
    void on_backBtn_clicked();
    void on_forwardBtn_clicked();
    void on_leftBtn_clicked();
    void on_tLeftBtn_clicked();
};
#endif // MAINWINDOW_H
