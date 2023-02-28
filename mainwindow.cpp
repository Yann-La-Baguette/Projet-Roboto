#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    connect(&tello,SIGNAL(newStateDataAvailable()),this,SLOT(updateGUI()));
    connect(&tello,SIGNAL(connectionStatusChanged()),this,SLOT(updateConnectionStatus()));
    connect(&tello,SIGNAL(newCommandReponseAvailable()),this,SLOT(updateCommandReponse()));
    connect(&tello,SIGNAL(cameraStreamAvailable()),this,SLOT(openCameraStream()));

    ui->image->setStyleSheet("background-color: #7F7F7F;");

    // Icone et nom d'application
    setWindowTitle("Roboto Collaboratif");
    setWindowIcon(QIcon("./icone"));

    // Affichage des données du drone
    affichageCompass();
    affichageAttitude();
    affichageHeight();
    affichageWifi();

    //Affichage de logos pour les boutons du drone
    logosBoutons();

    // Affichage waypoints robot
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(loop()));
    timer->start((1/1000)*1000);
}

MainWindow::~MainWindow(){
    delete ui;
}



void MainWindow::loop(){

    if(valeurDispo==true){

    QImage Image("./Image_satellite_test.png");

    QPainter painter(&Image);
    QPen pen;
    pen.setWidth(3);
    pen.setColor(Qt::red);
    painter.setPen(pen);
    for(int i = 0 ; i<points.count() ; i++){
        QPoint point = points[i];
        painter.drawLine(point.x()-5, point.y(), point.x()+5, point.y());
        painter.drawLine(point.x(), point.y()-5, point.x(), point.y()+5);
    }
    painter.end();

    QLabel *label = new QLabel(ui->image);
    label->setPixmap(QPixmap::fromImage(Image));

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(label);

    delete ui->image->layout();
    ui->image->setFixedSize(Image.width(), Image.height());
    ui->image->setLayout(layout);
    valeurDispo = false;
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton) {
        if(ui->tabWidget->currentIndex() == 0){
            QRect rect = ui->image->geometry();
            if (rect.contains(event->pos())){
                //QPoint p = ui->image->mapFrom(this, event->pos());
                QPoint globalPos = event->globalPos();
                QPoint imagePos = ui->image->mapFromGlobal(globalPos);
                imagePos.setX(imagePos.x()-10);
                points.append(imagePos);

                ui->cooSouris->setText("Coordonnées : \nx : " + QString::number(imagePos.rx()) + "\ny : " + QString::number(imagePos.ry()));
                valeurDispo = true;
            }
        }
    }
}

void MainWindow::reset(){
    points.clear();
    valeurDispo = true;
    ui->cooSouris->setText("Coordonnées : \nx : \ny : ");
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
    QString imageName = "./images_wifi/wifi4.png"; // Nom de l'image à charger
    QPixmap image(imageName); // Chargement de l'image
    QPixmap scaledImage = image.scaled(QSize(300, 382), Qt::KeepAspectRatio);
    ui->wifiLogo->setPixmap(scaledImage); // Affichage de l'image dans le label
}

void MainWindow::logosBoutons(){
    QIcon ermergencyLogo("./images_boutons/Stop.png");
    ui->emergencyButton->setIcon(ermergencyLogo);
    ui->emergencyButton->setIconSize(QSize(50,50));

    QIcon landLogo("./images_boutons/land.png");
    ui->landBtn->setIcon(landLogo);
    ui->landBtn->setIconSize(QSize(50,50));

    QIcon takeOffLogo("./images_boutons/take_off.png");
    ui->takeOffBtn->setIcon(takeOffLogo);
    ui->takeOffBtn->setIconSize(QSize(50,50));

}



void MainWindow::on_connectBtn_clicked(){
    if(connectBtnClicked == false){
        connectBtnClicked = true;
        ui->connectBtn->setText("Disconnect \n(Enter)");
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
}

void MainWindow::on_downBtn_clicked(){
    tello.sendCommand_generic("rc 0 0 -40 0");
}

void MainWindow::on_rightBtn_clicked(){
    tello.sendCommand_generic("rc 50 0 0");
}

void MainWindow::on_tRightBtn_clicked(){
    tello.sendCommand_generic("rc 0 0 0 70");
}

void MainWindow::on_backBtn_clicked(){
    tello.sendCommand_generic("rc 0 -50 0 0");
}

void MainWindow::on_forwardBtn_clicked(){
    tello.sendCommand_generic("rc 0 50 0 0");
}

void MainWindow::on_leftBtn_clicked(){
    tello.sendCommand_generic("rc -50 0 0 0");
}

void MainWindow::on_tLeftBtn_clicked(){
    tello.sendCommand_generic("rc 0 0 0 -70");
}

void MainWindow::on_stopMoveBtn_clicked()
{
    tello.sendCommand_generic("rc 0 0 0 0");
}

void MainWindow::on_emergencyButton_clicked(){
    tello.sendCommand_generic("emergency");
}

void MainWindow::on_takeOffBtn_clicked(){
    tello.sendCommand_generic("takeoff");
}

void MainWindow::on_landBtn_clicked(){
    tello.sendCommand_generic("land");
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
            case Qt::Key_Space:
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
    QPixmap scaledImage = image.scaled(QSize(300, 382), Qt::KeepAspectRatio);
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
        case Tello::Status::NO_RESPONSE:
            ui->label_status->setText("No Response");
            break;
        case Tello::Status::NO_RESPONSE_TIMEOUT: {
            ui->label_status->setText("No Response since 15s, Reboot");
            QPixmap image("./images_wifi/wifi0.png");
            QPixmap scaledImage = image.scaled(QSize(300, 382), Qt::KeepAspectRatio);
            ui->wifiLogo->setPixmap(scaledImage); // Affichage de l'image dans le label
            break;
            }
        case Tello::Status::CONNECTED:
            ui->label_status->setText("Connected");
            //ui->groupBox->setEnabled(true);
            break;

        case Tello::Status::CLOSED:
            ui->label_status->setText("Closed");
            //ui->groupBox->setEnabled(false);
            break;
        default:
            ui->label_status->setText("Error");
            //ui->groupBox->setEnabled(false);
            break;
    }

}

void MainWindow::updateCommandReponse(){
    QString str = "cmd: " + tello.getLastCommmandUsed() + " -> " + tello.getCommandResponseData();
    ui->lineEdit_cmd_reponse->setText(str);
}



