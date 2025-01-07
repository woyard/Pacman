#pragma once
#include <QVector>
#include <QPoint>

struct LevelData {
    int width = 0;
    int height = 0;
    int highscore = 0;
    QVector<QVector<char>> tiles;
    QPoint getDimensions() const { return {width, height}; }
};