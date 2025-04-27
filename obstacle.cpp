#include "obstacle.h"
#include <QBrush>
#include <QPen>
#include <QGraphicsPixmapItem>
#include <QTimer>

Obstacle::Obstacle(Type type, QGraphicsItem *parent) : QGraphicsRectItem(parent), type(type) {
    switch (type) {
    case Regular:
        setRect(0, 0, 60, 20);
        setBrush(QBrush(Qt::red));
        isSafe = true;
        break;

    case MovingPlatform:
        setRect(0, 0, 60, 20);
        setBrush(QBrush(Qt::green));
        verticalSpeed = 2;
        break;

    case Sawblade: {
        QGraphicsEllipseItem *blade = new QGraphicsEllipseItem(0, 0, 30, 30, this);
        blade->setBrush(QBrush(Qt::gray));
        setRect(0, 0, 30, 30);
        break;
    }

    case DisappearingBlock:
        setRect(0, 0, 40, 40);
        setBrush(QBrush(Qt::magenta));
        break;

    case TeleportPortal:
        setRect(0, 0, 20, 60);
        setBrush(QBrush(Qt::cyan));
        targetX = 800;
        targetY = 460;
        break;

    case Spike: {
        QPolygonF spikePolygon;
        spikePolygon << QPointF(15, 0) << QPointF(0, 30) << QPointF(30, 30);
        QGraphicsPolygonItem *spike = new QGraphicsPolygonItem(spikePolygon, this);
        spike->setBrush(QBrush(Qt::red));
        setRect(0, 0, 0, 0);
        break;
    }

    case JumpPad:
        setRect(0, 0, 40, 10);
        setBrush(QBrush(QColor(255, 165, 0))); // Оранжевый
        jumpForce = 15;
        break;

    case GravityPortal:
        setRect(0, 0, 20, 60);
        setBrush(QBrush(Qt::yellow));
        reversesGravity = true;
        break;

    case SpeedPortal:
        setRect(0, 0, 20, 60);
        setBrush(QBrush(QColor(138, 43, 226))); // Фиолетовый
        speedChange = 1.5f;
        break;

    case BounceBlock:
        setRect(0, 0, 40, 40);
        setBrush(QBrush(QColor(0, 191, 255))); // Голубой
        break;
    }
}

void Obstacle::update() {
    switch (type) {
    case MovingPlatform: {
        setPos(x(), y() + verticalSpeed);
        if (y() > 500 || y() < 300) {
            verticalSpeed = -verticalSpeed;
        }
        break;
    }
    case Sawblade: {
        rotationAngle += 5;
        setRotation(rotationAngle);
        break;
    }
    case DisappearingBlock: {
        toggleTimer++;
        if (toggleTimer >= 60) {
            isVisible = !isVisible;
            setVisible(isVisible);
            toggleTimer = 0;
        }
        break;
    }
    case Spike: {
        // Анимация пульсации
        pulseTimer++;
        if (pulseTimer % 20 == 0) {
            setOpacity(opacity() == 1.0 ? 0.7 : 1.0);
        }
        break;
    }
    case JumpPad: {
        // Мигание при активации
        if (activated) {
            activationTimer++;
            if (activationTimer > 10) {
                activated = false;
                activationTimer = 0;
                setBrush(QBrush(QColor(255, 165, 0)));
            }
        }
        break;
    }
    case Regular:
    case TeleportPortal:
    case GravityPortal:
    case SpeedPortal:
    case BounceBlock:

        break;
    }
}

void Obstacle::activate() {
    if (type == JumpPad) {
        activated = true;
        setBrush(QBrush(Qt::white));
        activationTimer = 0;
    }
}
bool Obstacle::isPortal() const {
    return
        type == GravityPortal ;

}
