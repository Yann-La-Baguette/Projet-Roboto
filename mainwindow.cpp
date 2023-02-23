#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    ui->image->setStyleSheet("background-color: #FF0000;");

    // Icone et nom d'application
    setWindowTitle("Roboto Collaboratif");
    setWindowIcon(QIcon("./icone"));

    // Affichage des données du drone
    affichageCompass();
    affichageAttitude();
    affichageHeight();
    affichageWifi();

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
    pen.setWidth(10);
    pen.setColor(Qt::blue);
    painter.setPen(pen);
    for(int i = 0 ; i<points.count() ; i++){
        QPoint point = points[i];
        painter.drawPoint(point);
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
        QRect rect = ui->image->geometry();
        if (rect.contains(event->pos())){
            QPoint p = ui->image->mapFromParent(event->pos());
            points.append(p);

            ui->cooSouris->setText("Coordonnées : \nx : " + QString::number(p.rx()) + "\ny : " + QString::number(p.ry()));
            valeurDispo = true;
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
    mAirspeedGauge->addDegrees(65)->setValueRange(0,120);
    QcColorBand *clrBand = mAirspeedGauge->addColorBand(50);
    clrBand->setValueRange(0,100);
    mAirspeedGauge->addValues(80)->setValueRange(0,30);
    mAirspeedGauge->addLabel(70)->setText("Height (m)");
    QcLabelItem *lab = mAirspeedGauge->addLabel(40);
    lab->setText("0");
    mAirspeedNeedle = mAirspeedGauge->addNeedle(60);
    mAirspeedNeedle->setLabel(lab);
    mAirspeedNeedle->setColor(Qt::blue);
    mAirspeedNeedle->setValueRange(0,30);
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
    w->setText("W");
    w->setAngle(0);
    w->setColor(Qt::white);

    QcLabelItem *n = mCompassGauge->addLabel(80);
    n->setText("N");
    n->setAngle(90);
    n->setColor(Qt::white);

    QcLabelItem *e = mCompassGauge->addLabel(80);
    e->setText("E");
    e->setAngle(180);
    e->setColor(Qt::white);

    QcLabelItem *s = mCompassGauge->addLabel(80);
    s->setText("S");
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
}

void MainWindow::affichageWifi(){
    QString imageName = "./images_wifi/wifi4.png"; // Nom de l'image à charger
    QPixmap image(imageName); // Chargement de l'image
    QPixmap scaledImage = image.scaled(QSize(300, 382), Qt::KeepAspectRatio);
    ui->wifiLogo->setPixmap(scaledImage); // Affichage de l'image dans le label
}


void MainWindow::on_verticalSlider_valueChanged(int value)
{
    // Height
    mAirspeedNeedle->setCurrentValue(value);
}

void MainWindow::on_verticalSlider_2_valueChanged(int value)
{
    // Attitude Pitch
    mAttMeter->setCurrentPitch(value);
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    // Attitude Roll
    mAttitudeNeedle->setCurrentValue(90-value);
    mAttMeter->setCurrentRoll(value);
}

void MainWindow::on_horizontalSlider_2_valueChanged(int value)
{
    // Compass
    mCompassNeedle->setCurrentValue(value);
}

void MainWindow::on_horizontalSlider_3_valueChanged(int value)
{
    // Wifi Symbol
    QString imageName = "./images_wifi/wifi" + QString::number(value) + ".png"; // Nom de l'image à charger
    QPixmap image(imageName); // Chargement de l'image
    QPixmap scaledImage = image.scaled(QSize(300, 382), Qt::KeepAspectRatio);
    ui->wifiLogo->setPixmap(scaledImage); // Affichage de l'image dans le label
}

