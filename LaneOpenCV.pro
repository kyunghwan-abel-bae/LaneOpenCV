QT += core qml quick multimedia quickcontrols2

CONFIG += c++11

SOURCES += main.cpp \
    opencvhelper.cpp \
    adas_filter.cpp

RESOURCES += qml.qrc

static {
    QT += svg
    QTPLUGIN += qtvirtualkeyboardplugin
}

linux:!android {


DEFINES += MAIN_QML=\\\"main_with_virtualkeyboard.qml\\\"
CONFIG += disable-desktop

INCLUDEPATH += /usr/local/include

LIBS += -L/home/abel/gsl_arm/lib \
        -L/home/abel/Desktop/cmake_opencv_lib/lib \
        -L/usr/local/lib \
         -lopencv_imgcodecs \
         -lopencv_core \
         -lopencv_imgproc \
         -lopencv_video \
         -lgsl \
         -lgslcblas \
}

android {

DEFINES += MAIN_QML=\\\"main.qml\\\"

}

osx {

DEFINES += MAIN_QML=\\\"main.qml\\\"

INCLUDEPATH += "/usr/local/Cellar/gsl/2.5/include"

CONFIG -= app_bundle

LIBS += -L/usr/local/lib\
         -L/usr/local/Cellar/opencv3/3.1.0_4/share/OpenCV/3rdparty/lib\
         -lgsl\
         -lcblas

QT_CONFIG -= no-pkg-config
CONFIG  += link_pkgconfig
PKGCONFIG += opencv

}

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# edit by KH -- should be erased
#disable-desktop|!isEmpty(CROSS_COMPILE)|qnx {
#    DEFINES += MAIN_QML=\\\"main.qml\\\"
#} else {
#    DEFINES += MAIN_QML=\\\"main_with_virtualkeyboard.qml\\\"
#}

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    opencvhelper.h \
    rgbframehelper.h \
    adas_filter.h
