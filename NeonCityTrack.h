#ifndef NEONCITYTRACK_H
#define NEONCITYTRACK_H

#include <QGraphicsScene>
#include "player.h"

#include "obstacle.h"
#include <QGraphicsPixmapItem>

class NeonCityTrack {
public:
    NeonCityTrack(QGraphicsScene *scene);
    void setupTrack();
    void spawnObstacle(int gameTime, int &obstacleSpawnTimer);
    // qreal getAudioAmplitude(int time) const;
    int getSpawnInterval() const;


private:
    QGraphicsScene *scene;
    int spawnInterval = 34; // 105 BPM
    QGraphicsPixmapItem *backgroundItem; // Для хранения фона
    void setupBackground();
    void setupGround();

    qreal getAudioAmplitude(int timer);
};

#endif // NEONCITYTRACK_H
