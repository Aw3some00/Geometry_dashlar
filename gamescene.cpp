#include "GameScene.h"
#include <QKeyEvent>
#include <QDebug>
#include <QMediaPlayer>

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

    mediaPlayer = new QMediaPlayer(this);
    mediaPlayer->setMedia(QUrl("qrc:/music/neoncity.mp3"));
    mediaPlayer->setVolume(50);

    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &GameScene::update);
    connect(mediaPlayer, &QMediaPlayer::mediaStatusChanged, this, &GameScene::handleMediaStatusChanged);
    connect(mediaPlayer, &QMediaPlayer::error, this, &GameScene::handleMediaError);
    mediaPlayer->play();
}

GameScene::~GameScene() {
    delete currentTrack;
}
void GameScene::handleMediaStatusChanged(QMediaPlayer::MediaStatus status) {
    qDebug() << "Media status changed:" << status;
    if (status == QMediaPlayer::LoadedMedia) {
        qDebug() << "Music loaded successfully";
    } else if (status == QMediaPlayer::PlayingState) {
        qDebug() << "Music playback started";
    } else if (status == QMediaPlayer::EndOfMedia) {
        mediaPlayer->play(); // Зацикливаем воспроизведение
    }
}

void GameScene::handleMediaError(QMediaPlayer::Error error) {
    qDebug() << "Media error:" << error << mediaPlayer->errorString();
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

void GameScene::startGame() {
    gameTimer->start(16);
    gameTime = 0;
    retryButton->hide();
}

void GameScene::restartGame() {
    // Clear obstacles
    for (auto item : items()) {
        if (dynamic_cast<Obstacle*>(item)) {
            removeItem(item);
            delete item;
        }
    }
    player->setPos(100, 460);
    player->setSpeedMultiplier(1.0);
    gameTime = 0;
    startGame();
}

void GameScene::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Space) {
        player->jump();
    }
}

void GameScene::update() {
    if (!player) return;

    player->update();
    gameTime++;

    // Отладка
    qDebug() << "Game time:" << gameTime << "Timer before increment:" << obstacleSpawnTimer;

    // Spawn obstacles
    if (currentTrack && obstacleSpawnTimer++ >= currentTrack->getSpawnInterval()) {
        currentTrack->spawnObstacle(gameTime, obstacleSpawnTimer);
        obstacleSpawnTimer = 0;
        qDebug() << "Timer reset to 0 after spawning";
    }

    // Используем копию списка для безопасной итерации
    const auto sceneItems = items();
    for (auto item : sceneItems) {
        Obstacle* obstacle = dynamic_cast<Obstacle*>(item);
        if (!obstacle) continue;

        // Двигаем препятствие
        obstacle->setPos(obstacle->x() - 5, obstacle->y());
        obstacle->update();

        // Проверяем столкновения
        if (player->collidesWithItem(obstacle, Qt::IntersectsItemBoundingRect)) {
            // Если препятствие безопасное (например, Regular), игрок может стоять на нём
            if (obstacle->isSafeToStandOn()) {
                // Убедимся, что игрок падает сверху
                if (player->y() + player->rect().height() <= obstacle->y() + 5) {
                    player->setPos(player->x(), obstacle->y() - player->rect().height());
                    player->resetVerticalSpeed();
                    qDebug() << "Player landed on Regular at y:" << obstacle->y();
                    continue;
                }
            }

            // Обрабатываем порталы
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

            // Для небезопасных препятствий (не порталов) — игра заканчивается
            if (!obstacle->isSafeToStandOn()) {
                gameTimer->stop();
                retryButton->show();
                qDebug() << "Game Over! Collided with obstacle type:" << obstacle->getType();
                return; // Прерываем обновление
            }
        }

        // Удаляем препятствия за пределами экрана
        if (obstacle->x() + obstacle->rect().width() < 0) {
            removeItem(obstacle);
            delete obstacle;
            continue;
        }
    }

    // Завершение трассы
    if (gameTime >= 18000) {
        gameTimer->stop();
        retryButton->show();
        qDebug() << "Track Completed!";
    }
}
