#pragma once
#include "../MovableEntity.h"
#include <QDebug>

class Player : public MovableEntity {
private:
    int m_score = 0;
    int m_eatenPellets = 0;
    int m_lives = 3;
    int m_powerUpTimer = 0;
    bool m_isGhosted = false;
public:
    Player(QPoint pos, int tileSize) : MovableEntity(pos, tileSize) {
        setHardCollisionEntities({EntityType::WALL, EntityType::DOOR});
        setFlag(QGraphicsItem::ItemIsFocusable);
        setFocus();
        setVelocity(QPoint(1, 0));
    }

    EntityType getType() override { return EntityType::PLAYER; }
    int getScore() const { return m_score; }
    int getEatenPellets() const { return m_eatenPellets; }
    int getLives() const { return m_lives; }
    int getPowerUpTimer() const { return m_powerUpTimer; }
    bool getIsGhosted() const { return m_isGhosted; }

    void resetGhosted() { m_isGhosted = false; }
    void tickPowerUpTimer() { m_powerUpTimer = std::max(0, m_powerUpTimer - 1); }

    void resetPosition() {
        setPos(m_originalPos * m_tileSize);
        setVelocity(QPoint(0, 0));
    }

    void processCollision(EntityType other) override {
        switch (other) {
            case EntityType::PELLET:
                m_score += 10;
                m_eatenPellets++;
                break;
            case EntityType::POWERUP:
                m_powerUpTimer = 200;
                break;
            case EntityType::LIVE_GHOST:
                if (m_powerUpTimer > 0) {
                    m_score += 100;
                } else {
                    m_lives--;
                    m_isGhosted = true;
                }
                break;
            default:
                break;
        }
    }

    void keyPressEvent(QKeyEvent *event) override {
        switch (event->key()) {
            case Qt::Key_Left:
                setVelocity(QPoint(-1, 0));
                break;
            case Qt::Key_Right:
                setVelocity(QPoint(1, 0));
                break;
            case Qt::Key_Up:
                setVelocity(QPoint(0, -1));
                break;
            case Qt::Key_Down:
                setVelocity(QPoint(0, 1));
                break;
        }
    }


    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
        painter->setBrush(Qt::yellow);
        painter->drawEllipse(0, 0, m_tileSize, m_tileSize);
    }
};
