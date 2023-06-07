/// @file mainwindow.h
/// @author Yann Di Padova

#ifndef MAINWINDOW_H
#define MAINWINDOW_H


// Libraries
#include <QMainWindow>
#include <QMouseEvent>
#include <QTimer>
#include <QLabel>
#include <QWebSocket>
#include <QtMath>
#include <QDateTime>
#include <QInputDialog>
#include <QDesktopServices>
#include <QtConcurrent>
#include <unistd.h>
#include <QFuture>

// Personnal classes
#include "gamepadmanager.h"
#include "Tello/Tello.h"
#include "qcgaugewidget.h"
#include "database.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT



public:
    // Constructor & Destructor
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Showing gauges for drone datas
    /**
     * @brief Affichage de la hauteur
     */
    void affichageHeight();
    /**
     * @brief Affichage des valeurs de Pitch et Roll
     */
    void affichageAttitude();
    /**
     * @brief Affichage de la boussole
     */
    void affichageCompass();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Put all the move buttons to white
    /**
     * @brief Reset de touts les boutons
     */
    void UIStyle();



private:
    Ui::MainWindow *ui;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Class definitions
    Tello *tello;
    GamepadManager *gamepad;
    QWebSocket *alphabot;
    database *db;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Drone values
    QcGaugeWidget *mAirspeedGauge;
    QcNeedleItem *mAirspeedNeedle;

    QcGaugeWidget * mAttitudeGauge;
    QcNeedleItem * mAttitudeNeedle;
    QcAttitudeMeter *mAttMeter;

    QcGaugeWidget * mCompassGauge;
    QcNeedleItem *mCompassNeedle;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Waypoints and video definition
    QVector<QPoint> points;
    bool valeurDispo = false;
    QLabel *pointsLabel;
    bool showPic = false;
    QPixmap savePixmap;
    float aspectRatio;
    bool MirrorMode = false;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //Vectors calculations
    bool activeVector = true;
    QPoint start;
    QPoint release;
    int robotDirectionVector[2];
    int captureHeight;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //Gamepad controls
    short SavesThumbLX, SavesThumbLY, SavesThumbRX, SavesThumbRY, SavehighValue;
    bool sameDatas;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //Drone and robot controls
    bool controlDroneMode = true;
    QString RobotIPAdress = "172.21.28.78";
    int telloSpeed;


    int BatteryPercentage;
    double Pitch;
    double Roll;
    double Yaw;
    int Height;

private slots:

    // Control Buttons
    /**
     * @brief on_changeControlBtn_clicked Change the controls from drone to Robot
     */
    void on_changeControlBtn_clicked();
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
    /**
     * @brief When the stpo move button is clicked
     */
    void on_stopMoveBtn_clicked();
    /**
     * @brief When the capture button is clicked
     */
    void on_captureBtn_clicked();
    /**
     * @brief on_flipBtn_clicked
     */
    void on_flipBtn_clicked();

    /**
     * @brief on_speedSlider_valueChanged Change Tello Speed
     * @param value
     */
    void on_speedSlider_valueChanged(int value);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Shortcuts for control
    /**
     * @brief Get keyboard pressed keys
     * @param event
     */
    void keyPressEvent(QKeyEvent *event);
    /**
     * @brief When a button is pressed on the gamepad
     * @param gamepadButton
     */
    void onGamepadButtonPressed(int gamepadButton);
    /**
     * @brief When a value is changed on the joystick or on RT/LT
     * @param sThumbLX
     * @param sThumbLY
     * @param sThumbRX
     * @param sThumbRY
     * @param leftTrigger
     * @param rightTrigger
     */
    void onGamepadJoystickChanged(short sThumbLX, short sThumbLY, short sThumbRX, short  sThumbRY, short leftTrigger, short rightTrigger);
    /**
     * @brief on_launchRobotBtn_clicked
     */
    void on_launchRobotBtn_clicked();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Waypoints placing
    /**
     * @brief Get coordinates of points when left clic
     * @param event
     */
    void mousePressEvent(QMouseEvent *event);
    /**
     * @brief mouseReleaseEvent
     * @param event
     */
    void mouseReleaseEvent(QMouseEvent *event);
    /**
     * @brief Drawing waypoints for the robot on the picture
     */
    void loop();
    /**
     * @brief Delete the last waypoint
     */
    void on_delLastWaypointBtn_clicked();
    /**
     * @brief Reset waypoints
     */
    void reset();
    /**
     * @brief on_robotPosDefBtn_clicked
     */
    void on_robotPosDefBtn_clicked();
    /**
     * @brief on_mirrorModeBtn_clicked Mirror the video for drone mirror
     */
    void on_mirrorModeBtn_clicked();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // GUI Data
    void displayStream(QPixmap video);
    void updateGUI();
    void updateConnectionStatus(TelloStatus alertSignal);
    void updateCommandReponse(TelloResponse response, QString datagram);
    void gamepadStatus(bool gamepadConnectionStatus);
    void on_adminButton_clicked(bool checked);
    void on_robotIPBtn_clicked();

    void dataBaseSave();
    void on_captureDirBtn_clicked();


    void launchRobot(double Angle, double ObjectiveDistance);
};
#endif // MAINWINDOW_H
