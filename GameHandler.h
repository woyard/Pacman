#pragma once

#include <QGraphicsScene>
#include <QGraphicsView>

#include "Entities/Entity.h"
#include "LevelUtilities/LevelData.h"
#include "LevelUtilities/tile_definitions.h"
#include "Entities/Scoreboard.h"
#include "PathFinder/PathFinder.h"
#include "Entities/MovableEntities/Player.h"
#include "Entities/MovableEntities/Ghost.h"
#include "Entities/StaticEntities/Door.h"
#include "Entities/StaticEntities/Pellet.h"
#include "Entities/StaticEntities/MazeWalls.h"
#include "Entities/StaticEntities/Powerup.h"

#define SCOREBOARD_TILE_HEIGHT 2

class GameHandler {
private:
    const LevelData &m_levelData;
    QGraphicsScene *m_scene = nullptr;
    QGraphicsView *m_view = nullptr;
    int m_tileSize;
    int m_numPellets = 0;
    Scoreboard m_scoreboard;
    PathFinder m_pathFinder;
    QVector<QGraphicsEllipseItem*> m_pathEllipses;

    EntityVector m_staticEntities;
    EntityVector m_movableEntities;
    Player *m_player = nullptr;
    QVector<Ghost*> m_ghosts;
    MazeWalls *m_mazeWalls = nullptr;
    QPoint m_ghostRespawnPos = QPoint(0, 0);

public:
    GameHandler(const LevelData &levelData, int tileSize) :
    m_levelData(levelData),
    m_tileSize(tileSize),
    m_scoreboard(QPoint(m_levelData.width * m_tileSize, m_tileSize * SCOREBOARD_TILE_HEIGHT)),
    m_pathFinder(levelData)
    {
        loadLevel();
    }

    void loadLevel();

    int getHighscore() {
        return std::max(m_levelData.highscore, m_player->getScore());
    }

    QVector<EntityType> getCollisions(Entity* entity);

    QPoint getOpposite(QPoint pos) const { return {m_levelData.width - pos.x(), m_levelData.height - pos.y()}; }

    void updateGhosts();

    void update();

    void reloadLevel();

    void resetDrawnPath();

    void drawPath(const QColor& color);

    QGraphicsScene* buildScene();

    QGraphicsView* buildView(QGraphicsScene *scene);
};
