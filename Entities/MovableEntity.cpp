#include "MovableEntity.h"

bool MovableEntity::isHardColliding(QVector<EntityType> &collisions) {
    for (EntityType type: collisions){
        if (hardCollisionEntities.contains(type)) {
            return true;
        }
    }
    return false;
}

QVector<EntityType> MovableEntity::processGoingStraight(const std::function<QVector<EntityType>(Entity *)> &getCollisions) {
    m_requestedPos = m_currentPos + m_velocity;
    setPos(m_requestedPos);
    auto collisions = getCollisions(this);
    if (isHardColliding(collisions)) { // old velocity is blocked
        setPos(m_currentPos);
        m_isBlocked = true;
    } else { // flying freely
        m_currentPos = m_requestedPos;
        m_isBlocked = false;
    }
    return collisions;
}

QVector<EntityType>
MovableEntity::processVelocityChange(const std::function<QVector<EntityType>(Entity *)> &getCollisions) {
    m_requestedPos = m_currentPos + m_newVelocity;
    setPos(m_requestedPos);
    auto collisions = getCollisions(this);
    if (isHardColliding(collisions)) { // new velocity is blocked
        m_requestedPos = m_currentPos + m_velocity;
        setPos(m_requestedPos);
        collisions = getCollisions(this);
        if (isHardColliding(collisions)) { // old velocity is blocked too
            m_newVelocity = QPoint(0, 0);
            setPos(m_currentPos);
            m_isBlocked = true;
        } else { // - new velocity is blocked, but old is not - going by "inertia"
            m_isBlocked = false;
        }
    } else { // new velocity is not blocked
        m_velocity = m_newVelocity;
        m_newVelocity = QPoint(0, 0);
    }
    return collisions;
}

void MovableEntity::processMovement(const std::function<QVector<EntityType>(Entity *)> &getCollisions) {
    prepareGeometryChange();
    m_currentPos = pos().toPoint();
    QVector<EntityType> collisions;
    if (m_newVelocity == QPoint(0, 0)){ // no new velocity
        collisions = processGoingStraight(getCollisions);
    } else {
        collisions = processVelocityChange(getCollisions);
    }
    for (EntityType type: collisions) {
        processCollision(type);
    }
}

void MovableEntity::checkForOutOfBounds(QPoint n_tiles) {
    QPoint levelSize = (n_tiles-QPoint(1,1)) * m_tileSize;
    if (pos().x() < 0) { setPos(levelSize.x(), pos().y()); }
    else if (pos().x() > levelSize.x()) { setPos(0, pos().y()); }
    if (pos().y() < 0) { setPos(pos().x(), levelSize.y()); }
    else if (pos().y() > levelSize.y()) { setPos(pos().x(), 0); }
}
