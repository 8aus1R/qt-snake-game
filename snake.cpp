#include "snake.h"

Snake::Snake()
{
    reset();
}

void Snake::reset()
{
    m_body.clear();

    m_body.append(QPoint(10,10));
    m_body.append(QPoint(9,10));
    m_body.append(QPoint(8,10));

    m_direction = Right;
    m_directionChanged = false;

}

QPoint Snake::nextHead() const
{
    // 只根据当前方向预测下一格蛇头，不修改蛇身；碰撞检查会先使用这个坐标。
    QPoint next = m_body.first();

    switch (m_direction) {
    case Up:
        next.setY(next.y() - 1);
        break;
    case Down:
        next.setY(next.y() + 1);
        break;
    case Left:
        next.setX(next.x() - 1);
        break;
    case Right:
        next.setX(next.x() + 1);
        break;
    }

    return next;
}

void Snake::move()
{
    // 正式移动时，把新蛇头插到链表前端，再删除旧蛇尾，因此长度保持不变。
    QPoint newHead = nextHead();

    m_body.prepend(newHead);
    m_body.removeLast();
    m_directionChanged = false;
}

void Snake::grow()
{
    m_body.append(m_body.last());
}

void Snake::changeDirection(Direction newDirection)
{
    if (m_directionChanged) {
        return;
    }

    if (m_direction == Up && newDirection == Down) {
        return;
    }

    if (m_direction == Down && newDirection == Up) {
        return;
    }

    if (m_direction == Left && newDirection == Right) {
        return;
    }

    if (m_direction == Right && newDirection == Left) {
        return;
    }

    m_direction = newDirection;
    m_directionChanged = true;
}

bool Snake::occupies(const QPoint &cell) const
{
    return m_body.contains(cell);
}

QList<QPoint> Snake::body() const
{
    return m_body;
}

QPoint Snake::head() const
{
    return m_body.first();
}
