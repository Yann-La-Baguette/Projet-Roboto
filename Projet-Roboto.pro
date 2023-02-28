QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    qcgaugewidget.cpp

HEADERS += \
    mainwindow.h \
    qcgaugewidget.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Image_satellite_test.png \
    icone.ico \
    images_boutons/Stop.png \
    images_boutons/land.png \
    images_boutons/take_off.png \
    images_wifi/wifi0.png \
    images_wifi/wifi1.png \
    images_wifi/wifi2.png \
    images_wifi/wifi3.png \
    images_wifi/wifi4.png
