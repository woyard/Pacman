#pragma once

#include "../Entity.h"

class Powerup : public Entity {
public:
    Powerup(QPoint pos, int tileSize) : Entity(pos, tileSize) {}

    EntityType getType() override { return EntityType::POWERUP; }

    void processCollision(EntityType other) override {
        if (other == EntityType::PLAYER) {
            //qDebug() << "Powerup collided with a player!";
            this->markForRemoval();
        }
    }

    QRectF boundingRect() const override {
        return QRect(m_tileSize / 4, m_tileSize / 4, m_tileSize / 2, m_tileSize / 2);
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
        painter->setBrush(Qt::white);
        painter->drawEllipse(m_tileSize / 2 - m_tileSize / 4, m_tileSize / 2 - m_tileSize / 4, m_tileSize / 2, m_tileSize / 2);
    }

};