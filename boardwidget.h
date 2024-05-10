// boardwidget.h
#ifndef BOARDWIDGET_H
#define BOARDWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

class BoardWidget : public QWidget
{
    Q_OBJECT

public://声明
    explicit BoardWidget(QWidget *parent = nullptr);
    void setGridValue(int row, int col, int value);  // 设置特定位置的棋盘状态
    void reset();
    bool checkBlackLegality(int x, int y);
    bool checkWhiteLegality(int x, int y);
    int countWhite();
    int countBlack();
    bool blackNeedSkip();
    bool whiteNeedSkip();
    void flipWhiteMove(int x, int y);
    void flipBlackMove(int x, int y);
    bool endgame();
    void normalBotWhiteMove();
    void normalBotBlackMove();
    void updateLabels();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    int cellSize;  // 棋子大小
    // 声明按钮
    QPushButton *backButton;// 声明按钮
    QLabel *roundLabel;
    QLabel *blackCountLabel;
    QLabel *whiteCountLabel;
    QLabel *currentPlayerLabel;
    QVBoxLayout *infoLayout;
    QPoint getCellPosition(const QPoint &point);  // 从鼠标点击位置获取对应的棋盘格
    void drawPiece(QPainter &painter, int row, int col);  // 绘制棋子
};

#endif // BOARDWIDGET_H
