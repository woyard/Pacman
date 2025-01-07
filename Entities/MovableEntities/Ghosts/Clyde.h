#pragma once
#include "../Ghost.h"

class Clyde : public Ghost {
public:
    Clyde(QPoint pos, int tileSize, PathFinder& pathFinder) :
    Ghost(pos, tileSize, pathFinder, QColor(255, 165, 0)) {
        m_name = GhostName::CLYDE;
        m_aggression = 0.125;
    }

    void processPathFinding(QPoint playerPosition, QPoint oppositePlayerPosition, QPoint playerDirection, QPoint ghostRespawnPos) override{
            switch (m_state) {
                case GhostState::CHASE:
                    getAndFollowPath(oppositePlayerPosition);
                    break;
                case GhostState::SCARED:
                    getAndFollowPath(ghostRespawnPos);
                    break;
                case GhostState::DEAD:
                    if (arePointsClose(getGridPos(), m_originalPos)) {
                        setPos(m_originalPos * m_tileSize);
                        setVelocity(QPoint(0, 0));
                    } else {
                        getAndFollowPath(m_originalPos);
                    }
                    break;
                case GhostState::IDLE:
                    setVelocity(playerDirection);
                    break;
                default:
                    break;
            }
        }

};
