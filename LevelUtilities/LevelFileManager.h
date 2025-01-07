#pragma once
#include <QString>
#include <QGraphicsScene>
#include <exception>
#include <utility>

#include "LevelData.h"

class LevelFileManager {
public:
    explicit LevelFileManager(QString filename) : m_filename(std::move(filename)) {
        loadFromFile();
    }
    void loadFromFile();
    void saveToFile(int highscore);
    QGraphicsPixmapItem* createFilePreview(int tileSize) const; // for debugging
    const LevelData& getData() const { return data; }

private:
    QString m_filename;
    LevelData data;

    void printToConsole();

public:
    class LoadException : public std::exception {
    public:
        explicit LoadException(QString  message) : m_message(std::move(message)) {}
        const char* what() const noexcept override {
            return m_message.toUtf8().constData();
        }
    private:
        QString m_message;
    };
};
