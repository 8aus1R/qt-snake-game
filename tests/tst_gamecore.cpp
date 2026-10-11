#include <QtTest>

#include "../snake.h"

class GameCoreTest : public QObject
{
    Q_OBJECT

private slots:
    void snakeStartsWithThreeSegments();
    void snakeMovesWithoutGrowing();
    void snakeGrowsWhenRequested();
    void snakeRejectsOppositeDirectionAndSecondTurn();
    void snakeReportsOccupiedCells();
};

void GameCoreTest::snakeStartsWithThreeSegments()
{
    Snake snake;

    QCOMPARE(snake.body(), QList<QPoint>({QPoint(10, 10), QPoint(9, 10), QPoint(8, 10)}));
    QCOMPARE(snake.head(), QPoint(10, 10));
}

void GameCoreTest::snakeMovesWithoutGrowing()
{
    Snake snake;

    snake.move();

    QCOMPARE(snake.head(), QPoint(11, 10));
    QCOMPARE(snake.body().size(), 3);
}

void GameCoreTest::snakeGrowsWhenRequested()
{
    Snake snake;

    snake.move();
    snake.grow();

    QCOMPARE(snake.head(), QPoint(11, 10));
    QCOMPARE(snake.body().size(), 4);
}

void GameCoreTest::snakeRejectsOppositeDirectionAndSecondTurn()
{
    Snake snake;

    // 初始向右时，立即向左属于反向转弯，下一格仍必须在右侧。
    snake.changeDirection(Left);
    QCOMPARE(snake.nextHead(), QPoint(11, 10));

    // 一个移动周期内只接受第一次有效转向，防止连续按键绕过反向限制。
    snake.changeDirection(Up);
    QCOMPARE(snake.nextHead(), QPoint(10, 9));
    snake.changeDirection(Right);
    QCOMPARE(snake.nextHead(), QPoint(10, 9));

    // 完成一次移动后，方向锁解除，可以在下一周期继续转向。
    snake.move();
    snake.changeDirection(Right);
    QCOMPARE(snake.nextHead(), QPoint(11, 9));
}

void GameCoreTest::snakeReportsOccupiedCells()
{
    Snake snake;

    QVERIFY(snake.occupies(QPoint(9, 10)));
    QVERIFY(!snake.occupies(QPoint(10, 11)));
}

QTEST_APPLESS_MAIN(GameCoreTest)

#include "tst_gamecore.moc"
