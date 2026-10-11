#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
#include <QTimer>

#include "gameboard.h"

class QPaintEvent;
class QKeyEvent;

class GameWindow : public QWidget
{
    Q_OBJECT

public:
    GameWindow(QWidget *parent = nullptr);
    ~GameWindow();

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void onTimeout();

private:
    GameBoard m_board;
    QTimer m_timer;
    bool m_paused;
    bool m_gameOverDialogShown;

    void restartGame();
    void showGameOverDialog();
};

#endif // GAMEWINDOW_H
