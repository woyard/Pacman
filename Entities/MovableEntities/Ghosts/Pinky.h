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
