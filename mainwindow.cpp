#include "mainwindow.h"
#include "ui_mainwindow.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    tello = new Tello;
    gamepad = new GamepadManager;
    alphabot = new QWebSocket;

    // Connexion au robot
    QUrl url;
    url.setUrl("ws://172.21.28.101");
    url.setPort(8080);
    alphabot->open(url);

    // Connexion pour les données du drone
    connect(tello->tello_command,&TelloCommand::responseSignal,this,&MainWindow::updateCommandReponse);
    connect(tello->tello_state,&TelloState::dataAvailable,this,&MainWindow::updateGUI);
    connect(tello->tello_state,&TelloState::status,this,&MainWindow::updateConnectionStatus);
    connect(tello->tello_stream,&TelloStream::newFrame,this,&MainWindow::displayStream);

    // Démarrage liaison avec le drone
    tello->start();

    // Affichage des données du drone
    affichageCompass();
    affichageAttitude();
    affichageHeight();
    affichageWifi();

    //Affichage de logos pour les boutons du drone
    logosBoutons();

    // Détection manette
    connect(gamepad, &GamepadManager::gamepadButtonPressed, this, &MainWindow::onGamepadButtonPressed);
    connect(gamepad, &GamepadManager::gamepadJoystickChanged, this, &MainWindow::onGamepadJoystickChanged);

    // Affichage waypoints robot
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(loop()));
    timer->start((1/1000)*1000);
}
MainWindow::~MainWindow(){
    alphabot->close();
    delete ui;
    delete tello;
    delete gamepad;
    delete alphabot;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::affichageHeight(){
    mAirspeedGauge = new QcGaugeWidget;

    mAirspeedGauge->addArc(55);
    mAirspeedGauge->addDegrees(65)->setValueRange(0,100);
    QcColorBand *clrBand = mAirspeedGauge->addColorBand(50);
    clrBand->setValueRange(0,100);
    mAirspeedGauge->addValues(80)->setValueRange(0,1000);
    mAirspeedGauge->addLabel(70)->setText("Height (cm)");
    QcLabelItem *lab = mAirspeedGauge->addLabel(40);
    lab->setText("0");
    mAirspeedNeedle = mAirspeedGauge->addNeedle(60);
    mAirspeedNeedle->setLabel(lab);
    mAirspeedNeedle->setColor(Qt::blue);
    mAirspeedNeedle->setValueRange(0,1000);
    mAirspeedGauge->addBackground(7);
    ui->heightGauge->addWidget(mAirspeedGauge);
}
void MainWindow::affichageAttitude(){
    mAttitudeGauge = new QcGaugeWidget;
    mAttitudeGauge->addBackground(99);
    QcBackgroundItem *bkg = mAttitudeGauge->addBackground(92);
    bkg->clearrColors();
    bkg->addColor(0.1,Qt::black);
    bkg->addColor(1.0,Qt::white);
    mAttMeter = mAttitudeGauge->addAttitudeMeter(88);

    mAttitudeNeedle = mAttitudeGauge->addNeedle(70);
    mAttitudeNeedle->setMinDegree(0);
    mAttitudeNeedle->setMaxDegree(180);
    mAttitudeNeedle->setValueRange(0,180);
    mAttitudeNeedle->setCurrentValue(90);
    mAttitudeNeedle->setColor(Qt::white);
    mAttitudeNeedle->setNeedle(QcNeedleItem::AttitudeMeterNeedle);
    mAttitudeGauge->addGlass(80);
    ui->attitudeMeter->addWidget(mAttitudeGauge);
}
void MainWindow::affichageCompass(){
    mCompassGauge = new QcGaugeWidget;

    mCompassGauge->addBackground(99);
    QcBackgroundItem *bkg1 = mCompassGauge->addBackground(92);
    bkg1->clearrColors();
    bkg1->addColor(0.1,Qt::black);
    bkg1->addColor(1.0,Qt::white);

    QcBackgroundItem *bkg2 = mCompassGauge->addBackground(88);
    bkg2->clearrColors();
    bkg2->addColor(0.1,Qt::white);
    bkg2->addColor(1.0,Qt::black);

    QcLabelItem *w = mCompassGauge->addLabel(80);
    w->setText("");
    w->setAngle(0);
    w->setColor(Qt::white);

    QcLabelItem *n = mCompassGauge->addLabel(80);
    n->setText("Launch");
    n->setAngle(90);
    n->setColor(Qt::white);

    QcLabelItem *e = mCompassGauge->addLabel(80);
    e->setText("");
    e->setAngle(180);
    e->setColor(Qt::white);

    QcLabelItem *s = mCompassGauge->addLabel(80);
    s->setText("");
    s->setAngle(270);
    s->setColor(Qt::white);

    QcDegreesItem *deg = mCompassGauge->addDegrees(70);
    deg->setStep(5);
    deg->setMaxDegree(270);
    deg->setMinDegree(-75);
    deg->setColor(Qt::white);
    mCompassNeedle = mCompassGauge->addNeedle(60);
    mCompassNeedle->setNeedle(QcNeedleItem::CompassNeedle);
    mCompassNeedle->setValueRange(0,360);
    mCompassNeedle->setMaxDegree(360);
    mCompassNeedle->setMinDegree(0);
    mCompassGauge->addBackground(7);
    mCompassGauge->addGlass(88);
    ui->compass->addWidget(mCompassGauge);
    mCompassNeedle->setCurrentValue(90);
}
void MainWindow::affichageWifi(){
    QString imageName = "./images_wifi/wifi0.png"; // Nom de l'image à charger
    QPixmap image(imageName); // Chargement de l'image
    QPixmap scaledImage = image.scaled(QSize(300, 382), Qt::KeepAspectRatio);
    ui->wifiLogo->setPixmap(scaledImage); // Affichage de l'image dans le label
}
void MainWindow::logosBoutons(){
    QIcon ermergencyLogo("./images_boutons/Stop.png");
    ui->emergencyButton->setIcon(ermergencyLogo);
    ui->emergencyButton->setIconSize(QSize(50,50));
    ui->emergencyButton->setStyleSheet("background-color: white;");

    QIcon takeOffLogo("./images_boutons/take_off.png");
    ui->takeOffBtn->setIcon(takeOffLogo);
    ui->takeOffBtn->setIconSize(QSize(50,50));
    ui->takeOffBtn->setStyleSheet("background-color: white;");

    QIcon landLogo("./images_boutons/land.png");
    ui->landBtn->setIcon(landLogo);
    ui->landBtn->setIconSize(QSize(50,50));
    ui->landBtn->setStyleSheet("background-color: orange;");

    QIcon upLogo("./images_boutons/up.png");
    ui->upBtn->setIcon(upLogo);
    ui->upBtn->setIconSize(QSize(50,50));

    QIcon downLogo("./images_boutons/down.png");
    ui->downBtn->setIcon(downLogo);
    ui->downBtn->setIconSize(QSize(50,50));

    QIcon forwardLogo("./images_boutons/forward.png");
    ui->forwardBtn->setIcon(forwardLogo);
    ui->forwardBtn->setIconSize(QSize(50,50));

    QIcon backLogo("./images_boutons/back.png");
    ui->backBtn->setIcon(backLogo);
    ui->backBtn->setIconSize(QSize(50,50));

    QIcon rightLogo("./images_boutons/right.png");
    ui->rightBtn->setIcon(rightLogo);
    ui->rightBtn->setIconSize(QSize(50,50));

    QIcon leftLogo("./images_boutons/left.png");
    ui->leftBtn->setIcon(leftLogo);
    ui->leftBtn->setIconSize(QSize(50,50));

    QIcon tRightLogo("./images_boutons/tRight.png");
    ui->tRightBtn->setIcon(tRightLogo);
    ui->tRightBtn->setIconSize(QSize(50,50));

    QIcon tLeftLogo("./images_boutons/tLeft.png");
    ui->tLeftBtn->setIcon(tLeftLogo);
    ui->tLeftBtn->setIconSize(QSize(50,50));

    QIcon stopMoveLogo("./images_boutons/stopMove.png");
    ui->stopMoveBtn->setIcon(stopMoveLogo);
    ui->stopMoveBtn->setIconSize(QSize(50,50));



    QIcon resetWaypointsLogo("./images_boutons/reset_waypoints.png");
    ui->Reset->setIcon(resetWaypointsLogo);
    ui->Reset->setIconSize(QSize(50,50));
    ui->Reset->setStyleSheet("background-color: white;");

    QIcon captureLogo("./images_boutons/capture.png");
    ui->captureBtn->setIcon(captureLogo);
    ui->captureBtn->setIconSize(QSize(50,50));

    QIcon delLastWaypointLogo("./images_boutons/delete_last_waypoint.png");
    ui->delLastWaypointBtn->setIcon(delLastWaypointLogo);
    ui->delLastWaypointBtn->setIconSize(QSize(50,50));

    QIcon launchRobotLogo("./images_boutons/go.png");
    ui->launchRobotBtn->setIcon(launchRobotLogo);
    ui->launchRobotBtn->setIconSize(QSize(50,50));

    QIcon robotPosDefLogo("./images_boutons/robotPosition.png");
    ui->robotPosDefBtn->setIcon(robotPosDefLogo);
    ui->robotPosDefBtn->setIconSize(QSize(50,50));

    UIStyle();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_emergencyButton_clicked(){
    tello->tello_command->emergency();
    ui->emergencyButton->setStyleSheet("background-color: red;");
    ui->landBtn->setStyleSheet("background-color: orange");
    ui->takeOffBtn->setStyleSheet("background-color: white");
    ui->emergencyButton->setText("Emergency Stop \nUse take off to reset \n(Escape)");
}
void MainWindow::on_takeOffBtn_clicked(){
    tello->tello_command->takeoff();
    ui->landBtn->setStyleSheet("background-color: white;");
    ui->emergencyButton->setStyleSheet("background-color: white");
    ui->takeOffBtn->setStyleSheet("background-color: orange;");
    ui->emergencyButton->setText("Emergency Stop \n(Escape)");
}
void MainWindow::on_landBtn_clicked(){
    tello->tello_command->land();
    ui->takeOffBtn->setStyleSheet("background-color: white;");
    ui->landBtn->setStyleSheet("background-color: orange;");
}
void MainWindow::on_upBtn_clicked(){
    tello->tello_command->setPosition(0,0,40,0);
    UIStyle();
    ui->upBtn->setStyleSheet("background-color: lightblue;");
}
void MainWindow::on_downBtn_clicked(){
    tello->tello_command->setPosition(0,0,-40,0);
    UIStyle();
    ui->downBtn->setStyleSheet("background-color: lightblue;");
}
void MainWindow::on_forwardBtn_clicked(){
    tello->tello_command->setPosition(0,80,0,0);
    UIStyle();
    ui->forwardBtn->setStyleSheet("background-color: lightblue;");
}
void MainWindow::on_backBtn_clicked(){
    tello->tello_command->setPosition(0,-50,0,0);
    UIStyle();
    ui->backBtn->setStyleSheet("background-color: lightblue;");
}
void MainWindow::on_rightBtn_clicked(){
    tello->tello_command->setPosition(50,0,0,0);
    UIStyle();
    ui->rightBtn->setStyleSheet("background-color: lightblue;");
}
void MainWindow::on_leftBtn_clicked(){
    tello->tello_command->setPosition(-50,0,0,0);
    UIStyle();
    ui->leftBtn->setStyleSheet("background-color: lightblue;");
}
void MainWindow::on_tRightBtn_clicked(){
    tello->tello_command->setPosition(0,0,0,70);
    UIStyle();
    ui->tRightBtn->setStyleSheet("background-color: lightblue;");
}
void MainWindow::on_tLeftBtn_clicked(){
    tello->tello_command->setPosition(0,0,0,-70);
    UIStyle();
    ui->tLeftBtn->setStyleSheet("background-color: lightblue;");
}
void MainWindow::on_stopMoveBtn_clicked(){
    tello->tello_command->setPosition(0,0,0,0);
    UIStyle();
    ui->stopMoveBtn->setStyleSheet("background-color: lightblue;");
}

void MainWindow::on_controllerCheckBox_stateChanged(int state){
    useController = state;
}
void MainWindow::onGamepadButtonPressed(int gamepadButton){
    if(useController == true){
        switch (gamepadButton) {


        // Drone Control
        case 12: // A
            ui->takeOffBtn->animateClick();
            break;
        case 13: // B
            ui->landBtn->animateClick();
            break;
        case 15: // Y
            ui->emergencyButton->animateClick();
            break;


        // Camera & robot control
        case 9: // Right click
            ui->captureBtn->animateClick();
            break;
        case 1: // Down
            ui->Reset->animateClick();
            break;
        case 3: // Right
            ui->delLastWaypointBtn->animateClick();
            break;
        case 2: // Left
            ui->robotPosDefBtn->animateClick();
            break;
        case 8: // Left click
            ui->launchRobotBtn->animateClick();
            break;

        default:
            break;
        }
    }
}
void MainWindow::onGamepadJoystickChanged(short sThumbLX, short sThumbLY, short sThumbRX, short  sThumbRY, short leftTrigger, short rightTrigger){
    if(useController == true){
        int highValue;
        if(leftTrigger > 0){
            highValue = -leftTrigger;
        }
        else{
            highValue = rightTrigger;
        }
        tello->tello_command->setPosition((sThumbLX*100/32768),(sThumbLY*100/32768),(highValue*100/255),(sThumbRX*100/32768));
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event){
    switch (event->key()) {
            case Qt::Key_Escape:
                ui->emergencyButton->animateClick();
                break;
            case Qt::Key_PageUp:
                ui->takeOffBtn->animateClick();
                break;
            case Qt::Key_PageDown:
                ui->landBtn->animateClick();
                break;
            case Qt::Key_Z:
                ui->forwardBtn->animateClick();
                break;
            case Qt::Key_Q:
                ui->leftBtn->animateClick();
                break;
            case Qt::Key_S:
                ui->backBtn->animateClick();
                break;
            case Qt::Key_D:
                ui->rightBtn->animateClick();
                break;
            case Qt::Key_A:
                ui->tLeftBtn->animateClick();
                break;
            case Qt::Key_E:
                ui->tRightBtn->animateClick();
                break;
            case Qt::Key_W:
                ui->upBtn->animateClick();
                break;
            case Qt::Key_X:
                ui->downBtn->animateClick();
                break;
            case Qt::Key_F:
                ui->stopMoveBtn->animateClick();
                break;

            case Qt::Key_C:
                ui->captureBtn->animateClick();
                break;
            case Qt::Key_R:
                ui->Reset->animateClick();
                break;
            case Qt::Key_T:
                ui->delLastWaypointBtn->animateClick();
                break;
            case Qt::Key_Y:
                ui->robotPosDefBtn->animateClick();
                break;
            case Qt::Key_U:
                ui->launchRobotBtn->animateClick();
                break;

            default:
                QWidget::keyPressEvent(event);
                break;
        }
}
void MainWindow::UIStyle(){
    QString Stylesheet = "background-color: white;";

    ui->stopMoveBtn->setStyleSheet(Stylesheet);
    ui->backBtn->setStyleSheet(Stylesheet);
    ui->downBtn->setStyleSheet(Stylesheet);
    ui->forwardBtn->setStyleSheet(Stylesheet);
    ui->leftBtn->setStyleSheet(Stylesheet);
    ui->tLeftBtn->setStyleSheet(Stylesheet);
    ui->rightBtn->setStyleSheet(Stylesheet);
    ui->tRightBtn->setStyleSheet(Stylesheet);
    ui->upBtn->setStyleSheet(Stylesheet);
    ui->stopMoveBtn->setStyleSheet(Stylesheet);
    ui->captureBtn->setStyleSheet(Stylesheet);
    ui->Reset->setStyleSheet(Stylesheet);
    ui->delLastWaypointBtn->setStyleSheet(Stylesheet);
    ui->launchRobotBtn->setStyleSheet(Stylesheet);
    ui->robotPosDefBtn->setStyleSheet("background-color: green;");

    ui->dronePicture->setStyleSheet("QLabel { background-color : black; color : white; }");
    ui->video->setStyleSheet("QLabel { background-color : black; color : white; }");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::displayStream(QPixmap videoPix){
    ui->video->setFixedSize(840,630);
    ui->video->setPixmap(videoPix);

    if(showPic == true){
        ui->dronePicture->setFixedSize(840, 630);
        savePixmap = videoPix;

        ui->dronePicture->setPixmap(savePixmap);

        reset();
        showPic = false;
    }
}
void MainWindow::on_captureBtn_clicked(){
    showPic = true;
}

void MainWindow::mousePressEvent(QMouseEvent *event){
    QPoint cursor = event->pos();

    QPoint imgLabel_pos = ui->dronePicture->pos();

    if (event->button() == Qt::LeftButton) {
        if(activeVector == false){
            if( cursor.x() >= imgLabel_pos.x() && cursor.x() <= (imgLabel_pos.x() + ui->dronePicture->width()) ){
                if( cursor.y() >= imgLabel_pos.y() && cursor.y() <= (imgLabel_pos.y() + ui->dronePicture->height()) ){
                    QPoint relativePos;

                    relativePos.setX(cursor.x() - imgLabel_pos.x());
                    relativePos.setY(cursor.y() - imgLabel_pos.y());

                    points.append(relativePos);

                    valeurDispo = true;
                }
            }
        }
        else{
            start = cursor;
        }
    }
}
void MainWindow::mouseReleaseEvent(QMouseEvent *event){
    if(activeVector == true){
        release = event->pos();

        int directionVector[2] = {start.x()-release.x(), start.y()-release.y()};

        valeurDispo = true;
        activeVector = false;
        ui->robotPosDefBtn->setStyleSheet("background-color: white;");
    }
}
void MainWindow::on_robotPosDefBtn_clicked(){
    if(activeVector == false){
        activeVector = true;
        ui->robotPosDefBtn->setStyleSheet("background-color: green;");
    }
    else{
        activeVector = false;
        ui->robotPosDefBtn->setStyleSheet("background-color: white;");
    }
}
void MainWindow::loop(){
    if(valeurDispo==true){

        double correction = 2/1.75;

        QPixmap new_img = savePixmap;
        ui->dronePicture->clear();

        QPainter painter(&new_img);
        QPen pen;
        pen.setWidth(4);
        pen.setColor(Qt::red);
        painter.setPen(pen);

        painter.drawLine(start*correction, release*correction);


        for(int i = 0 ; i<points.count() ; i++){
            QPoint point = points[i]*correction;
            painter.drawLine(point.x()-5, point.y(), point.x()+5, point.y());
            painter.drawLine(point.x(), point.y()-5, point.x(), point.y()+5);
            ui->cooSouris->setText("Last Waypoint Coordinates : \nX : " + QString::number(point.x()) + "\nY : " + QString::number(point.y()));
        }

        painter.end();
        ui->dronePicture->setPixmap(new_img);

        valeurDispo = false;
    }
}
void MainWindow::on_delLastWaypointBtn_clicked(){
    if(points.isEmpty() == false){
        valeurDispo = true;
        points.removeLast();
        ui->cooSouris->setText("Last Waypoint Coordinates : \nX : \nY : ");
    }
}
void MainWindow::reset(){
    points.clear();
    valeurDispo = true;
    ui->cooSouris->setText("Last Waypoint Coordinates : \nX : \nY : ");
}

void MainWindow::on_launchRobotBtn_clicked(){

    alphabot->sendTextMessage("av");
    qDebug()<<"1";

    QTimer::singleShot(5000, [=]() {
        alphabot->sendTextMessage("ar");
        qDebug()<<"2";
    });
    QTimer::singleShot(1000+500, [=]() {
        alphabot->sendTextMessage("stop");
        qDebug()<<"3";
    });
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::updateGUI(){
    int wifiValue = 0;
    int SNR = 90;

    if (SNR>=90){
        wifiValue = 4;
    } else if(SNR>=70){
        wifiValue = 3;
    } else if(SNR>=50){
        wifiValue = 2;
    } else if(SNR>=30){
        wifiValue = 1;
    } else{
        wifiValue = 0;
    }

    QString imageName = "./images_wifi/wifi" + QString::number(wifiValue) + ".png"; // Nom de l'image à charger
    QPixmap image(imageName); // Chargement de l'image
    QPixmap scaledImage = image.scaled(ui->wifiLogo->size());
    ui->wifiLogo->setPixmap(scaledImage); // Affichage de l'image dans le label

    ui->batteryPercentage->setValue(tello->tello_state->getBattery());

    mAttMeter->setCurrentPitch(tello->tello_state->getPitch());
    mAttMeter->setCurrentRoll(tello->tello_state->getRoll());

    int valueCompass;
    if(tello->tello_state->getYaw() + 270 >= 360){
        valueCompass = tello->tello_state->getYaw()-90;
    } else{
        valueCompass = tello->tello_state->getYaw()+270;
    }
    mCompassNeedle->setCurrentValue(valueCompass);

    mAirspeedNeedle->setCurrentValue(tello->tello_state->getHeight());
}
void MainWindow::updateConnectionStatus(TelloStatus alertSignal){
    QPixmap image("./images_wifi/wifi0.png");
    QPixmap scaledImage = image.scaled(QSize(300, 382), Qt::KeepAspectRatio);
    switch(alertSignal){
        case TelloStatus::TELLO_CONNECTED:{
            ui->statusLabel->setText("Status : Connected");
            ui->statusLabel->setStyleSheet("background-color: green;");
            break;
        }
        case TelloStatus::TELLO_DISCONNECTED:{
            ui->statusLabel->setText("Status : Disconnected");
            ui->statusLabel->setStyleSheet("background-color: red;");

            ui->batteryPercentage->setValue(0);
            ui->wifiLogo->setPixmap(scaledImage);
            mAttMeter->setCurrentPitch(0);
            mAttMeter->setCurrentRoll(0);
            mCompassNeedle->setCurrentValue(90);
            mAirspeedNeedle->setCurrentValue(0);
            break;
        }
        default:
            ui->statusLabel->setText("Status : Error");
            ui->statusLabel->setStyleSheet("background-color: red;");

            ui->batteryPercentage->setValue(0);
            ui->wifiLogo->setPixmap(scaledImage);
            mAttMeter->setCurrentPitch(0);
            mAttMeter->setCurrentRoll(0);
            mCompassNeedle->setCurrentValue(90);
            mAirspeedNeedle->setCurrentValue(0);
            break;
    }
}
void MainWindow::updateCommandReponse(TelloResponse response, QString datagram){
    ui->lineEdit_cmd_reponse->setReadOnly(false);
    ui->lineEdit_cmd_reponse->setText(tello->tello_command->getLastCommandUsed() + " -> " + datagram);
    ui->lineEdit_cmd_reponse->setReadOnly(true);
}
