#pragma once

#include <QDebug>
#include "../Entity.h"
class Door : public Entity {
public:
    Door(QPoint pos, int tileSize) : Entity(pos, tileSize) {}

    EntityType getType() override { return EntityType::DOOR; }

    void processCollision(EntityType other) override { }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
        painter->setBrush(QColor(85, 62, 42));
        painter->drawRect(0, 0, m_tileSize, m_tileSize);
    }
};
