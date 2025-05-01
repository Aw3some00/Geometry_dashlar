#include <QDebug>


#include "NeonCityTrack.h"
#include <QLinearGradient>
#include <QGraphicsRectItem>
#include <QRandomGenerator>
#include <QPainter>
#include <QPixmap>
#include <QSoundEffect>
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






/*void NeonCityTrack::spawnObstacle(int gameTime, int &obstacleSpawnTimer) {

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

/*void NeonCityTrack::spawnObstacle(int gameTime, int &obstacleSpawnTimer) {
    if (!scene) {
        qDebug() << "Error: Scene is null in spawnObstacle";
        return;
    }

    int seconds = gameTime / 60;
    qDebug() << "Game time:" << gameTime << " Seconds:" << seconds << " Timer:" << obstacleSpawnTimer;

    Obstacle* obstacle = nullptr;
    qreal baseY = 500;

    // Fixed obstacle timeline for 3:52 (13,920 frames) at 104 BPM (35 frames/beat)
    // Simplified track with fewer, easier obstacles

    // Intro (0:00–0:30): Simple platforms, spaced out
    if (gameTime == 70) { // 0:01 (Beat 2)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(800, baseY - obstacle->rect().height());
        qDebug() << "Spawning Regular platform at 0:01 (frame 70)";
    }
    if (gameTime == 210) { // 0:03 (Beat 6)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(800, baseY - obstacle->rect().height());
        qDebug() << "Spawning Regular platform at 0:03 (frame 210)";
    }
    if (gameTime == 350) { // 0:05 (Beat 10)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(800, baseY - obstacle->rect().height());
        qDebug() << "Spawning Regular platform at 0:05 (frame 350)";
    }
    if (gameTime == 490) { // 0:08 (Beat 14)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(800, baseY - obstacle->rect().height());
        qDebug() << "Spawning Regular platform at 0:08 (frame 490)";
    }
    if (gameTime == 630) { // 0:10 (Beat 18)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(800, baseY - obstacle->rect().height());
        qDebug() << "Spawning Regular platform at 0:10 (frame 630)";
    }

    // Verse (0:30–1:30): Platforms with occasional spikes
    if (gameTime == 1890) { // 0:31 (Beat 54)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(800, baseY - obstacle->rect().height());
        qDebug() << "Spawning Regular platform at 0:31 (frame 1890)";
    }
    if (gameTime == 2310) { // 0:38 (Beat 66)
        obstacle = new Obstacle(Obstacle::Spike);
        obstacle->setPos(800, baseY - 30);
        qDebug() << "Spawning Spike at 0:38 (frame 2310)";
    }
    if (gameTime == 2730) { // 0:45 (Beat 78)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(800, baseY - obstacle->rect().height());
        qDebug() << "Spawning Regular platform at 0:45 (frame 2730)";
    }
    if (gameTime == 3150) { // 0:52 (Beat 90)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(800, baseY - obstacle->rect().height());
        qDebug() << "Spawning Regular platform at 0:52 (frame 3150)";
    }
    if (gameTime == 3570) { // 0:59 (Beat 102)
        obstacle = new Obstacle(Obstacle::Spike);
        obstacle->setPos(800, baseY - 30);
        qDebug() << "Spawning Spike at 0:59 (frame 3570)";
    }
    if (gameTime == 3990) { // 1:06 (Beat 114)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(800, baseY - obstacle->rect().height());
        qDebug() << "Spawning Regular platform at 1:06 (frame 3990)";
    }
    if (gameTime == 4410) { // 1:13 (Beat 126)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(800, baseY - obstacle->rect().height());
        qDebug() << "Spawning Regular platform at 1:13 (frame 4410)";
    }
    if (gameTime == 4830) { // 1:20 (Beat 138)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(800, baseY - obstacle->rect().height());
        qDebug() << "Spawning Regular platform at 1:20 (frame 4830)";
    }

    // Chorus (1:30–2:30): Add JumpPads for variety
    if (gameTime == 5670) { // 1:34 (Beat 162)
        obstacle = new Obstacle(Obstacle::JumpPad);
        obstacle->setPos(800, baseY - obstacle->rect().height());
        qDebug() << "Spawning JumpPad at 1:34 (frame 5670)";
    }
    if (gameTime == 6090) { // 1:41 (Beat 174)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(800, baseY - obstacle->rect().height());
        qDebug() << "Spawning Regular platform at 1:41 (frame 6090)";
    }
    if (gameTime == 6510) { // 1:48 (Beat 186)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(800, baseY - obstacle->rect().height());
        qDebug() << "Spawning Regular platform at 1:48 (frame 6510)";
    }
    if (gameTime == 6930) { // 1:55 (Beat 198)
        obstacle = new Obstacle(Obstacle::JumpPad);
        obstacle->setPos(800, baseY - obstacle->rect().height());
        qDebug() << "Spawning JumpPad at 1:55 (frame 6930)";
    }
    if (gameTime == 7350) { // 2:02 (Beat 210)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(800, baseY - obstacle->rect().height());
        qDebug() << "Spawning Regular platform at 2:02 (frame 7350)";
    }
    if (gameTime == 7770) { // 2:09 (Beat 222)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(800, baseY - obstacle->rect().height());
        qDebug() << "Spawning Regular platform at 2:09 (frame 7770)";
    }
    if (gameTime == 8190) { // 2:16 (Beat 234)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(800, baseY - obstacle->rect().height());
        qDebug() << "Spawning Regular platform at 2:16 (frame 8190)";
    }
    if (gameTime == 8610) { // 2:23 (Beat 246)
        obstacle = new Obstacle(Obstacle::JumpPad);
        obstacle->setPos(800, baseY - obstacle->rect().height());
        qDebug() << "Spawning JumpPad at 2:23 (frame 8610)";
    }

    // Drop (2:30–3:00): Platforms with fewer spikes
    if (gameTime == 9450) { // 2:37 (Beat 270)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(800, baseY - obstacle->rect().height());
        qDebug() << "Spawning Regular platform at 2:37 (frame 9450)";
    }
    if (gameTime == 9870) { // 2:44 (Beat 282)
        obstacle = new Obstacle(Obstacle::Spike);
        obstacle->setPos(800, baseY - 30);
        qDebug() << "Spawning Spike at 2:44 (frame 9870)";
    }
    if (gameTime == 10290) { // 2:51 (Beat 294)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(800, baseY - obstacle->rect().height());
        qDebug() << "Spawning Regular platform at 2:51 (frame 10290)";
    }
    if (gameTime == 10710) { // 2:58 (Beat 306)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(800, baseY - obstacle->rect().height());
        qDebug() << "Spawning Regular platform at 2:58 (frame 10710)";
    }

    // Finale (3:00–3:52): TeleportPortals for a simple twist
    if (gameTime == 11130) { // 3:05 (Beat 318)
        obstacle = new Obstacle(Obstacle::TeleportPortal);
        obstacle->setPos(800, baseY - obstacle->rect().height());
        obstacle->setTarget(800, 460);
        qDebug() << "Spawning TeleportPortal at 3:05 (frame 11130)";
    }
    if (gameTime == 11550) { // 3:12 (Beat 330)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(800, baseY - obstacle->rect().height());
        qDebug() << "Spawning Regular platform at 3:12 (frame 11550)";
    }
    if (gameTime == 11970) { // 3:19 (Beat 342)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(800, baseY - obstacle->rect().height());
        qDebug() << "Spawning Regular platform at 3:19 (frame 11970)";
    }
    if (gameTime == 12390) { // 3:26 (Beat 354)
        obstacle = new Obstacle(Obstacle::TeleportPortal);
        obstacle->setPos(800, baseY - obstacle->rect().height());
        obstacle->setTarget(800, 460);
        qDebug() << "Spawning TeleportPortal at 3:26 (frame 12390)";
    }
    if (gameTime == 12810) { // 3:33 (Beat 366)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(800, baseY - obstacle->rect().height());
        qDebug() << "Spawning Regular platform at 3:33 (frame 12810)";
    }
    if (gameTime == 13230) { // 3:40 (Beat 378)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(800, baseY - obstacle->rect().height());
        qDebug() << "Spawning Regular platform at 3:40 (frame 13230)";
    }
    if (gameTime == 13650) { // 3:47 (Beat 390)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(800, baseY - obstacle->rect().height());
        qDebug() << "Spawning Regular platform at 3:47 (frame 13650)";
    }

    if (obstacle) {
        scene->addItem(obstacle);
        qDebug() << "Obstacle added to scene at frame " << gameTime;
    } else {
        qDebug() << "No obstacle spawned at frame " << gameTime;
    }
}*/

void NeonCityTrack::spawnObstacle(int gameTime, int &obstacleSpawnTimer) {
    if (!scene) {
        qDebug() << "Error: Scene is null in spawnObstacle";
        return;
    }

    int seconds = gameTime / 60;
    qDebug() << "Game time:" << gameTime << " Seconds:" << seconds << " Timer:" << obstacleSpawnTimer;

    Obstacle* obstacle = nullptr;
    qreal baseY = 500;
    int xOffset = 800 - (obstacleSpawnTimer % 5) * 20; // Stagger x-positions: 800, 780, 760, 740, 760, 780, ...

    // Fixed obstacle timeline for 3:52 (13,920 frames) at 104 BPM (35 frames/beat)
    // Balanced track for an average player: fewer hazards, more spacing, recovery zones

    // Intro (0:00–0:30): Simple platforms with occasional spikes, spaced out
    if (gameTime >= 70 && gameTime < 75) { // 0:01 (Beat 2)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(xOffset, baseY - obstacle->rect().height());
        qDebug() << "Spawning Regular platform at 0:01 (frame " << gameTime << ", x=" << xOffset << ")";
    }
    if (gameTime >= 210 && gameTime < 215) { // 0:03 (Beat 6)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(xOffset, baseY - obstacle->rect().height());
        qDebug() << "Spawning Regular platform at 0:03 (frame " << gameTime << ", x=" << xOffset << ")";
    }
    if (gameTime >= 350 && gameTime < 355) { // 0:05 (Beat 10)
        obstacle = new Obstacle(Obstacle::Spike);
        obstacle->setPos(xOffset, baseY - 30);
        qDebug() << "Spawning Spike at 0:05 (frame " << gameTime << ", x=" << xOffset << ")";
    }
    if (gameTime >= 490 && gameTime < 495) { // 0:08 (Beat 14)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(xOffset, baseY - obstacle->rect().height());
        qDebug() << "Spawning Regular platform at 0:08 (frame " << gameTime << ", x=" << xOffset << ")";
    }
    if (gameTime >= 630 && gameTime < 635) { // 0:10 (Beat 18)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(xOffset, baseY - obstacle->rect().height());
        qDebug() << "Spawning Regular platform at 0:10 (frame " << gameTime << ", x=" << xOffset << ")";
    }
    if (gameTime >= 770 && gameTime < 775) { // 0:12 (Beat 22)
        obstacle = new Obstacle(Obstacle::Spike);
        obstacle->setPos(xOffset, baseY - 30);
        qDebug() << "Spawning Spike at 0:12 (frame " << gameTime << ", x=" << xOffset << ")";
    }
    if (gameTime >= 910 && gameTime < 915) { // 0:15 (Beat 26)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(xOffset, baseY - obstacle->rect().height());
        qDebug() << "Spawning Regular platform at 0:15 (frame " << gameTime << ", x=" << xOffset << ")";
    }
    if (gameTime >= 1050 && gameTime < 1055) { // 0:17 (Beat 30)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(xOffset, baseY - obstacle->rect().height());
        qDebug() << "Spawning Regular platform at 0:17 (frame " << gameTime << ", x=" << xOffset << ")";
    }
    if (gameTime >= 1190 && gameTime < 1195) { // 0:19 (Beat 34)
        obstacle = new Obstacle(Obstacle::Spike);
        obstacle->setPos(xOffset, baseY - 30);
        qDebug() << "Spawning Spike at 0:19 (frame " << gameTime << ", x=" << xOffset << ")";
    }
    if (gameTime >= 1330 && gameTime < 1335) { // 0:22 (Beat 38)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(xOffset, baseY - obstacle->rect().height());
        qDebug() << "Spawning Regular platform at 0:22 (frame " << gameTime << ", x=" << xOffset << ")";
    }
    if (gameTime >= 1470 && gameTime < 1475) { // 0:24 (Beat 42)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(xOffset, baseY - obstacle->rect().height());
        qDebug() << "Spawning Regular platform at 0:24 (frame " << gameTime << ", x=" << xOffset << ")";
    }
    if (gameTime >= 1610 && gameTime < 1615) { // 0:26 (Beat 46)
        obstacle = new Obstacle(Obstacle::Spike);
        obstacle->setPos(xOffset, baseY - 30);
        qDebug() << "Spawning Spike at 0:26 (frame " << gameTime << ", x=" << xOffset << ")";
    }

    // Verse (0:30–1:30): Introduce JumpPads and MovingPlatforms, fewer spikes
    if (gameTime >= 1890 && gameTime < 1895) { // 0:31 (Beat 54)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(xOffset, baseY - obstacle->rect().height());
        qDebug() << "Spawning Regular platform at 0:31 (frame " << gameTime << ", x=" << xOffset << ")";
    }
    if (gameTime >= 2030 && gameTime < 2035) { // 0:33 (Beat 58)
        obstacle = new Obstacle(Obstacle::JumpPad);
        obstacle->setPos(xOffset, baseY - obstacle->rect().height());
        qDebug() << "Spawning JumpPad at 0:33 (frame " << gameTime << ", x=" << xOffset << ")";
    }
    if (gameTime >= 2310 && gameTime < 2315) { // 0:38 (Beat 66)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(xOffset, baseY - 80); // Slightly floating platform
        qDebug() << "Spawning Regular platform (floating) at 0:38 (frame " << gameTime << ", x=" << xOffset << ")";
    }
    if (gameTime >= 2450 && gameTime < 2455) { // 0:40 (Beat 70)
        obstacle = new Obstacle(Obstacle::Spike);
        obstacle->setPos(xOffset, baseY - 30);
        qDebug() << "Spawning Spike at 0:40 (frame " << gameTime << ", x=" << xOffset << ")";
    }
    if (gameTime >= 2730 && gameTime < 2735) { // 0:45 (Beat 78)
        obstacle = new Obstacle(Obstacle::MovingPlatform);
        obstacle->setPos(xOffset, baseY - 100);
        qDebug() << "Spawning MovingPlatform at 0:45 (frame " << gameTime << ", x=" << xOffset << ")";
    }
    if (gameTime >= 3010 && gameTime < 3015) { // 0:50 (Beat 86)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(xOffset, baseY - obstacle->rect().height());
        qDebug() << "Spawning Regular platform at 0:50 (frame " << gameTime << ", x=" << xOffset << ")";
    }
    if (gameTime >= 3290 && gameTime < 3295) { // 0:54 (Beat 94)
        obstacle = new Obstacle(Obstacle::JumpPad);
        obstacle->setPos(xOffset, baseY - obstacle->rect().height());
        qDebug() << "Spawning JumpPad at 0:54 (frame " << gameTime << ", x=" << xOffset << ")";
    }
    if (gameTime >= 3570 && gameTime < 3575) { // 0:59 (Beat 102)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(xOffset, baseY - 80);
        qDebug() << "Spawning Regular platform (floating) at 0:59 (frame " << gameTime << ", x=" << xOffset << ")";
    }
    if (gameTime >= 3850 && gameTime < 3855) { // 1:04 (Beat 110)
        obstacle = new Obstacle(Obstacle::Spike);
        obstacle->setPos(xOffset, baseY - 30);
        qDebug() << "Spawning Spike at 1:04 (frame " << gameTime << ", x=" << xOffset << ")";
    }
    if (gameTime >= 4130 && gameTime < 4135) { // 1:08 (Beat 118)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(xOffset, baseY - obstacle->rect().height());
        qDebug() << "Spawning Regular platform at 1:08 (frame " << gameTime << ", x=" << xOffset << ")";
    }
    if (gameTime >= 4410 && gameTime < 4415) { // 1:13 (Beat 126)
        obstacle = new Obstacle(Obstacle::MovingPlatform);
        obstacle->setPos(xOffset, baseY - 100);
        qDebug() << "Spawning MovingPlatform at 1:13 (frame " << gameTime << ", x=" << xOffset << ")";
    }
    if (gameTime >= 4690 && gameTime < 4695) { // 1:18 (Beat 134)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(xOffset, baseY - obstacle->rect().height());
        qDebug() << "Spawning Regular platform at 1:18 (frame " << gameTime << ", x=" << xOffset << ")";
    }
    if (gameTime >= 4970 && gameTime < 4975) { // 1:22 (Beat 142)
        obstacle = new Obstacle(Obstacle::JumpPad);
        obstacle->setPos(xOffset, baseY - obstacle->rect().height());
        qDebug() << "Spawning JumpPad at 1:22 (frame " << gameTime << ", x=" << xOffset << ")";
    }

    // Chorus (1:30–2:30): A few spikes and floating platforms, manageable pace
    if (gameTime >= 5530 && gameTime < 5535) { // 1:32 (Beat 158)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(xOffset, baseY - obstacle->rect().height());
        qDebug() << "Spawning Regular platform at 1:32 (frame " << gameTime << ", x=" << xOffset << ")";
    }
    if (gameTime >= 5810 && gameTime < 5815) { // 1:36 (Beat 166)
        obstacle = new Obstacle(Obstacle::JumpPad);
        obstacle->setPos(xOffset, baseY - obstacle->rect().height());
        qDebug() << "Spawning JumpPad at 1:36 (frame " << gameTime << ", x=" << xOffset << ")";
    }
    if (gameTime >= 6090 && gameTime < 6095) { // 1:41 (Beat 174)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(xOffset, baseY - 100); // Slightly floating
        qDebug() << "Spawning Regular platform (floating) at 1:41 (frame " << gameTime << ", x=" << xOffset << ")";
    }
    if (gameTime >= 6370 && gameTime < 6375) { // 1:46 (Beat 182)
        obstacle = new Obstacle(Obstacle::Spike);
        obstacle->setPos(xOffset, baseY - 30);
        qDebug() << "Spawning Spike at 1:46 (frame " << gameTime << ", x=" << xOffset << ")";
    }
    if (gameTime >= 6650 && gameTime < 6655) { // 1:50 (Beat 190)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(xOffset, baseY - obstacle->rect().height());
        qDebug() << "Spawning Regular platform at 1:50 (frame " << gameTime << ", x=" << xOffset << ")";
    }
    if (gameTime >= 6930 && gameTime < 6935) { // 1:55 (Beat 198)
        obstacle = new Obstacle(Obstacle::JumpPad);
        obstacle->setPos(xOffset, baseY - obstacle->rect().height());
        qDebug() << "Spawning JumpPad at 1:55 (frame " << gameTime << ", x=" << xOffset << ")";
    }
    if (gameTime >= 7210 && gameTime < 7215) { // 2:00 (Beat 206)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(xOffset, baseY - 100);
        qDebug() << "Spawning Regular platform (floating) at 2:00 (frame " << gameTime << ", x=" << xOffset << ")";
    }
    if (gameTime >= 7490 && gameTime < 7495) { // 2:04 (Beat 214)
        obstacle = new Obstacle(Obstacle::Spike);
        obstacle->setPos(xOffset, baseY - 30);
        qDebug() << "Spawning Spike at 2:04 (frame " << gameTime << ", x=" << xOffset << ")";
    }
    if (gameTime >= 7770 && gameTime < 7775) { // 2:09 (Beat 222)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(xOffset, baseY - obstacle->rect().height());
        qDebug() << "Spawning Regular platform at 2:09 (frame " << gameTime << ", x=" << xOffset << ")";
    }
    if (gameTime >= 8050 && gameTime < 8055) { // 2:14 (Beat 230)
        obstacle = new Obstacle(Obstacle::JumpPad);
        obstacle->setPos(xOffset, baseY - obstacle->rect().height());
        qDebug() << "Spawning JumpPad at 2:14 (frame " << gameTime << ", x=" << xOffset << ")";
    }
    if (gameTime >= 8330 && gameTime < 8335) { // 2:18 (Beat 238)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(xOffset, baseY - 100);
        qDebug() << "Spawning Regular platform (floating) at 2:18 (frame " << gameTime << ", x=" << xOffset << ")";
    }
    if (gameTime >= 8610 && gameTime < 8615) { // 2:23 (Beat 246)
        obstacle = new Obstacle(Obstacle::Spike);
        obstacle->setPos(xOffset, baseY - 30);
        qDebug() << "Spawning Spike at 2:23 (frame " << gameTime << ", x=" << xOffset << ")";
    }

    // Drop (2:30–3:00): Regular platforms and JumpPads, minimal hazards
    if (gameTime >= 9170 && gameTime < 9175) { // 2:32 (Beat 262)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(xOffset, baseY - obstacle->rect().height());
        qDebug() << "Spawning Regular platform at 2:32 (frame " << gameTime << ", x=" << xOffset << ")";
    }
    if (gameTime >= 9450 && gameTime < 9455) { // 2:37 (Beat 270)
        obstacle = new Obstacle(Obstacle::JumpPad);
        obstacle->setPos(xOffset, baseY - obstacle->rect().height());
        qDebug() << "Spawning JumpPad at 2:37 (frame " << gameTime << ", x=" << xOffset << ")";
    }
    if (gameTime >= 9730 && gameTime < 9735) { // 2:42 (Beat 278)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(xOffset, baseY - 80);
        qDebug() << "Spawning Regular platform (floating) at 2:42 (frame " << gameTime << ", x=" << xOffset << ")";
    }
    if (gameTime >= 10010 && gameTime < 10015) { // 2:46 (Beat 286)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(xOffset, baseY - obstacle->rect().height());
        qDebug() << "Spawning Regular platform at 2:46 (frame " << gameTime << ", x=" << xOffset << ")";
    }
    if (gameTime >= 10290 && gameTime < 10295) { // 2:51 (Beat 294)
        obstacle = new Obstacle(Obstacle::JumpPad);
        obstacle->setPos(xOffset, baseY - obstacle->rect().height());
        qDebug() << "Spawning JumpPad at 2:51 (frame " << gameTime << ", x=" << xOffset << ")";
    }
    if (gameTime >= 10570 && gameTime < 10575) { // 2:56 (Beat 302)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(xOffset, baseY - 80);
        qDebug() << "Spawning Regular platform (floating) at 2:56 (frame " << gameTime << ", x=" << xOffset << ")";
    }

    // Finale (3:00–3:52): TeleportPortals and a few spikes, end with a safe stretch
    if (gameTime >= 10920 && gameTime < 10925) { // 3:02 (Beat 312)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(xOffset, baseY - obstacle->rect().height());
        qDebug() << "Spawning Regular platform at 3:02 (frame " << gameTime << ", x=" << xOffset << ")";
    }
    if (gameTime >= 11130 && gameTime < 11135) { // 3:05 (Beat 318)
        obstacle = new Obstacle(Obstacle::TeleportPortal);
        obstacle->setPos(xOffset, baseY - obstacle->rect().height());
        obstacle->setTarget(800, 460);
        qDebug() << "Spawning TeleportPortal at 3:05 (frame " << gameTime << ", x=" << xOffset << ")";
    }
    if (gameTime >= 11410 && gameTime < 11415) { // 3:10 (Beat 326)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(xOffset, baseY - 100);
        qDebug() << "Spawning Regular platform (floating) at 3:10 (frame " << gameTime << ", x=" << xOffset << ")";
    }
    if (gameTime >= 11690 && gameTime < 11695) { // 3:14 (Beat 334)
        obstacle = new Obstacle(Obstacle::Spike);
        obstacle->setPos(xOffset, baseY - 30);
        qDebug() << "Spawning Spike at 3:14 (frame " << gameTime << ", x=" << xOffset << ")";
    }
    if (gameTime >= 11970 && gameTime < 11975) { // 3:19 (Beat 342)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(xOffset, baseY - obstacle->rect().height());
        qDebug() << "Spawning Regular platform at 3:19 (frame " << gameTime << ", x=" << xOffset << ")";
    }
    if (gameTime >= 12250 && gameTime < 12255) { // 3:24 (Beat 350)
        obstacle = new Obstacle(Obstacle::JumpPad);
        obstacle->setPos(xOffset, baseY - obstacle->rect().height());
        qDebug() << "Spawning JumpPad at 3:24 (frame " << gameTime << ", x=" << xOffset << ")";
    }
    if (gameTime >= 12530 && gameTime < 12535) { // 3:28 (Beat 358)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(xOffset, baseY - 80);
        qDebug() << "Spawning Regular platform (floating) at 3:28 (frame " << gameTime << ", x=" << xOffset << ")";
    }
    if (gameTime >= 12810 && gameTime < 12815) { // 3:33 (Beat 366)
        obstacle = new Obstacle(Obstacle::Spike);
        obstacle->setPos(xOffset, baseY - 30);
        qDebug() << "Spawning Spike at 3:33 (frame " << gameTime << ", x=" << xOffset << ")";
    }
    if (gameTime >= 13090 && gameTime < 13095) { // 3:38 (Beat 374)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(xOffset, baseY - obstacle->rect().height());
        qDebug() << "Spawning Regular platform at 3:38 (frame " << gameTime << ", x=" << xOffset << ")";
    }
    if (gameTime >= 13370 && gameTime < 13375) { // 3:42 (Beat 382)
        obstacle = new Obstacle(Obstacle::JumpPad);
        obstacle->setPos(xOffset, baseY - obstacle->rect().height());
        qDebug() << "Spawning JumpPad at 3:42 (frame " << gameTime << ", x=" << xOffset << ")";
    }
    if (gameTime >= 13650 && gameTime < 13655) { // 3:47 (Beat 390)
        obstacle = new Obstacle(Obstacle::Regular);
        obstacle->setPos(xOffset, baseY - obstacle->rect().height());
        qDebug() << "Spawning Regular platform at 3:47 (frame " << gameTime << ", x=" << xOffset << ")";
    }

    if (obstacle) {
        scene->addItem(obstacle);
        qDebug() << "Obstacle added to scene at frame " << gameTime;
    } else {
        qDebug() << "No obstacle spawned at frame " << gameTime;
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
    return 0; // 0.5 секунды при 60 FPS
}
