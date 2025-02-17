#include <qcoreapplication.h>
#include <QRandomGenerator>
#include "GameHandler.h"
#include "Entities/MovableEntities/Ghosts/Blinky.h"
#include "Entities/MovableEntities/Ghosts/Pinky.h"
#include "Entities/MovableEntities/Ghosts/Inky.h"
#include "Entities/MovableEntities/Ghosts/Clyde.h"

Entity* addGhostReturn(QVector<Ghost*>& vector, Ghost* entity) {
    vector.append(entity);
    return entity;
}

void GameHandler::loadLevel() {
    m_staticEntities.addEntity(m_mazeWalls = new MazeWalls(QPoint(0, 0), m_tileSize, m_levelData));
    for (int y = 0; y < m_levelData.height; ++y) {
        for (int x = 0; x < m_levelData.width; ++x) {
            QPoint pos(x, y);
            switch (m_levelData.tiles[y][x]) {
                case DOOR_CHAR:
                    m_staticEntities.addEntity(new Door(pos, m_tileSize));
                    break;
                case PLAYER_CHAR:
                    m_movableEntities.addEntity(m_player = new Player(pos, m_tileSize));
                    break;
                case BLINKY_CHAR:
                    m_movableEntities.addEntity(addGhostReturn(m_ghosts, new Blinky(pos, m_tileSize, m_pathFinder)));
                    break;
                case PINKY_CHAR:
                    m_movableEntities.addEntity(addGhostReturn(m_ghosts, new Pinky(pos, m_tileSize, m_pathFinder)));
                    break;
                case INKY_CHAR:
                    m_movableEntities.addEntity(addGhostReturn(m_ghosts, new Inky(pos, m_tileSize, m_pathFinder)));
                    break;
                case CLYDE_CHAR:
                    m_movableEntities.addEntity(addGhostReturn(m_ghosts, new Clyde(pos, m_tileSize, m_pathFinder)));
                    break;
                case GHOST_RESPAWN_CHAR:
                    m_ghostRespawnPos = pos;
                    break;
                case POWERUP_CHAR:
                    m_staticEntities.addEntity(new Powerup(pos, m_tileSize));
                    break;
                case PELLET_CHAR:
                    m_staticEntities.addEntity(new Pellet(pos, m_tileSize));
                    m_numPellets += 1;
                    break;
            }
        }
    }
    if (m_player == nullptr) {
        throw MissingPlayerException(QString("no player could be created: PLAYER_CHAR '%1' not found in level data").arg(PLAYER_CHAR));
    }
    qDebug() <<"m_numPellets: " << m_numPellets;
}

QVector<EntityType> GameHandler::getCollisions(Entity *entity) {
    // note: this triggers for normal but also "probing collisions"
    // (the ones the movement system uses to evaluate if a move is valid)
    // I go around this by processing the probing collisions as if they happened for real
    QVector<EntityType> collisions;
    QVector<Entity*> entitiesToRemove;
    for (auto other : m_staticEntities) {
        if (other != entity && other->collidesWithItem(entity)) {
            collisions.push_back(other->getType());
            other->processCollision(entity->getType());
        }
        if (other->isForRemoval()) {
            entitiesToRemove.push_back(other);
        }
    }
    for (auto entityToRemove : entitiesToRemove) {
        m_staticEntities.removeEntity(entityToRemove);
    }
    for (auto other : m_movableEntities) { // none of the movables are ever for removal
        if (other != entity && other->collidesWithItem(entity)) {
            collisions.push_back(other->getType());
            other->processCollision(entity->getType());
        }
    }
    return collisions;
}

void GameHandler::updateGhosts() {
    QPoint playerPosition = m_player->getGridPos();
    QPoint playerDirection = m_player->getDirection();
    for (auto ghost : m_ghosts) {
        ghost->tickDeathTimer();
        if (m_player->getPowerUpTimer() > 0) {
            ghost->scare();
        } else {
            ghost->relax();
        }
        if (m_player->getEatenPellets() > m_numPellets*ghost->getAggression()) {
            ghost->aggravate();
        }
        ghost->processPathFinding(playerPosition, getOpposite(playerPosition), playerDirection, m_ghostRespawnPos);
    }
}

void GameHandler::update() {
    for (auto entity : m_movableEntities) {
        entity->processMovement([this](Entity* entity) { return getCollisions(entity); });
        entity->checkForOutOfBounds(m_levelData.getDimensions());
    }
    m_scoreboard.updateScore(m_player->getScore(), m_player->getLives(), getHighscore());
    updateGhosts();
    if (m_pathFindingDebugOn) {
        resetDrawnPaths();
        drawGhostPaths();
    }
    m_player->tickPowerUpTimer();
    if (m_player->getLives() <= 0) {
        qDebug() << "Game Over!";
        qDebug() << "You scored: " << m_player->getScore();
        QCoreApplication::quit();
    } else if (m_player->getEatenPellets() >= m_numPellets) {
        qDebug() << "You win!";
        qDebug() << "This time...";
        reloadLevel();
    } else if (m_player->getIsGhosted()) {
        m_player->resetGhosted();
        m_player->resetPosition();
        for (auto ghost: m_ghosts) {
            ghost->teleportHome();
        }
    }

}

void GameHandler::reloadLevel() {
    int playerScore = m_player->getScore();
    int playerLives = m_player->getLives();
    if (m_scene!= nullptr){
        for (auto item : m_scene->items()) {
            m_scene->removeItem(item);
        }
    }
    m_staticEntities.clearEntities();
    m_movableEntities.clearEntities();
    m_pathFinder.clearState();
    m_numPellets = 0;
    m_ghosts.clear();
    m_player = nullptr;
    m_mazeWalls = nullptr;
    loadLevel();
    if (m_player == nullptr) {throw MissingPlayerException("this should never be thrown");}
    m_player->setGameData(playerScore, playerLives);
    buildScene();
    buildView(m_scene);
}

QGraphicsScene *GameHandler::buildScene() {
    if (m_scene==nullptr) {
        m_scene = new QGraphicsScene();
    }
    m_scene->setBackgroundBrush(Qt::black);
    for (auto entity : m_staticEntities) {
        m_scene->addItem(entity);
    }
    for (auto entity : m_movableEntities) {
        m_scene->addItem(entity);
    }
    m_scoreboard.setPos(0, m_levelData.height * m_tileSize);
    m_scene->addItem(&m_scoreboard);
    return m_scene;
}

QGraphicsView *GameHandler::buildView(QGraphicsScene *scene) {
    if (m_view==nullptr) {
        m_view = new QGraphicsView(scene);
    }
    m_view->setSceneRect(0, 0, m_levelData.width * m_tileSize, m_levelData.height * m_tileSize + SCOREBOARD_TILE_HEIGHT * m_tileSize);
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setFixedSize(m_levelData.width * m_tileSize+2, m_levelData.height * m_tileSize+2 + SCOREBOARD_TILE_HEIGHT * m_tileSize);
    return m_view;
}

void GameHandler::drawGhostPaths(){
    for (auto ghost : m_ghosts) {
        drawPath(ghost->getColor(), ghost->getPath());
    }
}

void GameHandler::drawPath(const QColor &color, const QVector<QPoint>& path){
    for (auto& point : path) {
        QGraphicsEllipseItem* ellipse = m_scene->addEllipse(point.x() * m_tileSize + m_tileSize / 2 - m_tileSize / 16,
                                                            point.y() * m_tileSize + m_tileSize / 2 - m_tileSize / 16,
                                                            m_tileSize / 8, m_tileSize / 8,
                                                            QPen(color), QBrush(color));
        m_pathEllipses.append(ellipse);
    }
}

void GameHandler::resetDrawnPaths() {
    for (auto ellipse : m_pathEllipses) {
        m_scene->removeItem(ellipse);
        delete ellipse;
    }
    m_pathEllipses.clear();
}

void GameHandler::spawnRandomExtraPellet() {
    // doesn't count towards total pellet count
    QPoint pos = {QRandomGenerator::global()->bounded(m_levelData.width), QRandomGenerator::global()->bounded(m_levelData.height)};
    while (m_levelData.tiles[pos.y()][pos.x()] != AIR_CHAR) {
        pos = {QRandomGenerator::global()->bounded(m_levelData.width), QRandomGenerator::global()->bounded(m_levelData.height)};
    }
    auto* pellet = new Pellet(pos, m_tileSize);
    m_staticEntities.addEntity(pellet);
    m_scene->addItem(pellet);
}

