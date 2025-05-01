QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    gamescene.cpp \
    main.cpp \
    mainmenu.cpp \
    mainwindow.cpp \
    neoncitytrack.cpp \
    obstacle.cpp \
    player.cpp \
    gamewindow.cpp\
    settingsdialog.cpp \
    theme.cpp

HEADERS += \
    gamescene.h \
    mainmenu.h \
    mainwindow.h \
    neoncitytrack.h \
    obstacle.h \
    player.h \
    gamewindow.h \
    settingsdialog.h \
    theme.h

FORMS += \
    mainwindow.ui

RESOURCES += music.qrc \
    nightBaku.qrc \
    portal.qrc \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


