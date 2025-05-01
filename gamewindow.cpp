#include "gamewindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

GameWindow::GameWindow(int trackId, int themeIndex, bool isFullScreen, QWidget *parent)
    : QMainWindow(parent) {
    theme = new Theme();
    setMinimumSize(1024, 768);

    // Create a central widget and layout to center the view
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    QHBoxLayout* horizontalLayout = new QHBoxLayout();

    // Setup game view
    view = new QGraphicsView(this);
    scene = new GameScene(this);
    view->setScene(scene);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Set the fixed size of the view to match the scene size
    view->setFixedSize(800, 600); // Matches scene size set in GameScene (800x600)
    view->setSceneRect(0, 0, 800, 600); // Ensure the view shows the entire scene

    // Add the view to the layout and center it
    horizontalLayout->addStretch(); // Add stretchable space on the left
    horizontalLayout->addWidget(view); // Add the view
    horizontalLayout->addStretch(); // Add stretchable space on the right
    mainLayout->addStretch(); // Add stretchable space on top
    mainLayout->addLayout(horizontalLayout); // Add the horizontal layout with the view
    mainLayout->addStretch(); // Add stretchable space on the bottom

    // Set the central widget
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // Connect return to menu signal
    connect(scene, &GameScene::returnToMenu, this, &GameWindow::returnToMenu);

    // Apply theme and start game
    applyTheme(themeIndex);
    scene->setTrack(trackId);
    scene->startGame();

    // Apply fullscreen setting
    if (isFullScreen) {
        showFullScreen();
    } else {
        show();
    }
}

GameWindow::~GameWindow() {
    delete theme;
}

void GameWindow::applyTheme(int themeIndex) {
    setStyleSheet(theme->getMainMenuStyle(themeIndex));
}

void GameWindow::returnToMenu() {
    emit closed();
    close();
}
