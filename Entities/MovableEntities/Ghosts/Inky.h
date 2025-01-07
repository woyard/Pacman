#pragma once
#include "../Ghost.h"

class Inky : public Ghost {
public:
    Inky(QPoint pos, int tileSize, PathFinder& pathFinder) :
    Ghost(pos, tileSize, pathFinder, Qt::cyan) {
        m_name = GhostName::INKY;
        m_aggression = 0.50;
    }

    void processPathFinding(QPoint playerPosition, QPoint oppositePlayerPosition, QPoint playerDirection, QPoint m_ghostRespawnPos) override{
            switch (m_state) {
                case GhostState::CHASE:
                    getAndFollowPath(playerPosition - playerDirection * 6);
                    break;
                case GhostState::SCARED:
                    getAndFollowPath(oppositePlayerPosition);
                    break;
                case GhostState::DEAD:
                case GhostState::IDLE:
                    if (arePointsClose(getGridPos(), m_originalPos)) {
                        setPos(m_originalPos * m_tileSize);
                        setVelocity(QPoint(0, 0));
                    } else {
                        getAndFollowPath(m_originalPos);
                    }
                    break;
                default:
                    break;
            }
        }

};
