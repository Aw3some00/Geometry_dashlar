


QT += core gui widgets multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# Стандарт C++17 для совместимости с Qt 5.16.0 и clang на macOS
CONFIG += c++17

# Имя исполняемого файла
TARGET = App
TEMPLATE = app

# Исходные файлы (логика игры и интерфейса)
SOURCES += \
    gamescene.cpp \
    main.cpp \
    mainmenu.cpp \
    mainwindow.cpp \
    neoncitytrack.cpp \
    obstacle.cpp \
    player.cpp \
    settingsdialog.cpp \
    theme.cpp

# Заголовочные файлы (объявления классов)
HEADERS += \
    gamescene.h \
    mainmenu.h \
    mainwindow.h \
    neoncitytrack.h \
    obstacle.h \
    player.h \
    settingsdialog.h \
    theme.h

# UI-файл (интерфейс главного окна)
FORMS += \
    mainwindow.ui

# Файлы ресурсов (музыка и визуальные элементы)
RESOURCES += \
    music.qrc \
    nightBaku.qrc

# Настройки установки для Unix/macOS (опционально, для make install)
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
macx {
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.14
    CONFIG += x86_64
}

# Предотвращение проблем с MOC
CONFIG += warn_on
