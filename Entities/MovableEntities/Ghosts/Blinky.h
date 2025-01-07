#pragma once
#include "../Ghost.h"

class Blinky : public Ghost {
public:
    Blinky(QPoint pos, int tileSize, PathFinder& pathFinder) :
    Ghost(pos, tileSize, pathFinder, Qt::red) {
        m_name = GhostName::BLINKY;
        m_aggression = 0.0;
    }

    void processPathFinding(QPoint playerPosition, QPoint oppositePlayerPosition, QPoint playerDirection, QPoint ghostRespawnPos) override{
            switch (m_state) {
                case GhostState::CHASE:
                    getAndFollowPath(playerPosition);
                    break;
                case GhostState::SCARED:
                    getAndFollowPath(oppositePlayerPosition);
                    break;
                case GhostState::DEAD:
                    if (arePointsClose(getGridPos(), m_originalPos)) {
                        setPos(m_originalPos * m_tileSize);
                        setVelocity(QPoint(0, 0));
                    } else {
                        getAndFollowPath(m_originalPos);
                    }
                case GhostState::IDLE:
                    setVelocity(playerDirection);
                    break;
                default:
                    break;
            }
        }

};
