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
                case GhostState::SCARED_FLEEING:
                    getAndFollowPath(oppositePlayerPosition);
                    break;
                case GhostState::DEAD:
                    processPostMortemBehaviour();
                    break;
                case GhostState::IDLE:
                case GhostState::SCARED_IDLE:
                    setVelocity(playerDirection*(-1));
                    break;
                default:
                    break;
            }
        }

};
