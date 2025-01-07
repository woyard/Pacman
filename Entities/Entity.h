#pragma once

#include <QGraphicsItem>
#include <QPainter>
#include <QKeyEvent>

enum class EntityType {
    WALL,
    DOOR,
    PLAYER,
    LIVE_GHOST,
    DEAD_GHOST,
    PELLET,
    POWERUP
};

class Entity : public QGraphicsItem {
public:
    Entity(QPoint pos, int tileSize) : m_tileSize(tileSize) {
        setFlag(QGraphicsItem::ItemIsFocusable, false);
        setPos(pos* tileSize);
    }

    virtual EntityType getType() = 0;

    virtual void processCollision(EntityType other) = 0;

    virtual void processCollisions(const std::function<QVector<EntityType>(Entity*)>& getCollisions) {
        for (EntityType type : getCollisions(this)) {
            processCollision(type);
        }
    }

    virtual void processMovement(const std::function<QVector<EntityType>(Entity*)>& getCollisions) {}

    virtual void checkForOutOfBounds(QPoint levelSize) {}

// Defaults to override if needed
    QRectF boundingRect() const override {
        return QRectF(0, 0, m_tileSize, m_tileSize);
    }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
        painter->setBrush(Qt::green);
        painter->drawRect(0, 0, 32, 32); // Draw a green rectangle
    }

    QPoint getGridPos() const {
        return pos().toPoint() / m_tileSize;
    }

    bool getIsDead() const { return isDead; }

    void addDeathCallback(const std::function<void(Entity*)>& callback) {
        deathCallbacks.push_back(callback);
    }

    void kill() {
        isDead = true;
        for (const auto& callback : deathCallbacks) {
            callback(this);
        }
        delete this;
    }

protected:
    int m_tileSize;
private:
    bool isDead = false;
    QVector<std::function<void(Entity*)>> deathCallbacks;
};


class EntityVector : private QVector<Entity*> {
public:
    using QVector<Entity*>::begin;
    using QVector<Entity*>::end;
    ~EntityVector() {
        for (auto entity : *this) {
            delete entity;
        }
    }

    void addEntity(Entity* entity) {
        push_back(entity);
        entity->addDeathCallback([this](Entity* entity) { removeEntity(entity); });
    }

    void removeEntity(Entity* entity) {
        removeOne(entity);
        if (!entity->getIsDead()) {
            delete entity;
        }
    }

    void clearEntities() {
        for (auto entity : *this) {
            delete entity;
        }
        QVector<Entity*>::clear();
    }
};
