#include "GameScene.h"
#include <QKeyEvent>
#include <QDebug>
#include <QThread>
#include <QMediaService>
#include <QAudioOutput>
bool on=1;
void GameScene::setupMusic() {
    qDebug() << "Starting music setup...";




    if (!musicEffect) {
        try {
            musicEffect = new QSoundEffect(this);
            QUrl sourceUrl("qrc:/music/neoncity.wav");
            qDebug() << "Source URL:" << sourceUrl.toString() << "Valid:" << sourceUrl.isValid();
            musicEffect->setSource(sourceUrl);
            musicEffect->setLoopCount(QSoundEffect::Infinite);
            musicEffect->setVolume(0.5);
            qDebug() << "Sound effect initialized with source: qrc:/music/neoncity.wav";

            // Отладка статуса и ошибок
            connect(musicEffect, &QSoundEffect::statusChanged, this, [this]() {
                qDebug() << "Sound effect status:" << musicEffect->status();
                if (musicEffect->status() == QSoundEffect::Error) {
                    qDebug() << "Sound effect error: Unable to load qrc:/music/neoncity.wav";
                }
            });

            connect(musicEffect, &QSoundEffect::sourceChanged, this, [this]() {
                qDebug() << "Sound source changed to:" << musicEffect->source();
            });

            // Проверка доступности аудиоустройства с увеличенным числом попыток
            bool deviceReady = false;
            for (int i = 0; i < 5; ++i) {
                QThread::msleep(200); // Задержка 200 мс
                if (musicEffect->isLoaded()) {
                    deviceReady = true;
                    break;
                }
                qDebug() << "Audio device attempt " << (i + 1) << ": not ready";
            }
            qDebug() << "Audio device available:" << deviceReady;
        } catch (...) {
            qDebug() << "Exception caught during musicEffect initialization";
            musicEffect = nullptr;
        }
    }
}
// Включение музыки
void GameScene::startMusic() {
    if (musicEffect) {
        if (musicEffect->status() == QSoundEffect::Ready && musicEffect->isLoaded()) {
            if (!musicEffect->isPlaying()) {
                qDebug() << "Attempting to play sound effect...";
                try {
                    QThread::msleep(100); // Задержка для стабилизации бэкенда
                    musicEffect->play();
                    qDebug() << "Sound effect started";
                } catch (...) {
                    qDebug() << "Exception caught while playing sound effect";
                }
            } else {
                qDebug() << "Sound effect already playing";
            }
        } else {
            qDebug() << "Cannot start sound: Status=" << musicEffect->status() << ", Loaded=" << musicEffect->isLoaded();
        }
    } else {
        qDebug() << "Cannot start sound: musicEffect is null";
    }
}

// Выключение музыки
void GameScene::stopMusic() {
    if (musicEffect && musicEffect->isPlaying()) {
        qDebug() << "Stopping sound effect...";
        try {
            musicEffect->stop();
            qDebug() << "Sound effect stopped";
        } catch (...) {
            qDebug() << "Exception caught while stopping sound effect";
        }
    } else {
        qDebug() << "Sound not playing or musicEffect is null";
    }
}

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
    connect(retryButton, &QPushButton::clicked, this,&GameScene::restartGame);


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
    connect(exitButton, &QPushButton::clicked, this,&GameScene::returnToMenu);



    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &GameScene::update);
    setupMusic();
}

GameScene::~GameScene() {
    if (musicEffect) {
        qDebug() << "Cleaning up musicEffect...";
        try {
            stopMusic();
            musicEffect->setSource(QUrl()); // Очистка источника
            delete musicEffect;
            musicEffect = nullptr;
        } catch (...) {
            qDebug() << "Exception caught while cleaning up musicEffect";
        }
    }
    delete currentTrack;
    currentTrack = nullptr;
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

    QList<QGraphicsItem*> sceneItems = items();
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
                qreal playerTop = player->y();
                qreal playerRight = player->x() + player->rect().width();
                qreal playerLeft = player->x();
                qreal obstacleTop = obstacle->y();
                qreal obstacleBottom = obstacle->y() + obstacle->rect().height();
                qreal obstacleLeft = obstacle->x();
                qreal obstacleRight = obstacle->x() + obstacle->rect().width();

                // Проверка столкновения слева
                bool isLeftCollision = playerRight > obstacleLeft && playerLeft < obstacleRight &&
                                       playerBottom > obstacleTop && playerTop < obstacleBottom &&
                                       player->getVerticalSpeed() <= 0; // Только если не падает сверху
                qDebug() << "Left collision check: playerRight=" << playerRight
                         << "obstacleLeft=" << obstacleLeft
                         << "playerLeft=" << playerLeft
                         << "obstacleRight=" << obstacleRight
                         << "playerBottom=" << playerBottom
                         << "obstacleTop=" << obstacleTop
                         << "playerTop=" << playerTop
                         << "obstacleBottom=" << obstacleBottom
                        // << "verticalSpeed=" << player->verticalSpeed()
                         << "condition=" << isLeftCollision;

                if (isLeftCollision) {
                    isGameOver = true;
                    stopMusic();
                    gameTimer->stop();
                    retryButton->show();
                    qDebug() << "Game Over! Collided with Regular from left";
                    return;
                }

                // Проверка посадки сверху
                bool isLanding = playerBottom >= obstacleTop && playerTop <= obstacleTop + 20 &&
                                 player->getVerticalSpeed() >= 0 && playerRight > obstacleLeft &&
                                 playerLeft < obstacleRight;
                qDebug() << "Landing check: playerBottom=" << playerBottom
                         << "obstacleTop=" << obstacleTop
                         << "playerTop=" << playerTop
                       //  << "verticalSpeed=" << player->verticalSpeed()
                         << "playerRight=" << playerRight
                         << "obstacleLeft=" << obstacleLeft
                         << "playerLeft=" << playerLeft
                         << "obstacleRight=" << obstacleRight
                         << "condition=" << isLanding;

                if (isLanding) {
                    player->setPos(player->x(), obstacle->y() - player->rect().height());
                    player->resetVerticalSpeed();
                    player->setOnGround(true);
                    isOnPlatform = true;
                    qDebug() << "Player landed on Regular at y:" << obstacle->y()
                             << "Player new pos: (" << player->x() << "," << player->y() << ")"
                             << "isOnPlatform:" << isOnPlatform << "isOnGround:" << player->isOnGroundf();
                    continue;
                }

                // Игнорируем другие столкновения с Regular
                qDebug() << "Collided with safe obstacle (Regular) from side/bottom, ignoring";
                continue;
            }

            // Обработка порталов
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

            // Для всех остальных препятствий (например, Spike) — Game Over
            isGameOver = true;
            stopMusic();
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
        stopMusic();
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
    musicPlayer->play();
    qDebug() << "Game started, timer active:" << gameTimer->isActive();
}
