#pragma once
#include <QDebug>
#include "Entity.h"
#include "../LevelUtilities/LevelData.h"

#define MOVABLE_ENTITY_DEFAULT_STEPS_PER_TILE 8

class MovableEntity : public Entity {
public:
    MovableEntity(QPoint pos, int tileSize) : Entity(pos, tileSize), m_originalPos(pos) {}

    void setHardCollisionEntities(QVector<EntityType> entities) { hardCollisionEntities = std::move(entities); }

    void setVelocity(QPoint velocity) { m_newVelocity = velocity * m_speed; direction = velocity; }

    QPoint getDirection() const { return direction; }

    bool isHardColliding(QVector<EntityType>& collisions);

    QVector<EntityType> processGoingStraight(const std::function<QVector<EntityType>(Entity*)>& getCollisions);

    QVector<EntityType> processVelocityChange(const std::function<QVector<EntityType>(Entity*)>& getCollisions);

    void processMovement(const std::function<QVector<EntityType>(Entity*)>& getCollisions) override;

    void checkForOutOfBounds(QPoint levelSize) override;

protected:
    QPoint m_originalPos;

    QVector<EntityType> hardCollisionEntities; // ghosts can toggle door collision, so not static

    int m_speed = m_tileSize / MOVABLE_ENTITY_DEFAULT_STEPS_PER_TILE;

    QPoint direction;
    QPoint m_velocity;
    QPoint m_newVelocity;

    QPoint m_currentPos;
    QPoint m_requestedPos;

    bool m_isBlocked = false;
};
