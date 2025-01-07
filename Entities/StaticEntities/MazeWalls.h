#pragma once

#include <QBitmap>
#include "../Entity.h"
#include "../../LevelUtilities/LevelData.h"
#include "../../LevelUtilities/tile_definitions.h"

class MazeWalls : public Entity {
public:
    MazeWalls(QPoint pos, int tileSize, const LevelData& data) : Entity(pos, tileSize) {
        m_tileSize = tileSize;
        n_tiles = data.getDimensions();
        generatePixmap(data);
        QBitmap mask = m_qPixmap.createMaskFromColor(Qt::transparent);
        m_qRegion = QRegion(mask);
    }

    EntityType getType() override { return EntityType::WALL; }

    void processCollision(EntityType other) override {}

    void generatePixmap(const LevelData& data) {
        m_qPixmap = QPixmap(m_tileSize * data.width, m_tileSize * data.height);
        m_qPixmap.fill(Qt::transparent);
        QPainter painter(&m_qPixmap);
        for (int y = 0; y < data.height; ++y) {
            for (int x = 0; x < data.width; ++x) {
                if (data.tiles[y][x] == WALL_CHAR) {
                    painter.fillRect(x * m_tileSize, y * m_tileSize,
                                     m_tileSize, m_tileSize,
                                     Qt::green);
                }
            }
        }
    }

    bool collidesWithItem(const QGraphicsItem *other, Qt::ItemSelectionMode mode = Qt::IntersectsItemShape) const override {
        auto rect = other->boundingRect().translated(other->pos().toPoint()).toRect();
        return m_qRegion.intersects(QRegion(rect));
    }

    QRectF boundingRect() const override {
        return {0, 0, (double)m_tileSize * n_tiles.x(), (double)m_tileSize * n_tiles.y()};
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
        painter->drawPixmap(0, 0, m_qPixmap);
    }
private:
    QPixmap m_qPixmap;
    QRegion m_qRegion;
    QPoint n_tiles;
};
