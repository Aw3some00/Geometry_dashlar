#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "mainmenu.h"
#include "gamescene.h"
#include "theme.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void applyTheme(int themeIndex);

public slots:
    void startGame(int trackId);
    void returnToMenu();
    void updateDisplayMode(bool isFullScreen);

private:
    Theme* theme;
    QStackedWidget* stackedWidget;
    MainMenu* menu;
    QGraphicsView* view;
    GameScene* scene;
};

#endif // MAINWINDOW_H
