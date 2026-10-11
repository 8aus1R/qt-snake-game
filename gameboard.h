#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "snake.h"
#include "food.h"

class GameBoard
{
public:
    GameBoard();

    void reset();
    void advance();

    bool isGameover() const;
    bool isWon() const;

    Snake snake() const;
    Food food() const;
    int score() const;
    void changeDirection(Direction newDirection);


private:
    Snake m_snake;
    Food m_food;
    int m_score;

    void placeFood();
    bool m_gameover;
    bool m_won;

};

#endif // GAMEBOARD_H
