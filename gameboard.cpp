#include "gameboard.h"
#include <QRandomGenerator>

GameBoard::GameBoard()
{
    reset();
}

void GameBoard::reset()
{
    m_snake.reset();

    m_score = 0;
    m_gameover = false;
    m_won = false;
    placeFood();
}

void GameBoard::advance()
{
    if (m_gameover || m_won) {
        return;
    }

    // 先预测下一格蛇头，再检查边界；这样失败时不会把越界坐标写入蛇身。
    QPoint snakeHead = m_snake.nextHead();

    // 碰到棋盘边缘立即结束本局，蛇保持在最后一个合法位置。
    if (snakeHead.x() < 0 || snakeHead.x() >= 20
            || snakeHead.y() < 0 || snakeHead.y() >= 20) {
        m_gameover = true;
        return;
    }

    m_snake.move();

    // 移动后从第 1 节开始检查，若新蛇头与任意身体格重叠就是撞到自己。
    QList<QPoint> snakeBody = m_snake.body();

    for (int i = 1; i < snakeBody.size(); i++) {
        QPoint bodyPart = snakeBody.at(i);

        if (snakeHead.x() == bodyPart.x()
                && snakeHead.y() == bodyPart.y()) {
            m_gameover = true;
            return;
        }
    }

    QPoint foodPosition = m_food.position();


    // 蛇头命中食物：增长一节、分数加一，再为下一颗食物选择空格。
    if (snakeHead.x() == foodPosition.x()
            && snakeHead.y() == foodPosition.y()) {
        m_snake.grow();
        m_score = m_score + 1;

        if (m_snake.body().size() >= 20 * 20) {
            m_won = true;
            return;
        }

        placeFood();
    }
}

void GameBoard::placeFood()
{
    QPoint newPosition;

    // 在 20×20 棋盘中随机抽取格子；抽到蛇身时重新抽取，保证食物不与蛇重叠。
    do {
        int x = QRandomGenerator::global()->bounded(20);
        int y = QRandomGenerator::global()->bounded(20);

        newPosition = QPoint(x, y);
    } while (m_snake.occupies(newPosition));

    m_food.setPosition(newPosition);
}

bool GameBoard::isGameover() const
{
    return m_gameover;
}

bool GameBoard::isWon() const
{
    return m_won;
}

Snake GameBoard::snake() const
{
    return m_snake;
}

Food GameBoard::food() const
{
    return m_food;
}

int GameBoard::score() const
{
    return m_score;
}

void GameBoard::changeDirection(Direction newDirection)
{
    m_snake.changeDirection(newDirection);
}
