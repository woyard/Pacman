#include "PathFinder.h"

void PathFinder::generatePath() {
    clearState();
    Node* startNode = new Node(m_start);
    m_openList.push_back(startNode);
    while (true){
        Node* current = findLowestCostOpenNode();
        if (current == nullptr) { m_path.clear(); return; }
        //qDebug()<<current->pos << current->f << current->g << current->h;
        m_closedList.push_back(current);
        m_openList.removeOne(current);
        if (current->pos == m_end) {
            m_path = reconstructPath(current);
            return;
        }
        QVector<QPoint> neighbours = getNeighbours(current->pos);
        for (auto pos : neighbours) {
            if (posInList(pos, m_closedList) || isBlocked(pos)) {
                continue;
            }
            int foundOpenId = indexOfNodeWithPos(pos, m_openList);
            if (foundOpenId == -1) {
                Node* neighbourNode = new Node(pos, current);
                neighbourNode->g = current->g + 1;
                neighbourNode->h = calculateH(pos);
                neighbourNode->f = neighbourNode->g + neighbourNode->h;
                m_openList.push_back(neighbourNode);
            } else {
                if (current->g + 1 < m_openList[foundOpenId]->g) {
                    m_openList[foundOpenId]->g = current->g + 1;
                    m_openList[foundOpenId]->f = m_openList[foundOpenId]->g + m_openList[foundOpenId]->h;
                }
            }
        }
    }
}

QVector<QPoint> PathFinder::getNeighbours(QPoint pos) const {
    QVector<QPoint> neighbours;
    if (pos.x() > 0) { neighbours.push_back(pos + QPoint(-1, 0)); }
    else { neighbours.push_back(pos + QPoint(m_levelData.width - 1, 0)); }
    if (pos.x() < m_levelData.width - 1) { neighbours.push_back(pos + QPoint(1, 0)); }
    else { neighbours.push_back(pos + QPoint(-m_levelData.width + 1, 0)); }
    if (pos.y() > 0) { neighbours.push_back(pos + QPoint(0, -1)); }
    else { neighbours.push_back(pos + QPoint(0, m_levelData.height - 1)); }
    if (pos.y() < m_levelData.height - 1) { neighbours.push_back(pos + QPoint(0, 1)); }
    else { neighbours.push_back(pos + QPoint(0, -m_levelData.height + 1)); }
    return neighbours;
}

PathFinder::Node *PathFinder::findLowestCostOpenNode() {
    int lowestCost = std::numeric_limits<int>::max();
    Node *lowestCostNode = nullptr;
    for (auto & node : m_openList) {
        if (node->f < lowestCost) {
            lowestCost = node->f;
            lowestCostNode = node;
        }
    }
    return lowestCostNode;
}

QVector<QPoint> PathFinder::reconstructPath(PathFinder::Node *node) {
    QVector<QPoint> path;
    while (node != nullptr) {
        path.push_back(node->pos);
        node = node->parent;
    }
    return path;
}

int PathFinder::indexOfNodeWithPos(QPoint pos, const QVector<Node *> &list) {
    for (int i = 0; i < list.size(); ++i) {
        if (list[i]->pos == pos) {
            return i;
        }
    }
    return -1;
}

void PathFinder::clearState() {
    for (auto node : m_openList) {
        delete node;
    }
    for (auto node : m_closedList) {
        delete node;
    }
    m_openList.clear();
    m_closedList.clear();
}

QGraphicsPixmapItem *PathFinder::createPreview(int tileSize) {
    if (!m_previewItem) {
        m_previewItem = new QGraphicsPixmapItem();
    }
    QPixmap pixmap(tileSize * m_levelData.width, tileSize * m_levelData.height);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    for (auto node : m_openList) {
        painter.setBrush(Qt::darkCyan);
        painter.drawRect(node->pos.x() * tileSize, node->pos.y() * tileSize, tileSize, tileSize);
    }
    for (auto node : m_closedList) {
        painter.setBrush(Qt::darkRed);
        painter.drawRect(node->pos.x() * tileSize, node->pos.y() * tileSize, tileSize, tileSize);
    }
    painter.setBrush(Qt::darkGreen);
    painter.drawRect(m_start.x() * tileSize, m_start.y() * tileSize, tileSize, tileSize);
    painter.setBrush(Qt::darkMagenta);
    painter.drawRect(m_end.x() * tileSize, m_end.y() * tileSize, tileSize, tileSize);
    m_previewItem->setPixmap(pixmap);
    return m_previewItem;
}