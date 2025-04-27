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
   float verticalSpeed = 0;
 float getVerticalSpeed(){return verticalSpeed;}
    void resetVerticalSpeed(){verticalSpeed=0;};
    void setOnGround(bool on) { isOnGround = on; }
    bool isOnGroundf() const { return isOnGround; }
     bool isOnGround= true;
private:


    qreal gravity = 0.8;
    qreal speedMultiplier = 1.0;
};

#endif // PLAYER_H
