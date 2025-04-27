#include "GameScene.h"
#include <QKeyEvent>
#include <QDebug>
bool on=1;
GameScene::GameScene(QObject *parent) : QGraphicsScene(parent), currentTrack(nullptr) {
    setSceneRect(0, 0, 800, 600);

    // Create player
    player = new Player();
    player->setPos(100, 460); // Adjusted to sit on gray surface
    addItem(player);

    // Setup retry button (hidden initially)
    retryButton = new QPushButton("Retry");
    retryButton->setGeometry(350, 250, 100, 40);
    retryButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #4CAF50;"
        "   color: white;"
        "   font-size: 18px;"
        "   border-radius: 10px;"
        "   border: 2px solid #388E3C;"
        "}"
        "QPushButton:hover {"
        "   background-color: #45a049;"
        "}"
        );
    retryButton->hide();
    addWidget(retryButton);
    connect(retryButton, &QPushButton::clicked, this, &GameScene::restartGame);

    // Setup exit button
    exitButton = new QPushButton("Exit");
    exitButton->setGeometry(10, 10, 60, 30);
    exitButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #f44336;"
        "   color: white;"
        "   font-size: 14px;"
        "   border-radius: 5px;"
        "   border: 2px solid #d32f2f;"
        "}"
        "QPushButton:hover {"
        "   background-color: #e53935;"
        "}"
        );
    addWidget(exitButton);
    connect(exitButton, &QPushButton::clicked, this, &GameScene::returnToMenu);

    // Setup game timer
    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &GameScene::update);
}

GameScene::~GameScene() {
    delete currentTrack;
}

void GameScene::setTrack(int trackId) {
    delete currentTrack;
    currentTrack = nullptr;
    if (trackId == 1) {
        currentTrack = new NeonCityTrack(this);
        currentTrack->setupTrack();
    } else {
        // Заглушка для других трасс
        currentTrack = new NeonCityTrack(this);
        currentTrack->setupTrack();
    }
}




void GameScene::update() {
    if (!player) return;

    isOnPlatform = false;

    player->update();
    gameTime++;

    qDebug() << "Game time:" << gameTime << "Timer before increment:" << obstacleSpawnTimer;

    if (currentTrack && obstacleSpawnTimer++ >= currentTrack->getSpawnInterval()) {
        currentTrack->spawnObstacle(gameTime, obstacleSpawnTimer);
        obstacleSpawnTimer = 0;
        qDebug() << "Timer reset to 0 after spawning";
    }

    const auto sceneItems = items();
    for (auto item : sceneItems) {
        Obstacle* obstacle = dynamic_cast<Obstacle*>(item);
        if (!obstacle) continue;

        obstacle->setPos(obstacle->x() - 3, obstacle->y());
        obstacle->update();

        bool collisionDetected = false;
        if (obstacle->getType() == Obstacle::Spike) {
            QGraphicsPolygonItem* spikePolygon = obstacle->getSpikePolygon();
            if (spikePolygon) {
                collisionDetected = player->collidesWithItem(spikePolygon, Qt::IntersectsItemShape);
                qDebug() << "Checking Spike collision: Polygon valid=" << (spikePolygon != nullptr)
                         << "Collision=" << collisionDetected
                         << "Polygon points=" << spikePolygon->polygon()
                         << "Spike pos: (" << obstacle->x() << "," << obstacle->y() << ")";
            } else {
                qDebug() << "Warning: Spike polygon is null!";
            }
        } else {
            collisionDetected = player->collidesWithItem(obstacle, Qt::IntersectsItemBoundingRect);
            qDebug() << "Checking obstacle collision: Type=" << obstacle->getType()
                     << "Obstacle rect=" << obstacle->rect()
                     << "Obstacle pos: (" << obstacle->x() << "," << obstacle->y() << ")"
                     << "Player rect=" << player->rect()
                     << "Player pos: (" << player->x() << "," << player->y() << ")";
        }

        if (collisionDetected) {
            qDebug() << "Collision detected with obstacle type:" << obstacle->getType()
            << "at position (" << obstacle->x() << "," << obstacle->y() << ")"
            << "Player position (" << player->x() << "," << player->y() << ")";

            if (obstacle->isSafeToStandOn()) {
                qreal playerBottom = player->y() + player->rect().height();
                qreal obstacleTop = obstacle->y();
                qDebug() << "Landing check: playerBottom=" << playerBottom
                         << "obstacleTop=" << obstacleTop
                         << "player->y()=" << player->y()
                         << "condition=" << (playerBottom >= obstacleTop && player->y() <= obstacleTop + 20);
                if (playerBottom >= obstacleTop && player->y() <= obstacleTop + 20) {
                    player->setPos(player->x(), obstacle->y() - player->rect().height());
                    player->resetVerticalSpeed();
                    player->setOnGround(true);
                    isOnPlatform = true;
                    qDebug() << "Player landed on Regular at y:" << obstacle->y()
                             << "Player new pos: (" << player->x() << "," << player->y() << ")"
                             << "isOnPlatform:" << isOnPlatform << "isOnGround:" << player->isOnGroundf();
                    continue;
                }

                qreal playerRight = player->x() + player->rect().width();
                qreal obstacleLeft = obstacle->x();
                if (playerRight >= obstacleLeft && player->x() < obstacleLeft) {
                    isGameOver = true;
                    gameTimer->stop();
                    retryButton->show();
                    qDebug() << "Game Over! Collided with Regular from left";
                    return;
                }

                qDebug() << "Collided with safe obstacle (Regular) from side/bottom, ignoring";
                continue;
            }

            if (obstacle->isPortal()) {
                switch (obstacle->getType()) {
                case Obstacle::TeleportPortal:
                    player->teleportTo(obstacle->getTargetX(), obstacle->getTargetY());
                    qDebug() << "Player teleported to (" << obstacle->getTargetX() << "," << obstacle->getTargetY() << ")";
                    break;
                case Obstacle::GravityPortal:
                    player->invertGravity();
                    qDebug() << "Gravity inverted";
                    break;
                case Obstacle::SpeedPortal:
                    player->setSpeedMultiplier(obstacle->getSpeedMultiplier());
                    qDebug() << "Speed multiplier set to:" << obstacle->getSpeedMultiplier();
                    break;
                default:
                    break;
                }
                removeItem(obstacle);
                delete obstacle;
                continue;
            }

            isGameOver = true;
            gameTimer->stop();
            retryButton->show();
            qDebug() << "Game Over! Collided with obstacle type:" << obstacle->getType();
            return;
        }

        if (obstacle->x() + obstacle->rect().width() < 0) {
            removeItem(obstacle);
            delete obstacle;
            continue;
        }
    }

    if (gameTime >= 18000) {
        isGameOver = true;
        gameTimer->stop();
        retryButton->show();
        qDebug() << "Track Completed!";
    }
}



void GameScene::keyPressEvent(QKeyEvent *event) {
    if (isGameOver) {
        qDebug() << "Game is over, ignoring key press";
        return;
    }

    if (event->key() == Qt::Key_Space) {
        player->jump();
        qDebug() << "Jump initiated, isOnPlatform:" << isOnPlatform;
    } else {
        qDebug() << "Key press ignored: key=" << event->key()
        << "isOnPlatform=" << isOnPlatform;
    }
    QGraphicsScene::keyPressEvent(event);
}

void GameScene::restartGame() {
    for (auto item : items()) {
        if (dynamic_cast<Obstacle*>(item)) {
            removeItem(item);
            delete item;
        }
    }
    player->setPos(100, 460);
    player->setSpeedMultiplier(1.0);
    player->resetVerticalSpeed();
    player->setOnGround(true);
    isGameOver = false;
    isOnPlatform = false;
    gameTime = 0;
    obstacleSpawnTimer = 0;
    retryButton->hide();
    startGame();
    qDebug() << "Game restarted, player pos: (100, 460), isOnGround: true";
}

void GameScene::startGame() {
    if (!gameTimer->isActive()) {
        gameTimer->start(1000 / 60);
    }
    qDebug() << "Game started, timer active:" << gameTimer->isActive();
}
