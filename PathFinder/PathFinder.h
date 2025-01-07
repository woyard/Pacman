#pragma once

#include <limits>
#include <QGraphicsPixmapItem>
#include <QPainter>

#include "../LevelUtilities/LevelData.h"

class PathFinder {
private:
    const LevelData &m_levelData;
    QVector<char> m_colliderChars = {};
    QPoint m_start;
    QPoint m_end;
    QVector<QPoint> m_path;
    bool hasNewWaypoints = true;

    struct Node {
        explicit Node (QPoint pos) : pos(pos) {}
        Node (QPoint pos, Node* parent) : pos(pos), parent(parent) {}
        QPoint pos;
        int g = 0;
        int h = 0;
        int f = 0;
        Node *parent = nullptr;
    };

    static int indexOfNodeWithPos(QPoint pos, const QVector<Node*> &list);
    static bool posInList(QPoint pos, const QVector<Node*> &list) {
        return indexOfNodeWithPos(pos, list) != -1;
    }

    Node* findLowestCostOpenNode();

    QVector<QPoint> getNeighbours(QPoint pos) const;

    bool isBlocked(QPoint pos) const {
        return m_colliderChars.contains(m_levelData.tiles[pos.y()][pos.x()]);
    }

    int calculateH(QPoint pos) const {
        return abs(pos.x() - m_end.x()) + abs(pos.y() - m_end.y());
    }

    static QVector<QPoint> reconstructPath(Node* node);

    QVector<Node*> m_openList;
    QVector<Node*> m_closedList;

public:
    explicit PathFinder(const LevelData &levelData) : m_levelData(levelData) {}

    ~PathFinder() {
        clearState();
    }

    void clearState();

    void setColliderChars(const QVector<char> colliderChars) {
        m_colliderChars = colliderChars;
    }

    void setWaypoints(QPoint start, QPoint end) {
        if (hasNewWaypoints || m_start != start || m_end != end) {
            m_start = start;
            m_end = end;
            hasNewWaypoints = true;
        }
    }

    void generatePath();

    QVector<QPoint>& getPath(){
        if (hasNewWaypoints) {
            clearState();
            generatePath();
            hasNewWaypoints = false;
        }
        return m_path;
    }

    // DEBUG PREVIEW
    QGraphicsPixmapItem* createPreview(int tileSize);

private:
    QGraphicsPixmapItem* m_previewItem = nullptr;
};
