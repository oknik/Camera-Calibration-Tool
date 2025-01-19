QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# source file and head file
SOURCES += \
    listcontrol.cpp \
    listwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    selectdlg.cpp

HEADERS += \
    include/BoardType.h \
    include/CalibrationOptions.h \
    include/DistortionLevel.h \
    include/CalculateCorner.h \
    include/CameraCalibrate.h \
    include/ImageUndistort.h \
    include/PointMapper.h \
    include/Tools.h \
    listcontrol.h \
    listwidget.h \
    mainwindow.h \
    selectdlg.h

FORMS += \
    listcontrol.ui \
    listwidget.ui \
    mainwindow.ui \
    selectdlg.ui

RESOURCES += res.qrc

RC_ICONS = icon.ico

INCLUDEPATH += include

win32 {
    # Windows-specific settings
    msvc {
        QMAKE_CFLAGS += /utf-8
        QMAKE_CXXFLAGS += /utf-8
    }

    LIBS += "D:/CV_Project/CV_Project/Computer-Vision-QT/lib/win32/Debug/CV_DLL.lib"
    LIBS += "D:/CV_Project/CV_Project/Computer-Vision-QT/lib/win32/Debug/opencv_world4100d.lib"
}

unix:!macx {
    INCLUDEPATH += /usr/local/include/opencv4
    LIBS += -L/usr/local/lib
    LIBS += -L $$PWD/lib/linux -lCV_DLL
    LIBS += -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_features2d -lopencv_calib3d -lopencv_imgcodecs
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
