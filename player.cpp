#include "player.h"
#include <QBrush>

Player::Player(QGraphicsItem *parent) : QGraphicsRectItem(0, 0, 40, 40, parent) {
    setBrush(QBrush(Qt::blue));
}

void Player::jump() {
    if (isOnGround) {
        verticalSpeed = -15 * (gravity > 0 ? 1 : -1);
        isOnGround = false;
    }
}

void Player::update() {
    verticalSpeed += gravity * speedMultiplier;
    setPos(x(), y() + verticalSpeed);

    if (gravity > 0) {
        if (y() >= 460) { // Adjusted to sit on gray surface (500 - 40)
            setPos(x(), 460);
            verticalSpeed = 0;
            isOnGround = true;
        }
    } else {
        if (y() <= 0) {
            setPos(x(), 0);
            verticalSpeed = 0;
            isOnGround = true;
        }
    }
}

void Player::invertGravity() {
    gravity = -gravity;
    verticalSpeed = 0;
    isOnGround = false;
}

void Player::setSpeedMultiplier(qreal multiplier) {
    speedMultiplier = multiplier;
}

void Player::teleportTo(qreal x, qreal y) {
    setPos(x, y);
    verticalSpeed = 0;
    isOnGround = false;
}
