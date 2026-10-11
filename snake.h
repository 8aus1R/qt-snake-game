#ifndef SNAKE_H
#define SNAKE_H

#include <QList>
#include <QPoint>

enum Direction
{
    Up,
    Down,
    Left,
    Right
};

class Snake
{
public:
    Snake();

    void reset();//重新开始
    void move();
    QPoint nextHead() const;
    void grow();
    void changeDirection(Direction newDirection);
    bool occupies(const QPoint &cell) const;

    QList<QPoint> body() const;
    QPoint head() const;

private:
//    int m_length;
    QList<QPoint> m_body;
    Direction m_direction;
    bool m_directionChanged;
//    int m_headX;
//    int m_headY;
};

#endif // SNAKE_H
