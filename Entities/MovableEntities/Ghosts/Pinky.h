#pragma once
#include "../Ghost.h"

class Pinky : public Ghost {
public:
    Pinky(QPoint pos, int tileSize, PathFinder& pathFinder) :
    Ghost(pos, tileSize, pathFinder, Qt::magenta) {
        m_name = GhostName::PINKY;
        m_aggression = 0.25;
    }

    void processPathFinding(QPoint playerPosition, QPoint oppositePlayerPosition, QPoint playerDirection, QPoint ghostRespawnPos) override{
            switch (m_state) {
                case GhostState::CHASE:
                    getAndFollowPath(playerPosition + playerDirection * 6);
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
                    break;
                case GhostState::IDLE:
                    setVelocity(playerDirection);
                    break;
                default:
                    break;
            }
        }

};
