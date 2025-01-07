#pragma once
#include "../Entity.h"
#include <QDebug>

class Pellet : public Entity {
public:
    Pellet(QPoint pos, int tileSize) : Entity(pos, tileSize) {}

    EntityType getType() override { return EntityType::PELLET; }

    void processCollision(EntityType other) override {
        if (other == EntityType::PLAYER) {
            //qDebug() << "Pellet collided with a player!";
            this->kill();
        }
    }

    QRectF boundingRect() const override {
        return QRect(m_tileSize / 4, m_tileSize / 4, m_tileSize / 2, m_tileSize / 2);
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
        painter->setBrush(Qt::white);
        painter->drawEllipse(m_tileSize / 2 - m_tileSize / 8, m_tileSize / 2 - m_tileSize / 8, m_tileSize / 4, m_tileSize / 4);
    }

};
