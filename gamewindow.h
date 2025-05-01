#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include "gamescene.h"
#include "theme.h"

class GameWindow : public QMainWindow {
    Q_OBJECT

public:
    GameWindow(int trackId, int themeIndex, bool isFullScreen, QWidget *parent = nullptr);
    ~GameWindow();

public slots:
    void returnToMenu();

signals:
    void closed();

private:
    void applyTheme(int themeIndex);
    GameScene* scene;
    QGraphicsView* view;
    Theme* theme;
};

#endif // GAMEWINDOW_H
