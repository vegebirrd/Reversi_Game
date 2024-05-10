#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "boardwidget.h"  // 包含 BoardWidget 的头文件
#include <QtMultimedia>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void saveGame();
    void loadGame();
    void showSplashScreen();

protected:
    void showEvent(QShowEvent *event) override;

private slots:
    void startGame();          // 声明开始游戏的槽函数
    void back();
    void back2();
    void startPvPGame();
    void startPvAIGame();
    void startBlack();
    void startWhite();
    void mute();
    void unmute();
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void re();

private:
    Ui::MainWindow *ui;
    BoardWidget *boardWidget;  // 添加 BoardWidget 作为成员变量
    QWidget *mainMenuWidget; // 声明 mainMenuWidget
    QPushButton *backButton;
    QPushButton *reButton;
    QWidget *selectModeWidget;
    QWidget *selectColorWidget;
    QMediaPlayer *musicPlayer;
};
#endif // MAINWINDOW_H
