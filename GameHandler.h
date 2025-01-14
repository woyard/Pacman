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
    bool m_pathFindingDebugOn;
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
    GameHandler(const LevelData &levelData, int tileSize, bool pathFindingDebugOn = false) :
    m_levelData(levelData),
    m_tileSize(tileSize),
    m_pathFindingDebugOn(pathFindingDebugOn),
    m_scoreboard(QPoint(m_levelData.width * m_tileSize, m_tileSize * SCOREBOARD_TILE_HEIGHT)),
    m_pathFinder(levelData)
    {
        loadLevel();
    }

    void loadLevel();

    int getHighscore() {
        return std::max(m_levelData.highscore, m_player->getScore());
    }

    void spawnRandomExtraPellet();

    QVector<EntityType> getCollisions(Entity* entity);

    QPoint getOpposite(QPoint pos) const { return {m_levelData.width - pos.x(), m_levelData.height - pos.y()}; }

    void updateGhosts();

    void update();

    void reloadLevel();

    void drawGhostPaths();

    void drawPath(const QColor& color, const QVector<QPoint>& path);

    void resetDrawnPaths();

    QGraphicsScene* buildScene();

    QGraphicsView* buildView(QGraphicsScene *scene);

    class MissingPlayerException : public std::exception {
    public:
        explicit MissingPlayerException(QString  message) : m_message(std::move(message)) {}
        const char* what() const noexcept override {
            return m_message.toUtf8().constData();
        }
    private:
        QString m_message;
    };
};

