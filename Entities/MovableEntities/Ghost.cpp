#include "Ghost.h"

QPoint Ghost::calculateNewVelocity(QPoint nextTile) {
    QPoint newVelocity = nextTile - getGridPos();
    if (newVelocity.x() > 1) { newVelocity.setX(-1); }
    else if (newVelocity.x() < -1) { newVelocity.setX(1); }
    if (newVelocity.y() > 1) { newVelocity.setY(-1); }
    else if (newVelocity.y() < -1) { newVelocity.setY(1); }
    return newVelocity;
}

void Ghost::processPostMortemBehaviour() {
    if (arePointsClose(getGridPos(), m_originalPos)) {
        setPos(m_originalPos * m_tileSize);
        setVelocity(QPoint(0, 0));
    } else {
        getAndFollowPath(m_originalPos);
    }
}

void Ghost::getAndFollowPath(QPoint destination) {
    bool hasNewPath = false;
    if (m_path.empty()) {
        m_pathFinder.setWaypoints(getGridPos(), destination);
        m_pathFinder.setColliderChars(getColliderChars());
        m_path = m_pathFinder.getPath();
        if (m_path.empty()) {
            give_up();
            return;
        }
        hasNewPath = true;
    }
    if ((m_nextPos == getGridPos() && m_lastPos != getGridPos()) || hasNewPath) {
        m_lastPos = getGridPos();
        while (!m_path.empty()){
            m_nextPos = m_path.takeLast();
            QPoint newVelocity = calculateNewVelocity(m_nextPos);
            if (newVelocity != QPoint(0, 0)){
                setVelocity(newVelocity);
                break;
            }
        }
    } else if (m_isBlocked) {
        m_path.clear();
        setPos( getGridPos() * m_tileSize);
    }
}

void Ghost::setGhostState(Ghost::GhostState state) {
    if (m_state == state) { return; }
    switch (state) {
        case GhostState::SCARED_IDLE:
        case GhostState::SCARED_FLEEING:
            m_speed = m_tileSize / 16;
            setHardCollisionEntities({EntityType::WALL, EntityType::DOOR});
            break;
        case GhostState::CHASE:
            m_speed = m_tileSize / 8;
            setHardCollisionEntities({EntityType::WALL});
            break;
        case GhostState::DEAD:
            m_speed = m_tileSize / 8;
            setHardCollisionEntities({});
            break;
        default:
            m_speed = m_tileSize / 8;
            setHardCollisionEntities({EntityType::WALL, EntityType::DOOR});
    }
    m_state = state;
}

QVector<char> Ghost::getColliderChars() {
    switch (m_state) {
        case GhostState::CHASE:
            return {WALL_CHAR};
        case GhostState::DEAD:
            return {WALL_CHAR};
        default:
            return {WALL_CHAR, DOOR_CHAR};
    }
}


void Ghost::tickDeathTimer() {
    if (m_state == GhostState::DEAD) {
        m_deathTimer++;
        if (m_deathTimer > 500) {
            teleportHome();
            m_deathTimer = 0;
        }
    }
}

void Ghost::processCollision(EntityType other) {
    switch (other) {
        case EntityType::PLAYER:
            if (m_state == GhostState::SCARED_FLEEING || m_state == GhostState::SCARED_IDLE) {
                setGhostState(GhostState::DEAD);
            }
            break;
        default:
            break;
    }
}

void Ghost::teleportHome() {
    setGhostState(GhostState::IDLE);
    setPos(m_originalPos * m_tileSize);
    m_nextPos = getGridPos();
    m_lastPos = getGridPos();
    m_path.clear();
}

void Ghost::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    switch (m_state) {
        case GhostState::SCARED_FLEEING:
        case GhostState::SCARED_IDLE:
            painter->setBrush(Qt::blue);
            break;
        case GhostState::DEAD:
            painter->setBrush(Qt::gray);
            break;
        default:
            painter->setBrush(m_color);
            break;
    }
    painter->drawRect(0, 0, m_tileSize, m_tileSize);
}