#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QGraphicsRectItem>

class Obstacle : public QGraphicsRectItem {
public:
    enum Type {
        Regular,
        MovingPlatform,
        Sawblade,
        DisappearingBlock,
        TeleportPortal,
        Spike,
        JumpPad,
        GravityPortal,
        SpeedPortal,
        BounceBlock
    };

    Obstacle(Type type, QGraphicsItem *parent = nullptr);
    void update();
    void activate();

    Type getType() const { return type; }
    float getJumpForce() const { return jumpForce; }
    float getSpeedChange() const { return speedChange; }
    float getSpeed=2;
    bool isSafeToStandOn() const { return isSafe; }
    bool reverseGravity(){return reversesGravity;}
    QPointF getTeleportTarget() const { return QPointF(targetX, targetY); }
    Type type;
    bool isSafe = false;
    int verticalSpeed = 0;
    int toggleTimer = 0;
    int pulseTimer = 0;
    int activationTimer = 0;
    bool isVisible = true;
    bool activated = false;
    bool reversesGravity = false;
    float rotationAngle = 0;
    float jumpForce = 0;
    float speedChange = 1.0f;
    float targetX = 0;
    float targetY = 0;
    float getTargetX(){
        return targetX;
    }
    float getTargetY(){
        return targetY;
    }
    float getSpeedMultiplier(){
        return getSpeed;
    }
    bool isPortal() const;

};

#endif // OBSTACLE_H
