QT += quick serialport widgets

CONFIG += c++11

TARGET = MC5000-GUI

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        serialcommunication.cpp

RESOURCES += \
    font/Roboto-Regular.ttf \
    qml.qrc

ICON = icons/icon.icns
RC_ICONS = icons/icon.ico

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    serialcommunication.h

DISTFILES += \
    icons/icon.ico \
    icons/icon.icns
