#include "food.h"

Food::Food()
{
    m_position = QPoint(5, 5);
}

QPoint Food::position() const
{
    return m_position;
}

void Food::setPosition(QPoint newPosition)
{
    m_position = newPosition;
}
