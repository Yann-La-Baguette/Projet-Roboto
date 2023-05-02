#include "mainwindow.h"
#include "ui_mainwindow.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);


    // Gestion drone Tello

    tello = new Tello;

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


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Gestion Manette

    gamepad = new GamepadManager;

    // Détection manette
    connect(gamepad, &GamepadManager::gamepadButtonPressed, this, &MainWindow::onGamepadButtonPressed);
    connect(gamepad, &GamepadManager::gamepadJoystickChanged, this, &MainWindow::onGamepadJoystickChanged);
    connect(gamepad, &GamepadManager::gamepadConnected, this, &MainWindow::gamepadStatus);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Gestion du robot Alphabot

    alphabot = new QWebSocket;

    // Connexion a l'alphabot
    QUrl url;
    url.setUrl("ws://172.21.28.78");
    url.setPort(8080);
    alphabot->open(url);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Gestion de l'interface

    // Affichage waypoints robot
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(loop()));
    timer->start((1/1000)*1000);

    ui->gamepadStatusLabel->setVisible(false);
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
    tello->tello_command->setPosition(0,0,telloSpeed,0);
    UIStyle();
    ui->upBtn->setStyleSheet("background-color: lightblue;");
}
void MainWindow::on_downBtn_clicked(){
    tello->tello_command->setPosition(0,0,-telloSpeed,0);
    UIStyle();
    ui->downBtn->setStyleSheet("background-color: lightblue;");
}
void MainWindow::on_forwardBtn_clicked(){
    tello->tello_command->setPosition(0,telloSpeed,0,0);
    UIStyle();
    ui->forwardBtn->setStyleSheet("background-color: lightblue;");
}
void MainWindow::on_backBtn_clicked(){
    tello->tello_command->setPosition(0,-telloSpeed,0,0);
    UIStyle();
    ui->backBtn->setStyleSheet("background-color: lightblue;");
}
void MainWindow::on_rightBtn_clicked(){
    tello->tello_command->setPosition(telloSpeed,0,0,0);
    UIStyle();
    ui->rightBtn->setStyleSheet("background-color: lightblue;");
}
void MainWindow::on_leftBtn_clicked(){
    tello->tello_command->setPosition(-telloSpeed,0,0,0);
    UIStyle();
    ui->leftBtn->setStyleSheet("background-color: lightblue;");
}
void MainWindow::on_tRightBtn_clicked(){
    tello->tello_command->setPosition(0,0,0,telloSpeed);
    UIStyle();
    ui->tRightBtn->setStyleSheet("background-color: lightblue;");
}
void MainWindow::on_tLeftBtn_clicked(){
    tello->tello_command->setPosition(0,0,0,-telloSpeed);
    UIStyle();
    ui->tLeftBtn->setStyleSheet("background-color: lightblue;");
}
void MainWindow::on_flipBtn_clicked(){
    tello->tello_command->flip();
}
void MainWindow::on_stopMoveBtn_clicked(){
    tello->tello_command->setPosition(0,0,0,0);
    UIStyle();
    ui->stopMoveBtn->setStyleSheet("background-color: lightblue;");
}

void MainWindow::on_speedSlider_valueChanged(int value)
{
    telloSpeed = value;
    ui->speedLabel->setText(QString::number(value));
}

void MainWindow::onGamepadButtonPressed(int gamepadButton){
    qDebug()<<gamepadButton;
    switch (gamepadButton) {


    // Drone Control
    case 12: // A
        ui->launchRobotBtn->animateClick();
        break;
    case 13: // B
        ui->captureBtn->animateClick();
        break;
    case 14: // X
        ui->Reset->animateClick();
        break;
    case 15: // Y
        ui->delLastWaypointBtn->animateClick();
        break;

    case 9: // Right click
        ui->takeOffBtn->animateClick();
        break;
    case 8: // Left click
        ui->landBtn->animateClick();
        break;

    case 1: // Down
        ui->robotPosDefBtn->animateClick();
        break;
    case 3: // Right
        break;
    case 2: // Left
        break;

    case 4: // Start
        ui->emergencyButton->animateClick();
        break;
    case 5: // Select
        ui->flipBtn->animateClick();
        break;

    default:
        break;
    }
}
void MainWindow::onGamepadJoystickChanged(short sThumbLX, short sThumbLY, short sThumbRX, short  sThumbRY, short leftTrigger, short rightTrigger){
        int highValue;

        if(leftTrigger > 0){
            highValue = -leftTrigger;
        }
        else{
            highValue = rightTrigger;
        }

        if(SavesThumbLX != sThumbLX || SavesThumbLY != sThumbLY || SavesThumbRX != sThumbRX || SavesThumbRY != sThumbRY || SavehighValue != highValue){
            sameDatas = false;
        }
        else{
            sameDatas = true;
        }

        if(sameDatas == false){
            tello->tello_command->setPosition((sThumbLX*100/32768),(sThumbLY*100/32768),(highValue*100/255),(sThumbRX*100/32768));
            qDebug()<<"oui";
        }
        SavesThumbLX = sThumbLX;
        SavesThumbLY = sThumbLY;
        SavesThumbRX = sThumbRX;
        SavesThumbRY = sThumbRY;
        SavehighValue = highValue;
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
        case Qt::Key_B:
            ui->flipBtn->animateClick();
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

        case Qt::Key_M:
            ui->speedSlider->setValue(ui->speedSlider->value()-2);
            break;
        case Qt::Key_P:
            ui->speedSlider->setValue(ui->speedSlider->value()+2);
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
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::displayStream(QPixmap videoPix){
    ui->video->setFixedSize(840,630);
    ui->video->setPixmap(videoPix);

    if(showPic == true){
        ui->dronePicture->setFixedSize(840, 630);
        savePixmap = videoPix;

        ui->dronePicture->setPixmap(savePixmap);



        QString path = "./dronePictures";
        QDir dir(path);
        dir.setFilter( QDir::AllEntries | QDir::NoDotAndDotDot );
        int increment = dir.count() + 1;

        QFile file("./dronePictures/image" + QString::number(increment) + ".png");
        file.open(QIODevice::WriteOnly);
        savePixmap.save(&file, "PNG");




        reset();
        showPic = false;
    }
}
void MainWindow::on_captureBtn_clicked(){
    showPic = true;
    captureHeight = tello->tello_state->getHeight();
}

void MainWindow::mousePressEvent(QMouseEvent *event){
    if(ui->dronePicture->pixmap().isNull() == 0){
        if (event->button() == Qt::LeftButton) {
            QPoint cursor = event->pos();
            QPoint imgLabel_pos = ui->dronePicture->pos();
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
}
void MainWindow::mouseReleaseEvent(QMouseEvent *event){
    if(ui->dronePicture->pixmap().isNull() == 0){
        QPoint cursor = event->pos();
        QPoint imgLabel_pos = ui->dronePicture->pos();
        if( cursor.x() >= imgLabel_pos.x() && cursor.x() <= (imgLabel_pos.x() + ui->dronePicture->width()) ){
            if( cursor.y() >= imgLabel_pos.y() && cursor.y() <= (imgLabel_pos.y() + ui->dronePicture->height()) ){
                if(activeVector == true){
                    release = event->pos();

                    robotDirectionVector[0] = release.x() - start.x();
                    robotDirectionVector[1] = release.y() - start.y();

                    valeurDispo = true;
                    on_robotPosDefBtn_clicked();
                }
            }
        }
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
    if(valeurDispo==true && ui->dronePicture->pixmap().isNull() == 0){
        double correction = 2/1.75;
        QPoint startCorrected = start*correction;
        QPoint releaseCorrected = release*correction;


        QPixmap new_img = savePixmap;
        ui->dronePicture->clear();

        QPainter painter(&new_img);
        QPen pen;
        pen.setWidth(4);
        pen.setColor(Qt::red);
        painter.setPen(pen);

        painter.drawLine(startCorrected, releaseCorrected);


        for(int i = 0 ; i<points.count() ; i++){
            qDebug()<<points[i];
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
    double correction = 2/1.75;
    double Ratio_distance = 3.636363636; // m/s
    double Ratio_angle = 0.00875; // °/s
    int goalVector[2] = {0,0};

    double numerator = 0;
    double denominator = 0;
    double Angle = 0;
    double AngleDirect = 0;

    double ScreenDistance = 0;
    double ObjectiveDistance = 0;

    for(int i = 0 ; i<points.count() ; i++){
        if(i==0){
            goalVector[0] = abs(points[0].x()*correction - start.x()*correction);
            goalVector[1] = abs(points[0].y()*correction - start.y()*correction);
        }
        else{
            goalVector[0] = abs(points[i].x()*correction - points[i-1].x()*correction);
            goalVector[1] = abs(points[i].y()*correction - points[i-1].y()*correction);
        }

        numerator = robotDirectionVector[0]*goalVector[0] + robotDirectionVector[1]*goalVector[1];
        denominator = qSqrt(pow(robotDirectionVector[0], 2) + pow(robotDirectionVector[1], 2)) * qSqrt(pow(goalVector[0], 2) + pow(goalVector[1], 2));
        Angle = qAcos(numerator / denominator);
        Angle = qRadiansToDegrees(Angle);
        qDebug() << "Angle : " << Angle;

        ScreenDistance = qSqrt(pow(goalVector[0], 2)+ pow(goalVector[1], 2));
        ObjectiveDistance = (ScreenDistance * ((960/(2*(tan(41.3)*captureHeight)))/100))/100;
        qDebug() << "Screen Distance : " << ScreenDistance;
        qDebug() << "Objective Distance : " << ObjectiveDistance;

        /*Angle = 270;
        ObjectiveDistance = 1;

        qDebug() << "Angle : " << Angle;
        qDebug() << "Objective Distance : " << ObjectiveDistance;

        qDebug() << "DistanceRatio : " << Ratio_distance*ObjectiveDistance*100;
        qDebug() << "AngleRatio : " << Ratio_angle*Angle;*/

        if (Angle <180){
            alphabot->sendTextMessage("dr");
            AngleDirect = Angle;
        }
        else{
            AngleDirect = 360 - Angle;
            alphabot->sendTextMessage("ga");
        }
        QTimer::singleShot(int(Ratio_angle*AngleDirect*1000), [=]() {
            alphabot->sendTextMessage("av");
        });
        QTimer::singleShot(int((Ratio_angle*AngleDirect+Ratio_distance*ObjectiveDistance)*1000), [=]() {
            alphabot->sendTextMessage("stop");
        });


        robotDirectionVector[0] = goalVector[0];
        robotDirectionVector[1] = goalVector[1];
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::updateGUI(){

    // Wifi

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

    // Battery

    ui->batteryPercentage->setValue(tello->tello_state->getBattery());

    // Pitch and roll

    mAttMeter->setCurrentPitch(tello->tello_state->getPitch());
    mAttMeter->setCurrentRoll(tello->tello_state->getRoll());

    // Compass

    int valueCompass;
    if(tello->tello_state->getYaw() + 270 >= 360){
        valueCompass = tello->tello_state->getYaw()-90;
    } else{
        valueCompass = tello->tello_state->getYaw()+270;
    }
    mCompassNeedle->setCurrentValue(valueCompass);

    // Height

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

void MainWindow::gamepadStatus(bool gamepadConnectionStatus){
    ui->gamepadStatusLabel->setVisible(gamepadConnectionStatus);
}
