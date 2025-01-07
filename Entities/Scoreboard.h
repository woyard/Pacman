#pragma once

#include <QGraphicsItem>
#include <QPainter>


class Scoreboard : public QGraphicsItem {
public:
    Scoreboard(QPoint size) : m_size(size) {}

    QRectF boundingRect() const override {
        return QRectF(0, 0, m_size.x(), m_size.y());
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
        painter->setBrush(Qt::darkBlue);
        painter->drawRect(boundingRect());
        QFont font = painter->font();
        font.setPointSize(m_size.x() / 64);
        painter->setFont(font);
        painter->setPen(Qt::white);
        painter->drawText(m_size.x()/10, m_size.y()/3, m_textScore);
        painter->drawText(m_size.x()/3, m_size.y()/3, m_textHighscore);
        painter->drawText(m_size.x()/3*2, m_size.y()/3, m_textLives);

    }

    void updateScore(int score, int lives, int highscore) {
        if (m_score == score && m_lives == lives && m_highscore == highscore) {
            return;
        }
        m_textScore = QString("Score: %1").arg(QString::number(score));
        m_textLives = QString("Lives: %1").arg(QString::number(lives));
        m_textHighscore = QString("Highscore: %1").arg(QString::number(highscore));
        m_score = score;
        m_lives = lives;
        m_highscore = highscore;
        update();
    }

private:
    int m_score = -1;
    int m_lives = -1;
    int m_highscore = -1;
    QString m_textScore;
    QString m_textLives;
    QString m_textHighscore;
    QPoint m_size;
};
