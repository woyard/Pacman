#pragma once
#include "../Ghost.h"

class Inky : public Ghost {
public:
    Inky(QPoint pos, int tileSize, PathFinder& pathFinder) :
    Ghost(pos, tileSize, pathFinder, Qt::cyan) {
        m_name = GhostName::INKY;
        m_aggression = 0.50;
    }

    void processPathFinding(QPoint playerPosition, QPoint oppositePlayerPosition, QPoint playerDirection, QPoint ghostRespawnPos) override{
            switch (m_state) {
                case GhostState::CHASE:
                    getAndFollowPath(playerPosition - playerDirection * 6);
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
