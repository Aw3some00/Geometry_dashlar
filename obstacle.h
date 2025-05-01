#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QGraphicsRectItem>
#include <QGraphicsPolygonItem>

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
    float getSpeedMultiplier() const { return speedChange; }
    void setSpeedMultiplier(float multiplier);

    void setTarget(qreal x, qreal y);
    Obstacle(Type type, QGraphicsItem *parent = nullptr);
    void update();
    void activate();

    Type getType() const { return type; }
    float getJumpForce() const { return jumpForce; }
    float getSpeedChange() const { return speedChange; }
    float getSpeed = 2;
    bool isSafeToStandOn() const { return isSafe; }
    bool reverseGravity() { return reversesGravity; }
    QPointF getTeleportTarget() const { return QPointF(targetX, targetY); }
    QGraphicsPolygonItem* getSpikePolygon() const { return spikePolygon; } // Новый метод
    float getTargetX() { return targetX; }
    float getTargetY() { return targetY; }
    float getSpeedMultiplier() { return getSpeed; }
    bool isPortal() const;

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

private:
    QGraphicsPolygonItem* spikePolygon = nullptr; // Новое поле для полигона Spike
};

#endif // OBSTACLE_H
