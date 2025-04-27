#include "mainwindow.h"
#include <QSettings>
#include<QGraphicsView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    theme = new Theme();
    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    // Setup main menu
    menu = new MainMenu(this);
    stackedWidget->addWidget(menu);
    connect(menu, &MainMenu::startGame, this, &MainWindow::startGame);

    // Setup game view
    view = new QGraphicsView();
    scene = new GameScene(this);
    view->setScene(scene);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    stackedWidget->addWidget(view);

    // Connect the returnToMenu signal from GameScene
    connect(scene, &GameScene::returnToMenu, this, &MainWindow::returnToMenu);

    // Show menu initially
    stackedWidget->setCurrentWidget(menu);

    // Window settings
    setMinimumSize(1024, 768);

    // Apply saved theme
    QSettings settings("Dashlar", "Geometry_Dashlar");
    applyTheme(settings.value("theme", 0).toInt());
}

void MainWindow::applyTheme(int themeIndex) {
    menu->applyTheme(themeIndex, isFullScreen());
    setStyleSheet(theme->getMainMenuStyle(themeIndex));
}

void MainWindow::startGame(int trackId) {
    scene->setTrack(trackId);
    stackedWidget->setCurrentWidget(view);
    view->setFixedSize(stackedWidget->size());
    scene->startGame();
}

void MainWindow::returnToMenu() {
    stackedWidget->setCurrentWidget(menu);
    QSettings settings("Geometry_Dashlar", "Geometry_Dashlar");
    applyTheme(settings.value("theme", 0).toInt());
}

void MainWindow::updateDisplayMode(bool isFullScreen) {
    QSettings settings("Geometry_Dashlar", "Geometry_Dashlar");
    menu->applyTheme(settings.value("theme", 0).toInt(), isFullScreen);
}
