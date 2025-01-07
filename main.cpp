// main.cpp
#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>

#include "LevelUtilities/LevelFileManager.h"
#include "GameHandler.h"


int main(int argc, char *argv[]) {
    int tileSize = 64;
    if (argc > 1) {
        tileSize = std::stoi(argv[1])*8;
    }
    int frameRate = 1000/60;
    if (argc > 2 && std::stoi(argv[2]) > 0) {
        frameRate = 1000/std::stoi(argv[2]);
    }
    QString filename = "level_16x16.txt";
    if (argc > 3) {
        filename = argv[3];
    }
    QApplication a(argc, argv);

    LevelFileManager level(filename);

    GameHandler gameHandler(level.getData(), tileSize);

    QGraphicsScene *scene = gameHandler.buildScene();

    //auto *preview = level.createFilePreview(tileSize);
    //scene.addItem(preview);

    QGraphicsView* view = gameHandler.buildView(scene);

    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&gameHandler]() { gameHandler.update(); });
    timer.start(frameRate);
    QObject::connect(&a, &QApplication::aboutToQuit, [&level, &gameHandler]() { level.saveToFile(gameHandler.getHighscore());});
    view->show();
    return a.exec();
}