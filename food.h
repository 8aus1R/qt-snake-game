#ifndef FOOD_H
#define FOOD_H

#include <QPoint>

class Food
{
public:
    Food();

    QPoint position() const;
    void setPosition(QPoint newSetPosition);

private:
    QPoint m_position;
};

#endif // FOOD_H
