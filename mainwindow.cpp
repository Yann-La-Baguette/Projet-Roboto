#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    connect(&tello,SIGNAL(newStateDataAvailable()),this,SLOT(updateGUI()));
    connect(&tello,SIGNAL(connectionStatusChanged()),this,SLOT(updateConnectionStatus()));
    connect(&tello,SIGNAL(newCommandReponseAvailable()),this,SLOT(updateCommandReponse()));
    connect(&tello,SIGNAL(cameraStreamAvailable()),this,SLOT(openCameraStream()));

    ui->image->setStyleSheet("background-color: #7F7F7F;");

    // Affichage des données du drone
    affichageCompass();
    affichageAttitude();
    affichageHeight();
    affichageWifi();

    //Affichage de logos pour les boutons du drone
    logosBoutons();

    // Affichage waypoints robot
    QTimer *timer = new QTimer(this);
    pointsLabel = new QLabel(ui->image);
    connect(timer, SIGNAL(timeout()), this, SLOT(loop()));
    timer->start((1/1000)*1000);
}

MainWindow::~MainWindow(){
    delete ui;
}



void MainWindow::loop(){

    if(valeurDispo==true){

    QImage Image("./Image_satellite_test.png");

    Image = Image.scaled(ui->image->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QPainter painter(&Image);
    QPen pen;
    pen.setWidth(4);
    pen.setColor(Qt::red);
    painter.setPen(pen);
    for(int i = 0 ; i<points.count() ; i++){
        QPoint point = points[i];
        painter.drawLine(point.x()-5, point.y(), point.x()+5, point.y());
        painter.drawLine(point.x(), point.y()-5, point.x(), point.y()+5);
    }
    painter.end();

    pointsLabel->setPixmap(QPixmap::fromImage(Image));

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(pointsLabel);

    delete ui->image->layout();
    ui->image->setFixedSize(Image.width(), Image.height());
    ui->image->setLayout(layout);
    valeurDispo = false;
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton) {
        QPoint clickPos = ui->image->mapFrom(this, event->pos());
        if (ui->image->rect().contains(clickPos)) {
            clickPos.setX(clickPos.x()-8);
            clickPos.setY(clickPos.y());
            points.append(clickPos);
            ui->cooSouris->setText("Waypoints Coordinates : \nx : " + QString::number(clickPos.x()) + "\ny : " + QString::number(clickPos.y()));
            valeurDispo = true;
        }
    }
}

void MainWindow::reset(){
    points.clear();
    valeurDispo = true;
    ui->cooSouris->setText("Waypoints Coordinates : \nx : \ny : ");
}



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

    QIcon landLogo("./images_boutons/land.png");
    ui->landBtn->setIcon(landLogo);
    ui->landBtn->setIconSize(QSize(50,50));
    ui->landBtn->setStyleSheet("background-color: orange;");

    QIcon takeOffLogo("./images_boutons/take_off.png");
    ui->takeOffBtn->setIcon(takeOffLogo);
    ui->takeOffBtn->setIconSize(QSize(50,50));
    ui->takeOffBtn->setStyleSheet("background-color: white;");

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

    QIcon connectLogo("./images_boutons/connect.png");
    ui->connectBtn->setIcon(connectLogo);
    ui->connectBtn->setIconSize(QSize(30,30));
    ui->connectBtn->setStyleSheet("background-color: white;");

    QIcon resetWaypointsLogo("./images_boutons/reset_waypoints.png");
    ui->Reset->setIcon(resetWaypointsLogo);
    ui->Reset->setIconSize(QSize(50,50));
    ui->Reset->setStyleSheet("background-color: white;");

    allMoveBtnWhite();
}



void MainWindow::on_connectBtn_clicked(){
    if(connectBtnClicked == false){
        connectBtnClicked = true;
        tello.init(QHostAddress("192.168.10.1"), 8889, 8890, 11111);
    }
    else{
        connectBtnClicked = false;
        ui->connectBtn->setText("Connect \n(Enter)");
        tello.close();
    }
}

void MainWindow::on_upBtn_clicked(){
    tello.sendCommand_generic("rc 0 0 40 0");
    allMoveBtnWhite();
    ui->upBtn->setStyleSheet("background-color: lightblue;");
}

void MainWindow::on_downBtn_clicked(){
    tello.sendCommand_generic("rc 0 0 -40 0");
    allMoveBtnWhite();
    ui->downBtn->setStyleSheet("background-color: lightblue;");
}

void MainWindow::on_rightBtn_clicked(){
    tello.sendCommand_generic("rc 50 0 0");
    allMoveBtnWhite();
    ui->rightBtn->setStyleSheet("background-color: lightblue;");
}

void MainWindow::on_tRightBtn_clicked(){
    tello.sendCommand_generic("rc 0 0 0 70");
    allMoveBtnWhite();
    ui->tRightBtn->setStyleSheet("background-color: lightblue;");
}

void MainWindow::on_backBtn_clicked(){
    tello.sendCommand_generic("rc 0 -50 0 0");
    allMoveBtnWhite();
    ui->backBtn->setStyleSheet("background-color: lightblue;");
}

void MainWindow::on_forwardBtn_clicked(){
    tello.sendCommand_generic("rc 0 50 0 0");
    allMoveBtnWhite();
    ui->forwardBtn->setStyleSheet("background-color: lightblue;");
}

void MainWindow::on_leftBtn_clicked(){
    tello.sendCommand_generic("rc -50 0 0 0");
    allMoveBtnWhite();
    ui->leftBtn->setStyleSheet("background-color: lightblue;");
}

void MainWindow::on_tLeftBtn_clicked(){
    tello.sendCommand_generic("rc 0 0 0 -70");
    allMoveBtnWhite();
    ui->tLeftBtn->setStyleSheet("background-color: lightblue;");
}

void MainWindow::on_stopMoveBtn_clicked(){
    tello.sendCommand_generic("rc 0 0 0 0");
    allMoveBtnWhite();
    ui->stopMoveBtn->setStyleSheet("background-color: lightblue;");
}

void MainWindow::on_emergencyButton_clicked(){
    tello.sendCommand_generic("emergency");
    ui->emergencyButton->setStyleSheet("background-color: red;");
    ui->landBtn->setStyleSheet("background-color: orange");
    ui->takeOffBtn->setStyleSheet("background-color: white");
    ui->emergencyButton->setText("Emergency Stop \nUse take off to reset \n(Escape)");
}

void MainWindow::on_takeOffBtn_clicked(){
    tello.sendCommand_generic("takeoff");
    ui->landBtn->setStyleSheet("background-color: white;");
    ui->emergencyButton->setStyleSheet("background-color: white");
    ui->takeOffBtn->setStyleSheet("background-color: orange;");
    ui->emergencyButton->setText("Emergency Stop \n(Escape)");
}

void MainWindow::on_landBtn_clicked(){
    tello.sendCommand_generic("land");
    ui->takeOffBtn->setStyleSheet("background-color: white;");
    ui->landBtn->setStyleSheet("background-color: orange;");
}

void MainWindow::keyPressEvent(QKeyEvent *event){
    switch (event->key()) {
            case Qt::Key_Return:
                ui->connectBtn->animateClick();
                break;
            case Qt::Key_Z:
                ui->forwardBtn->animateClick();
                break;
            case Qt::Key_S:
                ui->backBtn->animateClick();
                break;
            case Qt::Key_Q:
                ui->leftBtn->animateClick();
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
            case Qt::Key_Escape:
                ui->emergencyButton->animateClick();
                break;
            case Qt::Key_PageUp:
                ui->takeOffBtn->animateClick();
                break;
            case Qt::Key_PageDown:
                ui->landBtn->animateClick();
                break;
            default:
                QWidget::keyPressEvent(event);
                break;
        }
}

void MainWindow::allMoveBtnWhite(){
    ui->stopMoveBtn->setStyleSheet("background-color: white;");
    ui->backBtn->setStyleSheet("background-color: white;");
    ui->downBtn->setStyleSheet("background-color: white;");
    ui->forwardBtn->setStyleSheet("background-color: white;");
    ui->leftBtn->setStyleSheet("background-color: white;");
    ui->tLeftBtn->setStyleSheet("background-color: white;");
    ui->rightBtn->setStyleSheet("background-color: white;");
    ui->tRightBtn->setStyleSheet("background-color: white;");
    ui->upBtn->setStyleSheet("background-color: white;");
    ui->stopMoveBtn->setStyleSheet("background-color: white;");
}



void MainWindow::updateGUI(){

    int wifiValue = 0;
    if (tello.getWifiSNR()>=90){
        wifiValue = 4;
    } else if(tello.getWifiSNR()>=70){
        wifiValue = 3;
    } else if(tello.getWifiSNR()>=50){
        wifiValue = 2;
    } else if(tello.getWifiSNR()>=30){
        wifiValue = 1;
    } else{
        wifiValue = 0;
    }

    QString imageName = "./images_wifi/wifi" + QString::number(wifiValue) + ".png"; // Nom de l'image à charger
    QPixmap image(imageName); // Chargement de l'image
    QPixmap scaledImage = image.scaled(ui->wifiLogo->size());
    ui->wifiLogo->setPixmap(scaledImage); // Affichage de l'image dans le label

    ui->batteryPercentage->setValue(tello.getBattery());

    mAttMeter->setCurrentPitch(tello.getPitch());
    mAttMeter->setCurrentRoll(tello.getRoll());

    int valueCompass;
    if(tello.getYaw() + 270 >= 360){
        valueCompass = tello.getYaw()-90;
    } else{
        valueCompass = tello.getYaw()+270;
    }
    mCompassNeedle->setCurrentValue(valueCompass);

    mAirspeedNeedle->setCurrentValue(tello.getHeight());
}

void MainWindow::updateConnectionStatus(){
    switch(tello.getStatus()){
        case Tello::Status::NO_RESPONSE:{
            ui->connectBtn->setText("Disconnect \nStatus : No Response \n(Enter)");
            ui->connectBtn->setStyleSheet("background-color: red;");
            ui->batteryPercentage->setValue(0);
            break;
        }
        case Tello::Status::NO_RESPONSE_TIMEOUT: {
            ui->connectBtn->setText("Disconnect \nStatus : No Response since 15s, Reboot \n(Enter)");
            QPixmap image("./images_wifi/wifi0.png");
            QPixmap scaledImage = image.scaled(QSize(300, 382), Qt::KeepAspectRatio);
            ui->wifiLogo->setPixmap(scaledImage); // Affichage de l'image dans le label
            ui->connectBtn->setStyleSheet("background-color: red;");
            ui->batteryPercentage->setValue(0);
            break;
        }
        case Tello::Status::CONNECTED:
            ui->connectBtn->setText("Disconnect \nStatus : Connected \n(Enter)");
            ui->connectBtn->setStyleSheet("background-color: green;");
            break;

        case Tello::Status::CLOSED:{
            ui->connectBtn->setStyleSheet("background-color: white;");
            QPixmap image("./images_wifi/wifi0.png");
            QPixmap scaledImage = image.scaled(QSize(300, 382), Qt::KeepAspectRatio);
            ui->wifiLogo->setPixmap(scaledImage); // Affichage de l'image dans le label
            break;
        }
        default:
            ui->connectBtn->setText("Disconnect \nStatus : Error \n(Enter)");
            ui->connectBtn->setStyleSheet("background-color: red;");
            break;
    }

}

void MainWindow::updateCommandReponse(){
    QString str = "Sending command : " + tello.getLastCommmandUsed() + "        Response : " + tello.getCommandResponseData();
    ui->lineEdit_cmd_reponse->setReadOnly(false);
    ui->lineEdit_cmd_reponse->setText(str);
    ui->lineEdit_cmd_reponse->setReadOnly(true);
}
