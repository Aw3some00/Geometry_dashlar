#include <QDebug>


#include "NeonCityTrack.h"
#include <QLinearGradient>
#include <QGraphicsRectItem>
#include <QRandomGenerator>
#include <QPainter>
#include <QPixmap>
//#include <QSoundEffect>
#include <QDebug>

NeonCityTrack::NeonCityTrack(QGraphicsScene *scene) : scene(scene), backgroundItem(nullptr) {


}

void NeonCityTrack::setupTrack() {

    setupBackground();
    setupGround();


}


void NeonCityTrack::setupBackground() {
    // Загружаем изображение с правильным путём
    QPixmap backgroundImage(":/new/prefix1/nightbaku.jpg");
    if (backgroundImage.isNull()) {
        qDebug() << "Failed to load background image! Path: :/new/prefix1/nightbaku.jpg";
        // Запасной вариант: градиентный фон
        QLinearGradient backgroundGradient(0, 0, 0, 600);
        backgroundGradient.setColorAt(0, QColor(255, 69, 0));
        backgroundGradient.setColorAt(0.5, QColor(255, 165, 0));
        backgroundGradient.setColorAt(1, QColor(255, 215, 0));
        scene->setBackgroundBrush(QBrush(backgroundGradient));
        return;
    }


    // Создаём элемент сцены для изображения
    backgroundItem = new QGraphicsPixmapItem(backgroundImage);

    // Масштабируем изображение, чтобы оно соответствовало размеру сцены (800x600)
    backgroundItem->setScale(800.0 / backgroundImage.width());
    if (backgroundImage.height() * backgroundItem->scale() > 600) {
        backgroundItem->setScale(600.0 / backgroundImage.height());
    }

    // Устанавливаем позицию (0, 0) и добавляем на сцену
    backgroundItem->setPos(0, 0);
    backgroundItem->setZValue(-1); // Помещаем фон позади всех элементов
    scene->addItem(backgroundItem);
}

void NeonCityTrack::setupGround() {
    // Ground with Baku-inspired design
    QGraphicsRectItem *ground = new QGraphicsRectItem(0, 500, 800, 100);
    QLinearGradient groundGradient(0, 500, 0, 600);
    groundGradient.setColorAt(0, Qt::black);
    groundGradient.setColorAt(1, Qt::white);
    ground->setBrush(QBrush(groundGradient));
    scene->addItem(ground);

    // Checkered pattern on ground
    for (int i = 0; i < 800; i += 40) {
        QGraphicsRectItem *check = new QGraphicsRectItem(i, 500, 20, 20);
        check->setBrush(QBrush((i / 40) % 2 == 0 ? Qt::black : Qt::white));
        scene->addItem(check);
    }
}






void NeonCityTrack::spawnObstacle(int gameTime, int &obstacleSpawnTimer) {

    if (!scene) {
        qDebug() << "Error: Scene is null in spawnObstacle";
        return;
    }

    int seconds = gameTime / 60;
    qreal amplitude = getAudioAmplitude(obstacleSpawnTimer);

    qDebug() << "Game time:" << gameTime << "Seconds:" << seconds
             << "Amplitude:" << amplitude << "Timer:" << obstacleSpawnTimer;

    Obstacle* obstacle = nullptr;
    qreal baseY = 500;

    if (seconds < 30) { // Вступление (0:00–0:30)
        if (amplitude > 0.3) {
            // Чередуем платформы и спайки
            if (seconds % 2 == 0) {
                obstacle = new Obstacle(Obstacle::Regular);
                obstacle->setPos(800, baseY - obstacle->rect().height());
                qDebug() << "Spawning Regular platform (Intro)";
            } else {
                obstacle = new Obstacle(Obstacle::Spike);
                obstacle->setPos(800, baseY - 30); // Спайк на земле
                qDebug() << "Spawning Spike (Intro)";
            }
        }
    } else if (seconds < 90) { // Куплет (0:30–1:30)
        if (amplitude > 0.6) {
            obstacle = new Obstacle(Obstacle::MovingPlatform);
            obstacle->setPos(800, baseY - 100);
            qDebug() << "Spawning MovingPlatform (Verse)";
        } else if (amplitude > 0.4) {
            obstacle = new Obstacle(Obstacle::Spike);
            obstacle->setPos(800, baseY - 30);
            qDebug() << "Spawning Spike (Verse)";
        }
    } else if (seconds < 150) { // Припев (1:30–2:30)
        if (amplitude > 0.8) {
            obstacle = new Obstacle(Obstacle::Sawblade);
            obstacle->setPos(800, baseY - 30);
            qDebug() << "Spawning Sawblade (Chorus)";
        } else if (amplitude > 0.5) {
            obstacle = new Obstacle(Obstacle::Regular);
            obstacle->setPos(800, baseY - 200); // Платформа в воздухе
            qDebug() << "Spawning Regular platform (Chorus)";
        }
    } else if (seconds < 180) { // Спад (2:30–3:00)
        if (amplitude > 0.4) {
            obstacle = new Obstacle(Obstacle::JumpPad);
            obstacle->setPos(800, baseY - obstacle->rect().height());
            qDebug() << "Spawning JumpPad (Drop)";
        }
    } else { // Финал (3:00–4:00)
        if (amplitude > 0.7) {
            obstacle = new Obstacle(Obstacle::TeleportPortal);
            obstacle->setPos(800, baseY - obstacle->rect().height());
            qDebug() << "Spawning TeleportPortal (Finale)";
        } else if (amplitude > 0.5) {
            obstacle = new Obstacle(Obstacle::Spike);
            obstacle->setPos(800, baseY - 30);
            qDebug() << "Spawning Spike (Finale)";
        }
    }

    if (obstacle) {
        scene->addItem(obstacle);
        qDebug() << "Obstacle added to scene";
    } else {
        qDebug() << "No obstacle spawned";
    }
}


qreal NeonCityTrack::getAudioAmplitude(int timer) {
    int seconds = timer / 60;
    if (seconds < 30) return 0.4; // Вступление: достаточно для Regular и Spike
    else if (seconds < 90) return 0.7; // Куплет: достаточно для MovingPlatform и Spike
    else if (seconds < 150) return 0.9; // Припев: достаточно для Sawblade и Regular
    else if (seconds < 180) return 0.5; // Спад: достаточно для JumpPad
    else return 0.8; // Финал: достаточно для TeleportPortal и Spike
}

int NeonCityTrack::getSpawnInterval() const {
    return 30; // 0.5 секунды при 60 FPS
}
