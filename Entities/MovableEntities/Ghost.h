#pragma once
#include "../MovableEntity.h"
#include "../../PathFinder/PathFinder.h"
#include "../../LevelUtilities/tile_definitions.h"
#include <QDebug>
#include <utility>

class Ghost : public MovableEntity {
public:
    enum class GhostState {
        IDLE,
        CHASE,
        SCARED_FLEEING,
        SCARED_IDLE,
        DEAD
    };
    enum class GhostName {
        BLINKY,
        PINKY,
        INKY,
        CLYDE,
        DEFAULT_GHOST
    };
protected:
    QColor m_color;
    GhostName m_name = GhostName::DEFAULT_GHOST;
    float m_aggression = 0.0;
    GhostState m_state = GhostState::IDLE;
    int m_deathTimer = 0;

    PathFinder& m_pathFinder;
    QVector<QPoint> m_path;
    QPoint m_nextPos;
    QPoint m_lastPos;
public:
    Ghost(QPoint pos, int tileSize, PathFinder& pathFinder, QColor color) : MovableEntity(pos, tileSize), m_pathFinder(pathFinder), m_color(std::move(color)) {
        setHardCollisionEntities({EntityType::WALL, EntityType::DOOR, EntityType::LIVE_GHOST});
        m_speed = tileSize / 8;
        m_nextPos = getGridPos();
    }

    EntityType getType() override { return m_state == GhostState::DEAD ? EntityType::DEAD_GHOST : EntityType::LIVE_GHOST; }

    QColor getColor() const { return m_color; }

    GhostName getName() { return m_name; }

    float getAggression() const { return m_aggression; }

    void setGhostState(GhostState state);

    QVector<char> getColliderChars();

    const QVector<QPoint>& getPath() const {return m_path;}

    void tickDeathTimer();

    void teleportHome();

    void processCollision(EntityType other) override;

    QPoint calculateNewVelocity(QPoint nextTile);

    void processPostMortemBehaviour();

    void getAndFollowPath(QPoint destination);

    static bool arePointsClose(QPoint a, QPoint b) {
        return abs(a.x() - b.x()) <= 1 && abs(a.y() - b.y()) <= 1;
    }

    virtual void processPathFinding(QPoint playerPosition, QPoint oppositePlayerPosition, QPoint playerDirection, QPoint m_ghostRespawnPos) = 0;

    void scare() {
        if (m_state == GhostState::IDLE || m_state == GhostState::CHASE) {
            setGhostState(GhostState::SCARED_FLEEING);
        }
    }

    void relax() {
        if (m_state == GhostState::DEAD) { return; }
        setGhostState(GhostState::IDLE);
    }

    void aggravate() {
        if (m_state != GhostState::IDLE) { return; }
        setGhostState(GhostState::CHASE);
    }

    void give_up() {
        if (m_state == GhostState::SCARED_FLEEING) {
            setGhostState(GhostState::SCARED_IDLE);
        }
    }

    QRectF boundingRect() const override {
        return QRectF(0, 0, m_tileSize, m_tileSize);
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};
