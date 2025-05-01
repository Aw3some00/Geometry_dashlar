#include "GameScene.h"
#include "gamescene.h"
#include <QKeyEvent>
#include <QDebug>
#include <QThread>
#include <QMediaService>
#include <QAudioOutput>
#include <QAudioOutput>
#include <iostream>
#include <QRandomGenerator>
int randomIndex;
void GameScene::restartPlayback() {
    fon->setPosition(0);
    QTimer::singleShot(50,fon, &QMediaPlayer::play);
}


int getRandomInt(int min, int max) {
    return QRandomGenerator::global()->bounded(min, max + 1);
}
void GameScene::startMusic() {
    if (!musicPlayer) return;

    if (musicPlayer->state() != QMediaPlayer::PlayingState) {
        if (musicPlayer->media().isNull()) {
            qDebug() << "Music not loaded!";
            return;
        }
        musicPlayer->play();
        qDebug() << "Music started";
    }
}

void GameScene::stopMusic() {
    if (!musicPlayer) return;

    if (musicPlayer->state() == QMediaPlayer::PlayingState) {
        musicPlayer->stop();
        qDebug() << "Music stopped";
    }
}
void GameScene::fail() {

    int randomIndex=getRandomInt(0,14);
    faultSound->setMedia(QMediaContent(soundFiles[randomIndex]));
    faultSound->setVolume(100);
    faultSound->play();

}
void GameScene::failstop(){
    faultSound->setVolume(0);
}
void GameScene::fonstop(){
    fon->setVolume(0);
}
GameScene::GameScene(QObject *parent) : QGraphicsScene(parent), currentTrack(nullptr) {

    musicPlayer = new QMediaPlayer(this);


    musicPlayer->setMedia(QMediaContent(QUrl("qrc:/neoncity.mp3")));
    musicPlayer->setVolume(100);
    if (!QFile(":/neoncity.mp3").exists()) {
        qDebug() << "ERROR: Music file not found in resources!";
        return;
    }
    fon = new QMediaPlayer(this);



    int randomIndex1=getRandomInt(0,2);
    fon->setMedia(QMediaContent(backsounds[randomIndex1]));
    fon->setVolume(10);
    fon->play();
    connect(fon, &QMediaPlayer::stateChanged, this, [this](QMediaPlayer::State state) {
        if (state == QMediaPlayer::StoppedState || fon->mediaStatus() == QMediaPlayer::EndOfMedia) {
            restartPlayback();

        }
    });

    win=new QMediaPlayer(this);
    win->setMedia(QMediaContent(QUrl("qrc:/congr.mp3")));
    win->setVolume(100);

    randomIndex = getRandomInt(0,14);

    faultSound=new QMediaPlayer(this);
    faultSound->setMedia(QMediaContent(QUrl(soundFiles[randomIndex])));
    faultSound->setVolume(100);



    // Обработка ошибок
    connect(musicPlayer, QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error),
            [](QMediaPlayer::Error error) {
                qDebug() << "Music error:" << error;
            });


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
    connect(exitButton, &QPushButton::clicked, this, [this]() {
        returnToMenu();
         failstop();
        fon->setVolume(10);
        fon->play();

    });



    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &GameScene::update);

}

GameScene::~GameScene() {

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
                    fail();
                    isGameOver = true;
                    randomIndex = getRandomInt(0,14);
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
            randomIndex = getRandomInt(0,14);
            fail();
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

    if (gameTime >= 14200) {
        isGameOver = true;
        stopMusic();
        gameTimer->stop();
        win->play();
        retryButton->show();
        qDebug() << "Track Completed!";
    }
}


void GameScene::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_R) {
        restartGame();
        qDebug() << "Level restarted via 'R' key";
        event->accept(); // Mark event as handled
    } else if (event->key() == Qt::Key_Escape) {
        emit returnToMenu();
        qDebug() << "Exiting to menu via 'Esc' key";
        event->accept(); // Mark event as handled
    } else if (event->key() == Qt::Key_Space && !isGameOver) {
        player->jump();
        qDebug() << "Jump initiated, isOnPlatform:" << isOnPlatform;
        event->accept(); // Mark event as handled
    } else {
        qDebug() << "Key press ignored: key=" << event->key()
        << "isOnPlatform=" << isOnPlatform;
    }
    // Only propagate unhandled events
    if (!event->isAccepted()) {
        QGraphicsScene::keyPressEvent(event);
    }
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
    stopMusic();
    startGame();
    failstop();
    qDebug() << "Game restarted, player pos: (100, 460), isOnGround: true";
}

void GameScene::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if (event->button() == Qt::LeftButton && !isGameOver) {
        player->jump();
        qDebug() << "Mouse jump initiated";
    }
    QGraphicsScene::mousePressEvent(event);
}

void GameScene::startGame() {
    if (!gameTimer->isActive()) {
        gameTimer->start(1000 / 60);
    }
    fonstop();
    startMusic();
    qDebug() << "Game started, timer active:" << gameTimer->isActive();
}


