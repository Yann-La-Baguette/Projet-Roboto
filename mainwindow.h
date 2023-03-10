#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QTimer>

#include "qlabel.h"
#include "Tello/tello.h"
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

    //Showing gauges for drone datas
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

    //Showing logos for the buttons
    /**
     * @brief Affichage de logos pour les boutons
     */
    void logosBoutons();

    //Put all the move buttons to white
    /**
     * @brief Reset de touts les boutons
     */
    void allMoveBtnWhite();

private:
    Ui::MainWindow *ui;
    QVector<QPoint> points;
    bool valeurDispo = true;
    QLabel *pointsLabel;


    QcGaugeWidget *mAirspeedGauge;
    QcNeedleItem *mAirspeedNeedle;

    QcGaugeWidget * mAttitudeGauge;
    QcNeedleItem * mAttitudeNeedle;
    QcAttitudeMeter *mAttMeter;

    QcGaugeWidget * mCompassGauge;
    QcNeedleItem *mCompassNeedle;

    bool showPic = false;
    QPixmap savePixmap;
    float aspectRatio;

    Tello *tello;

private slots:
    /**
     * @brief Drawing waypoints for the robot on the picture
     */
    void loop();
    /**
     * @brief Get coordinates of points when left clic
     * @param event
     */
    void mousePressEvent(QMouseEvent *event);
    /**
     * @brief Reset waypoints
     */
    void reset();


    /**
     * @brief Get keyboard pressed keys
     * @param event
     */
    void keyPressEvent(QKeyEvent *event);

    //Drone Control Buttons
    /**
     * @brief on_upBtn_clicked Go up
     */
    void on_upBtn_clicked();
    /**
     * @brief on_downBtn_clicked Go down
     */
    void on_downBtn_clicked();
    /**
     * @brief on_rightBtn_clicked Go to the right
     */
    void on_rightBtn_clicked();
    /**
     * @brief on_tRightBtn_clicked Turn to the right
     */
    void on_tRightBtn_clicked();
    /**
     * @brief on_backBtn_clicked Back off
     */
    void on_backBtn_clicked();
    /**
     * @brief on_forwardBtn_clicked Go ahead
     */
    void on_forwardBtn_clicked();
    /**
     * @brief on_leftBtn_clicked Go to the left
     */
    void on_leftBtn_clicked();
    /**
     * @brief on_tLeftBtn_clicked Turn to the left
     */
    void on_tLeftBtn_clicked();
    /**
     * @brief on_emergencyButton_clicked Stop motors
     */
    void on_emergencyButton_clicked();
    /**
     * @brief on_takeOffBtn_clicked Take off
     */
    void on_takeOffBtn_clicked();
    /**
     * @brief on_landBtn_clicked Land
     */
    void on_landBtn_clicked();

    //GUI DATA
    void updateGUI();
    void updateConnectionStatus(TelloAlerts alertSignal);
    void updateCommandReponse(TelloResponse response, QString datagram);
    void on_stopMoveBtn_clicked();

    void displayStream(QPixmap video);
    void on_captureBtn_clicked();
};
#endif // MAINWINDOW_H
