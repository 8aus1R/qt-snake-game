#include "gamewindow.h"
#include <QPainter>
#include <QFont>
#include <QKeyEvent>
#include <QMessageBox>

namespace {
const int BoardPixelSize = 500;
const int InfoBarHeight = 70;
const int CellSize = 25;

const int InitialIntervalMs = 220;
const int IntervalDecreasePerFoodMs = 10;
const int MinimumIntervalMs = 100;
}

GameWindow::GameWindow(QWidget *parent)
    : QWidget(parent),
      m_paused(false),
      m_gameOverDialogShown(false)
{
    // 顶部信息栏分两行显示，避免署名、分数和操作说明互相挤压。
    setFixedSize(BoardPixelSize, BoardPixelSize + InfoBarHeight);

    setFocusPolicy(Qt::StrongFocus);//确保接受到键盘事件

    m_timer.setInterval(InitialIntervalMs);

    connect(&m_timer, &QTimer::timeout,
            this, &GameWindow::onTimeout);

    m_timer.start();
}

GameWindow::~GameWindow()
{
}

void GameWindow::onTimeout()
{
    const bool wasGameOver = m_board.isGameover();
    const int scoreBeforeMove = m_board.score();

    m_board.advance();

    if (m_board.score() > scoreBeforeMove) {
        int nextInterval = InitialIntervalMs
                           - m_board.score() * IntervalDecreasePerFoodMs;

        if (nextInterval < MinimumIntervalMs) {
            nextInterval = MinimumIntervalMs;
        }

        m_timer.setInterval(nextInterval);
    }

    if (m_board.isGameover() || m_board.isWon()) {
        m_timer.stop();
        update();

        // 只有本局刚失败时弹一次，避免计时器事件重复打断玩家。
        if (m_board.isGameover() && !wasGameOver && !m_gameOverDialogShown) {
            m_gameOverDialogShown = true;
            showGameOverDialog();
        }

        return;
    }

    update();
}

void GameWindow::restartGame()
{
    m_board.reset();
    m_timer.setInterval(InitialIntervalMs);
    m_paused = false;
    m_gameOverDialogShown = false;
    m_timer.start();
    update();
}

void GameWindow::showGameOverDialog()
{
    QMessageBox::information(this,
                             "游戏结束",
                             "撞墙或撞到自己，游戏结束！\n"
                             "按 Space 或 R 重新开始。");
}

void GameWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    // 顶部信息栏
    painter.fillRect(0, 0, BoardPixelSize, InfoBarHeight,
                     QColor(38, 50, 56));

    QFont infoFont = painter.font();
    infoFont.setPointSize(9);
    infoFont.setBold(true);
    painter.setFont(infoFont);
    painter.setPen(Qt::white);

    // 答辩要求：署名固定显示在顶部信息栏最左侧。
    const int infoLineHeight = InfoBarHeight / 2;

    // 署名单独左对齐，满足答辩要求，也不会随分数位数变化而移动。
    painter.drawText(QRect(15, 0, 170, infoLineHeight),
                     Qt::AlignLeft | Qt::AlignVCenter,
                     "28 张宝泽");

    const QString gameInfo = "Score: "
                             + QString::number(m_board.score())
                             + "　Speed: "
                             + QString::number(m_timer.interval()) + "ms";

    painter.drawText(QRect(185, 0, 300, infoLineHeight),
                     Qt::AlignRight | Qt::AlignVCenter,
                     gameInfo);

    painter.drawText(QRect(15, infoLineHeight, 470, infoLineHeight),
                     Qt::AlignCenter,
                     "方向键移动　Space暂停/继续　失败后 Space/R 重开");

    // 棋盘背景
    painter.fillRect(0, InfoBarHeight,
                     BoardPixelSize, BoardPixelSize,
                     QColor(248, 250, 252));

    // 画 20 × 20 的格子
    painter.setPen(QColor(218, 226, 236));

    for (int x = 0; x < 20; x++) {
        for (int y = 0; y < 20; y++) {
            painter.drawRect(x * CellSize,
                             InfoBarHeight + y * CellSize,
                             CellSize, CellSize);
        }
    }

    // 画绿色的蛇
    QList<QPoint> snakeBody = m_board.snake().body();

    for (int i = 0; i < snakeBody.size(); i++) {
        QPoint bodyPart = snakeBody.at(i);

        if (i == 0) {
            painter.setBrush(Qt::darkGreen); // 蛇头
            painter.setPen(Qt::black);
        } else {
            painter.setBrush(Qt::green);     // 蛇身
            painter.setPen(Qt::darkGreen);
        }

        painter.drawRoundedRect(bodyPart.x() * CellSize + 2,
                                InfoBarHeight + bodyPart.y() * CellSize + 2,
                                CellSize - 4, CellSize - 4,
                                5, 5);
    }

    if (!m_board.isWon()) {
        //红色食物
        QPoint foodPosition = m_board.food().position();

        painter.setBrush(Qt::red);
        painter.setPen(Qt::darkRed);

        painter.drawEllipse(foodPosition.x() * CellSize + 3,
                            InfoBarHeight + foodPosition.y() * CellSize + 3,
                            19, 19);

        painter.drawEllipse(foodPosition.x() * CellSize + 3,
                            InfoBarHeight + foodPosition.y() * CellSize + 3,
                            19, 19);

        painter.setBrush(Qt::white);
        painter.setPen(Qt::NoPen);

        painter.drawEllipse(foodPosition.x() * CellSize + 7,
                            InfoBarHeight + foodPosition.y() * CellSize + 7,
                            6, 6);
    }


    const QRect statusPanelRect(70, InfoBarHeight + 175, 360, 150);

    const QRect statusTitleRect(70, InfoBarHeight + 190, 360, 60);
    const QRect statusHintRect(70, InfoBarHeight + 250, 360, 40);

    QFont statusTitleFont = painter.font();
    statusTitleFont.setPointSize(18);
    statusTitleFont.setBold(true);

    QFont statusHintFont = painter.font();
    statusHintFont.setPointSize(10);
    statusHintFont.setBold(false);

    if (m_paused || m_board.isGameover() || m_board.isWon()) {
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(38, 50, 56, 220));
        painter.drawRoundedRect(statusPanelRect, 12, 12);

        painter.setPen(Qt::white);
    }

    if (m_paused) {
        painter.setPen(Qt::white);

        painter.setFont(statusTitleFont);
        painter.drawText(statusTitleRect,
                         Qt::AlignCenter, "Paused");

        painter.setFont(statusHintFont);
        painter.drawText(statusHintRect,
                         Qt::AlignCenter, "Press Space to continue");
    }

    if (m_board.isWon()) {
        painter.setPen(Qt::white);

        painter.setFont(statusTitleFont);
        painter.drawText(statusTitleRect,
                         Qt::AlignCenter, "You Win!");

        painter.setFont(statusHintFont);
        painter.drawText(statusHintRect,
                         Qt::AlignCenter, "Press R to restart");
    } else if (m_board.isGameover()) {
        painter.setPen(Qt::white);

        painter.setFont(statusTitleFont);
        painter.drawText(statusTitleRect,
                         Qt::AlignCenter, "Game Over");

        painter.setFont(statusHintFont);
        painter.drawText(statusHintRect,
                         Qt::AlignCenter, "Press R to restart");
    }
}

void GameWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up:
        m_board.changeDirection(Up);
        break;

    case Qt::Key_Down:
        m_board.changeDirection(Down);
        break;

    case Qt::Key_Left:
        m_board.changeDirection(Left);
        break;

    case Qt::Key_Right:
        m_board.changeDirection(Right);
        break;
    case Qt::Key_Space:
        if (m_board.isGameover() || m_board.isWon()) {
            restartGame();
        } else {
            m_paused = !m_paused;

            if (m_paused) {
                m_timer.stop();
            } else {
                m_timer.start();
            }

            update();
        }
        break;
    case Qt::Key_R:
        if (m_board.isGameover() || m_board.isWon()) {
            restartGame();
        }
        break;
    default:
        QWidget::keyPressEvent(event);
        break;
    }
}
