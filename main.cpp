// main.cpp
#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>

#include "Utilities/LevelFileManager.h"
#include "GameHandler.h"

const int TILE_SIZE = 64;
const int FRAME_RATE = 1000 / 60;

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    LevelFileManager level("test_level_32x16.txt");

    GameHandler gameHandler(level.getData(), TILE_SIZE);

    QGraphicsScene *scene = gameHandler.buildScene();

    //auto *preview = level.createFilePreview(TILE_SIZE);
    //scene.addItem(preview);

    QGraphicsView* view = gameHandler.buildView(scene);

    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&gameHandler]() { gameHandler.update(); });
    timer.start(FRAME_RATE);
    QObject::connect(&a, &QApplication::aboutToQuit, [&level, &gameHandler]() { level.saveToFile(gameHandler.getHighscore());});
    view->show();
    return a.exec();
}