#include "LevelFileManager.h"
#include "tile_definitions.h"

#include <QFile>
#include <QTextStream>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QPainter>
#include <QDebug>

void LevelFileManager::loadFromFile() {
    QFile file(m_filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw LoadException("could not open file '" + m_filename + "'");
    }
    QTextStream in(&file);
    QString line;
    if (!in.readLineInto(&line)) {
        throw LoadException("corrupted file '" + m_filename + "' (missing data)");
    }
    QStringList parts = line.split(' ');
    if (parts.size() != 3) {
        throw LoadException("corrupted file '" + m_filename + "' (invalid data)");
    }
    data.width = parts[0].toInt();
    data.height = parts[1].toInt();
    data.highscore = parts[2].toInt();
    data.tiles.resize(data.height);
    for (int y = 0; y < data.height; y++) {
        if (!in.readLineInto(&line) || line.length() != data.width) {
            throw LoadException("corrupted file '" + m_filename + "' (invalid line length)");
        }
        data.tiles[y].resize(data.width);
        for (int x = 0; x < data.width; x++) {
            data.tiles[y][x] = line[x].toLatin1();
        }
    }
    file.close();
    printToConsole();
}

void LevelFileManager::saveToFile(int highscore) {
    QFile file(m_filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        throw LoadException("could not open file '" + m_filename + "'");
    }
    QTextStream out(&file);
    out << data.width << ' ' << data.height << ' ' << highscore << '\n';
    for (int y = 0; y < data.height; y++) {
        for (int x = 0; x < data.width; x++) {
            out << data.tiles[y][x];
        }
        out << '\n';
    }
    file.close();
}

void LevelFileManager::printToConsole() {
    qDebug() << "level size: " << data.width << "x" << data.height;
    qDebug() << "highscore: " << data.highscore;
    for (int y = 0; y < data.height; ++y) {
        QString line;
        for (int x = 0; x < data.width; ++x) {
            line += data.tiles[y][x];
        }
        qDebug() << line;
    }
}

QGraphicsPixmapItem* LevelFileManager::createFilePreview(int tileSize) const {
    QPixmap pixmap(tileSize * data.width, tileSize * data.height);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    for (int y = 0; y < data.height; ++y) {
        for (int x = 0; x < data.width; ++x) {
            switch (data.tiles[y][x]) {
                case WALL_CHAR:
                    painter.fillRect(x * tileSize, y * tileSize, tileSize, tileSize, Qt::blue);
                    break;
                case AIR_CHAR:
                    painter.fillRect(x * tileSize, y * tileSize, tileSize, tileSize, Qt::black);
                    break;
                case PELLET_CHAR:
                    painter.setBrush(Qt::white);
                    painter.drawEllipse(x * tileSize + tileSize/2 - tileSize/8, y * tileSize + tileSize/2 - tileSize/8, tileSize/4, tileSize/4);
                    break;
                case POWERUP_CHAR:
                    painter.setBrush(Qt::white);
                    painter.drawEllipse(x * tileSize + tileSize/4, y * tileSize + tileSize/4, tileSize/2, tileSize/2);
                    break;
                case DOOR_CHAR:
                    painter.fillRect(x * tileSize, y * tileSize, tileSize, tileSize, QColor(85, 62, 42));
                    break;
                case GHOST_RESPAWN_CHAR:
                    painter.fillRect(x * tileSize, y * tileSize, tileSize, tileSize, Qt::darkGray);
                    break;
                case PLAYER_CHAR:
                    painter.setBrush(Qt::yellow);
                    painter.drawEllipse(x * tileSize, y * tileSize, tileSize, tileSize);
                    break;
                case BLINKY_CHAR:
                    painter.fillRect(x * tileSize, y * tileSize, tileSize, tileSize, Qt::red);
                    break;
                case PINKY_CHAR:
                    painter.fillRect(x * tileSize, y * tileSize, tileSize, tileSize, Qt::magenta);
                    break;
                case INKY_CHAR:
                    painter.fillRect(x * tileSize, y * tileSize, tileSize, tileSize, Qt::cyan);
                    break;
                case CLYDE_CHAR:
                    painter.fillRect(x * tileSize, y * tileSize, tileSize, tileSize, Qt::yellow);
                    break;
                default:
                    painter.fillRect(x * tileSize, y * tileSize, tileSize, tileSize, QColor(255, 165, 0)); // ERROR ORANGE
            }
        }
    }
    return new QGraphicsPixmapItem(pixmap);
}