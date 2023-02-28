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
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv.hpp>
#include <tello.h>

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

    /**
     * @brief logosBoutons
     */
    void logosBoutons();

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

    Tello tello;
    bool connectBtnClicked = false;

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

    void on_upBtn_clicked();
    void on_downBtn_clicked();
    void on_rightBtn_clicked();
    void on_tRightBtn_clicked();
    void on_backBtn_clicked();
    void on_forwardBtn_clicked();
    void on_leftBtn_clicked();
    void on_tLeftBtn_clicked();
    void on_emergencyButton_clicked();
    void on_takeOffBtn_clicked();
    void on_landBtn_clicked();
    void on_connectBtn_clicked();

    //GUI DATA
    void updateGUI();
    void updateConnectionStatus();
    void updateCommandReponse();
    void on_stopMoveBtn_clicked();
};
#endif // MAINWINDOW_H
