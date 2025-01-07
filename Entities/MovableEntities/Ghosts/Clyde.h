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
                case GhostState::SCARED_FLEEING:
                    getAndFollowPath(ghostRespawnPos);
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
