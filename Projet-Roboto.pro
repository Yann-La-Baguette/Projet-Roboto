QT       += core gui network websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Tello/Tello.cpp \
    Tello/TelloCommand.cpp \
    Tello/TelloLogger.cpp \
    Tello/TelloState.cpp \
    Tello/TelloStream.cpp \
    gamepadmanager.cpp \
    main.cpp \
    mainwindow.cpp \
    qcgaugewidget.cpp \

HEADERS += \
    Tello/TelloCommand.h \
    Tello/TelloEnumTypes.h \
    Tello/TelloLogger.h \
    Tello/TelloState.h \
    Tello/TelloStream.h \
    Tello/tello.h \
    gamepadmanager.h \
    mainwindow.h \
    qcgaugewidget.h \


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
    images_boutons/back.png \
    images_boutons/battery.png \
    images_boutons/capture.png \
    images_boutons/connect.png \
    images_boutons/down.png \
    images_boutons/forward.png \
    images_boutons/land.png \
    images_boutons/left.png \
    images_boutons/reset_waypoints.png \
    images_boutons/right.png \
    images_boutons/stopMove.png \
    images_boutons/tLeft.png \
    images_boutons/tRight.png \
    images_boutons/take_off.png \
    images_boutons/up.png \
    images_wifi/wifi0.png \
    images_wifi/wifi1.png \
    images_wifi/wifi2.png \
    images_wifi/wifi3.png \
    images_wifi/wifi4.png \

DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += C:\Users\ydipadova\Documents\opencv\build\include

LIBS += C:\Users\ydipadova\Documents\opencv\buildcustom\bin\libopencv_calib3d470.dll
LIBS += C:\Users\ydipadova\Documents\opencv\buildcustom\bin\libopencv_core470.dll
LIBS += C:\Users\ydipadova\Documents\opencv\buildcustom\bin\libopencv_features2d470.dll
LIBS += C:\Users\ydipadova\Documents\opencv\buildcustom\bin\libopencv_flann470.dll
LIBS += C:\Users\ydipadova\Documents\opencv\buildcustom\bin\libopencv_gapi470.dll
LIBS += C:\Users\ydipadova\Documents\opencv\buildcustom\bin\libopencv_highgui470.dll
LIBS += C:\Users\ydipadova\Documents\opencv\buildcustom\bin\libopencv_imgcodecs470.dll
LIBS += C:\Users\ydipadova\Documents\opencv\buildcustom\bin\libopencv_imgproc470.dll
LIBS += C:\Users\ydipadova\Documents\opencv\buildcustom\bin\libopencv_ml470.dll
LIBS += C:\Users\ydipadova\Documents\opencv\buildcustom\bin\libopencv_objdetect470.dll
LIBS += C:\Users\ydipadova\Documents\opencv\buildcustom\bin\libopencv_photo470.dll
LIBS += C:\Users\ydipadova\Documents\opencv\buildcustom\bin\libopencv_stitching470.dll
LIBS += C:\Users\ydipadova\Documents\opencv\buildcustom\bin\libopencv_video470.dll
LIBS += C:\Users\ydipadova\Documents\opencv\buildcustom\bin\libopencv_videoio470.dll
LIBS += C:\Users\ydipadova\Documents\opencv\buildcustom\bin\opencv_videoio_ffmpeg470_64.dll

LIBS += -lXinput
