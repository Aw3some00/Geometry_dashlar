#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsRectItem>

class Player : public QGraphicsRectItem {
public:
    explicit Player(QGraphicsItem *parent = nullptr);
    void jump();
    void update();
    void invertGravity();
    void setSpeedMultiplier(qreal multiplier);
    void teleportTo(qreal x, qreal y);
    qreal verticalSpeed = 0;
    void resetVerticalSpeed(){verticalSpeed=0;};
private:

    bool isOnGround = true;
    qreal gravity = 0.8;
    qreal speedMultiplier = 1.0;
};

#endif // PLAYER_H
