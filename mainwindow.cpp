#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QFile>
#include <QTimer>
#include <QtMultimedia>
#include <QLabel>
#include <QPixmap>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include "globals.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QLabel *bgLabel = new QLabel(this);
    QPixmap pixmap("/Users/zhangwenhao/Reversi/bg.JPG");

    musicPlayer = new QMediaPlayer(this);
    musicPlayer->setSource(QUrl::fromLocalFile("/Users/zhangwenhao/Reversi/No Surprises.flac"));
    connect(musicPlayer, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::onMediaStatusChanged);
    QAudioOutput *audioOutput = new QAudioOutput;
    musicPlayer->setAudioOutput(audioOutput);
    musicPlayer->play();

    // 缩放 QPixmap 到 QLabel 的大小
    QPixmap scaledPixmap = pixmap.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // 设置 QLabel 的大小与主窗口一样大
    bgLabel->setFixedSize(this->size());
    // 设置缩放后的 QPixmap 到 QLabel
    bgLabel->setPixmap(scaledPixmap);
    bgLabel->setAlignment(Qt::AlignCenter);
    bgLabel->show();


    // 创建主菜单界面
    mainMenuWidget = new QWidget(this); // 声明 mainMenuWidget 为成员变量

    // 创建布局
    QVBoxLayout *mainMenuLayout = new QVBoxLayout(mainMenuWidget);

    // 创建标题
    QLabel *titleLabel = new QLabel("Reversi", mainMenuWidget);
    QFont titleFont;
    titleFont.setPointSize(34);  // 设置字体大小
    titleFont.setBold(true);     // 加粗
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);  // 居中对齐
    mainMenuLayout->addWidget(titleLabel);

    // 创建开始游戏按钮
    QPushButton *startButton = new QPushButton("New Game", mainMenuWidget);
    startButton->setFixedSize(150, 50);  // 设置按钮大小
    startButton->setFont(QFont("Arial", 16));  // 设置按钮字体和大小
    startButton->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; }");  // 设置按钮样式
    startButton->setCursor(Qt::PointingHandCursor);  // 鼠标悬停样式
    startButton->setFocusPolicy(Qt::NoFocus);  // 移除焦点框
    startButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);  // 固定大小
    startButton->setToolTip("Start a new game");  // 鼠标悬停提示
    mainMenuLayout->addWidget(startButton);

    QPushButton *loadButton = new QPushButton("Load Game", mainMenuWidget);
    loadButton->setFixedSize(150, 50);  // 设置按钮大小
    loadButton->setFont(QFont("Arial", 16));  // 设置按钮字体和大小
    loadButton->setStyleSheet("QPushButton { background-color: #2196F3; color: white; }");  // 设置按钮样式
    loadButton->setCursor(Qt::PointingHandCursor);  // 鼠标悬停样式
    loadButton->setFocusPolicy(Qt::NoFocus);  // 移除焦点框
    loadButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);  // 固定大小
    loadButton->setToolTip("Load a saved game");  // 鼠标悬停提示
    mainMenuLayout->addWidget(loadButton);

    // 连接加载游戏按钮的点击事件到槽函数
    connect(loadButton, &QPushButton::clicked, this, &MainWindow::loadGame);

    // 创建退出按钮
    QPushButton *exitButton = new QPushButton("Exit", mainMenuWidget);
    exitButton->setFixedSize(150, 50);  // 设置按钮大小
    exitButton->setFont(QFont("Arial", 16));  // 设置按钮字体和大小
    exitButton->setStyleSheet("QPushButton { background-color: #f44336; color: white; }");  // 设置按钮样式
    exitButton->setCursor(Qt::PointingHandCursor);  // 鼠标悬停样式
    exitButton->setFocusPolicy(Qt::NoFocus);  // 移除焦点框
    exitButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);  // 固定大小
    exitButton->setToolTip("Exit the game");  // 鼠标悬停提示
    mainMenuLayout->addWidget(exitButton);

    // 创建一个水平布局
    QHBoxLayout *muteLayout = new QHBoxLayout();

    // 创建一个按钮并设置其为静音图标
    QPushButton *muteButton = new QPushButton();
    muteButton->setIcon(QIcon("/Users/zhangwenhao/Reversi/mute.png")); // 替换为静音图标的路径
    muteButton->setIconSize(QSize(30, 30)); // 设置图标大小
    muteButton->setToolTip("Mute");  // 鼠标悬停提示

    // 创建一个按钮并设置其为不静音图标
    QPushButton *unmuteButton = new QPushButton();
    unmuteButton->setIcon(QIcon("/Users/zhangwenhao/Reversi/unmute.jpeg")); // 替换为不静音图标的路径
    unmuteButton->setIconSize(QSize(30, 30)); // 设置图标大小
    unmuteButton->setToolTip("Start playing music. BGM: No Surprises by Radiohead");  // 鼠标悬停提示

    // 将布局的水平和垂直对齐设置为靠右靠下

    muteLayout->addWidget(muteButton);
    muteLayout->addWidget(unmuteButton);

    mainMenuLayout->addLayout(muteLayout);

    muteButton->show();
    unmuteButton->show();

    // 连接静音按钮的点击事件到槽函数
    connect(muteButton, &QPushButton::clicked, this, &MainWindow::mute);
    connect(unmuteButton, &QPushButton::clicked, this, &MainWindow::unmute);

    // 居中对齐
    mainMenuLayout->setAlignment(Qt::AlignCenter);

    // 连接开始游戏按钮的点击事件到槽函数
    connect(startButton, &QPushButton::clicked, this, &MainWindow::startGame);

    // 连接退出按钮的点击事件到槽函数
    connect(exitButton, &QPushButton::clicked, this, &MainWindow::close);

    // 创建游戏界面
    boardWidget = new BoardWidget(this);

    // 设置游戏界面的布局
    QVBoxLayout *gameLayout = new QVBoxLayout;
    gameLayout->addWidget(boardWidget);
    // 创建退出按钮
    backButton = new QPushButton("Back", boardWidget);  // 将按钮命名为 backButton

    backButton->setFixedSize(150, 50);
    backButton->setFont(QFont("Arial", 16));
    backButton->setStyleSheet("QPushButton { background-color: #f44336; color: white; }");
    backButton->setCursor(Qt::PointingHandCursor);
    backButton->setFocusPolicy(Qt::NoFocus);
    backButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    backButton->setToolTip("Go back to the main menu");

    boardWidget->hide();  // 初始时隐藏界面
    backButton->hide();

    connect(backButton, &QPushButton::clicked, this, &MainWindow::back);

    // 创建重新开始按钮
    reButton = new QPushButton("Re-start", boardWidget);  // 将按钮命名为 reButton

    reButton->setFixedSize(150, 50);
    reButton->setFont(QFont("Arial", 16));
    reButton->setStyleSheet("QPushButton { background-color: #2196F3; color: white; }");
    reButton->setCursor(Qt::PointingHandCursor);
    reButton->setFocusPolicy(Qt::NoFocus);
    reButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    reButton->setToolTip("Restart the game.");

    boardWidget->hide();  // 初始时隐藏界面
    reButton->hide();

    connect(reButton, &QPushButton::clicked, this, &MainWindow::re);


    // 将主菜单界面和游戏界面添加到窗口
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(mainMenuWidget);
    mainLayout->addLayout(gameLayout);
    mainLayout->addWidget(reButton);
    mainLayout->addWidget(backButton);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    selectModeWidget = new QWidget(this);
    // 创建布局
    QVBoxLayout *selectModeLayout = new QVBoxLayout(selectModeWidget);
    // 创建标题
    QLabel *titleLabel2 = new QLabel("Select Game Mode", selectModeWidget);
    QFont titleFont2;
    titleFont2.setPointSize(24);  // 设置字体大小
    titleFont2.setBold(true);     // 加粗
    titleLabel2->setFont(titleFont2);
    titleLabel2->setAlignment(Qt::AlignCenter);  // 居中对齐
    selectModeLayout->addWidget(titleLabel2);

    // 创建PvP游戏按钮
    QPushButton *pvpButton = new QPushButton("PvP Mode", selectModeWidget);
    pvpButton->setFixedSize(150, 50);  // 设置按钮大小
    pvpButton->setFont(QFont("Arial", 16));  // 设置按钮字体和大小
    pvpButton->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; }");  // 设置按钮样式
    pvpButton->setCursor(Qt::PointingHandCursor);  // 鼠标悬停样式
    pvpButton->setFocusPolicy(Qt::NoFocus);  // 移除焦点框
    pvpButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);  // 固定大小
    pvpButton->setToolTip("Player versus player.");  // 鼠标悬停提示
    selectModeLayout->addWidget(pvpButton);

    // 创建PvAI按钮
    QPushButton *aiButton = new QPushButton("PvAI Mode", selectModeWidget);
    aiButton->setFixedSize(150, 50);  // 设置按钮大小
    aiButton->setFont(QFont("Arial", 16));  // 设置按钮字体和大小
    aiButton->setStyleSheet("QPushButton { background-color: #2196F3; color: white; }");  // 设置按钮样式
    aiButton->setCursor(Qt::PointingHandCursor);  // 鼠标悬停样式
    aiButton->setFocusPolicy(Qt::NoFocus);  // 移除焦点框
    aiButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);  // 固定大小
    aiButton->setToolTip("Player versus the robot.");  // 鼠标悬停提示
    selectModeLayout->addWidget(aiButton);

    // 创建退出按钮
    QPushButton *backButton2 = new QPushButton("Back", selectModeWidget);  // 将按钮命名为 backButton
    backButton2->setFixedSize(150, 50);
    backButton2->setFont(QFont("Arial", 16));
    backButton2->setStyleSheet("QPushButton { background-color: #f44336; color: white; }");
    backButton2->setCursor(Qt::PointingHandCursor);
    backButton2->setFocusPolicy(Qt::NoFocus);
    backButton2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    backButton2->setToolTip("Go back to the main menu");
    selectModeLayout->addWidget(backButton2);

    connect(backButton2, &QPushButton::clicked, this, &MainWindow::back2);

    // 连接PVP按钮的点击事件到槽函数
    connect(pvpButton, &QPushButton::clicked, this, &MainWindow::startPvPGame);

    // 连接AI按钮的点击事件到槽函数
    connect(aiButton, &QPushButton::clicked, this, &MainWindow::startPvAIGame);

    selectModeLayout->setAlignment(Qt::AlignCenter);

    mainLayout->addWidget(selectModeWidget);

    selectModeWidget->hide();

    selectColorWidget = new QWidget(this);
    // 创建布局
    QVBoxLayout *selectColorLayout = new QVBoxLayout(selectColorWidget);
    // 创建标题
    QLabel *titleLabel3 = new QLabel("Pick Your Side", selectColorWidget);
    QFont titleFont3;
    titleFont3.setPointSize(24);  // 设置字体大小
    titleFont3.setBold(true);     // 加粗
    titleLabel3->setFont(titleFont3);
    titleLabel3->setAlignment(Qt::AlignCenter);  // 居中对齐
    selectColorLayout->addWidget(titleLabel3);

    // 创建black按钮
    QPushButton *blackButton = new QPushButton("Black", selectColorWidget);
    blackButton->setFixedSize(150, 50);  // 设置按钮大小
    blackButton->setFont(QFont("Arial", 16));  // 设置按钮字体和大小
    blackButton->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; }");  // 设置按钮样式
    blackButton->setCursor(Qt::PointingHandCursor);  // 鼠标悬停样式
    blackButton->setFocusPolicy(Qt::NoFocus);  // 移除焦点框
    blackButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);  // 固定大小
    blackButton->setToolTip("Black");  // 鼠标悬停提示
    selectColorLayout->addWidget(blackButton);

    // 创建white按钮
    QPushButton *whiteButton = new QPushButton("White", selectColorWidget);
    whiteButton->setFixedSize(150, 50);  // 设置按钮大小
    whiteButton->setFont(QFont("Arial", 16));  // 设置按钮字体和大小
    whiteButton->setStyleSheet("QPushButton { background-color: #2196F3; color: white; }");  // 设置按钮样式
    whiteButton->setCursor(Qt::PointingHandCursor);  // 鼠标悬停样式
    whiteButton->setFocusPolicy(Qt::NoFocus);  // 移除焦点框
    whiteButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);  // 固定大小
    whiteButton->setToolTip("White");  // 鼠标悬停提示
    selectColorLayout->addWidget(whiteButton);

    // 连接BLACK按钮的点击事件到槽函数
    connect(blackButton, &QPushButton::clicked, this, &MainWindow::startBlack);

    // 连接WHITE按钮的点击事件到槽函数
    connect(whiteButton, &QPushButton::clicked, this, &MainWindow::startWhite);

    selectColorLayout->setAlignment(Qt::AlignCenter);

    mainLayout->addWidget(selectColorWidget);

    selectColorWidget->hide();

}

MainWindow::~MainWindow()
{
    delete ui;
}



// 保存按钮的槽函数
void MainWindow::saveGame() {
    QFile file("savegame.dat");
    if (file.open(QIODevice::WriteOnly)) {
        QDataStream out(&file);
        // 将游戏状态数据写入文件
        out << Round << ' ';
        for (int i=0;i<8;i++){
            for (int j=0;j<8;j++){
                out<<gridCondition[i][j]<<' ';
            }
        }
        out << gameMode;
        out << ' ';
        out << blackStone;
        out << ' ';
        out << whiteStone;
        file.close();
    }
}

// 加载按钮的槽函数
void MainWindow::loadGame() {
    QFile file("savegame.dat");
    if (file.open(QIODevice::ReadOnly)) {
        QDataStream in(&file);
        // 从文件中读取游戏状态数据
        in >> Round;
        char space;
        for (int i=0;i<8;i++){
            for (int j=0;j<8;j++){
                in >> space >> gridCondition[i][j];
            }
        }
        in >> space;
        in >> gameMode;
        in >> space;
        in >> blackStone;
        in >> space;
        in >> whiteStone;
        file.close();
        // 更新游戏界面以反映加载的状态
        update();
        mainMenuWidget->hide();
        backButton->show();
        reButton->show();
        selectModeWidget->hide();
        boardWidget->show();  // 显示游戏界面
        boardWidget->updateLabels();
    }
}

void MainWindow::startGame() {

    mainMenuWidget->hide();
    selectModeWidget->show();
}

void MainWindow::startPvPGame() {
    // 隐藏主菜单界面
    selectModeWidget->hide();
    boardWidget->show();  // 显示游戏界面
    backButton->show();
    reButton->show();
    gameMode=1;
    // 执行棋盘重置操作
    boardWidget->reset();
    boardWidget->updateLabels();
    QMessageBox::information(this, "Game Started", "PvP game has started! Happy Gaming!");
}

void MainWindow::startPvAIGame(){
    selectModeWidget->hide();
    selectColorWidget->show();
}

void MainWindow::startBlack(){
    selectColorWidget->hide();
    gameMode=2;
    boardWidget->show();
    backButton->show();
    reButton->show();
    boardWidget->reset();
    boardWidget->updateLabels();
    QMessageBox::information(this, "Game Started", "PvAI game has started! You've picked the Black side. Happy Gaming!");
}

void MainWindow::startWhite(){
    selectColorWidget->hide();
    gameMode=3;
    boardWidget->show();
    backButton->show();
    reButton->show();
    boardWidget->reset();
    Round++;
    gridCondition[4][4]=1;
    gridCondition[4][5]=1;
    blackStone=4;
    whiteStone=1;
    boardWidget->update();
    boardWidget->updateLabels();
    QMessageBox::information(this, "Game Started", "PvAI game has started! You've picked the White side. Happy Gaming!");
}

void MainWindow::back() {
    if (blackStone+whiteStone!=64){
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Save Game", "Do you want to save the game?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

    if (reply == QMessageBox::Yes) {
        // 用户选择保存，执行保存操作
        saveGame();
        boardWidget->hide();
        selectModeWidget->hide();
        mainMenuWidget->show();
        backButton->hide();
        reButton->hide();
    } else if (reply == QMessageBox::No) {
        // 用户选择不保存，直接返回主菜单
        boardWidget->hide();
        selectModeWidget->hide();
        mainMenuWidget->show();
        backButton->hide();
        reButton->hide();
    } else {
        // 用户选择取消，不执行任何操作
    }
    }
    else {
        boardWidget->hide();
        selectModeWidget->hide();
        mainMenuWidget->show();
        backButton->hide();
        reButton->hide();
        boardWidget->reset();
    }
}

void MainWindow::back2(){
    boardWidget->hide();
    selectModeWidget->hide();
    mainMenuWidget->show();
}

void MainWindow::mute(){
    musicPlayer->stop();
}

void MainWindow::unmute(){
    musicPlayer->play();
}

// 槽函数，用于检测歌曲播放状态
void MainWindow::onMediaStatusChanged(QMediaPlayer::MediaStatus status) {
    if (status == QMediaPlayer::EndOfMedia) {
        // 歌曲播放完毕，重新开始播放
        musicPlayer->play();
    }
}

void MainWindow::re(){
    if(gameMode==3){
        boardWidget->reset();
        Round++;
        gridCondition[4][4]=1;
        gridCondition[4][5]=1;
        blackStone=4;
        whiteStone=1;
        boardWidget->update();
        boardWidget->updateLabels();
    }
    else{
        boardWidget->reset();
        boardWidget->update();
        boardWidget->updateLabels();
    }
    QMessageBox::information(this, "Game Re-started", "Game re-started. Happy Gaming!");
}

void MainWindow::showEvent(QShowEvent *event) {
    QMainWindow::showEvent(event);

    static bool splashShown = false;
    if (!splashShown) {
        // 显示开场动画
        showSplashScreen();
        splashShown = true;
    }
}

void MainWindow::showSplashScreen() {
    QLabel *splashLabel = new QLabel(this);
    QPixmap pixmap1("/Users/zhangwenhao/Reversi/PKU.JPG");

    QPixmap scaledPixmap1 = pixmap1.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    splashLabel->setPixmap(scaledPixmap1);
    splashLabel->setScaledContents(true);
    splashLabel->setFixedSize(this->size());
    splashLabel->setAlignment(Qt::AlignCenter);
    splashLabel->show();
    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(splashLabel);
    splashLabel->setGraphicsEffect(effect);
    effect->setOpacity(1.0); // 初始为完全不透明

    QPropertyAnimation *fadeOut = new QPropertyAnimation(effect, "opacity");
    fadeOut->setDuration(4000);
    fadeOut->setStartValue(1.0);
    fadeOut->setEndValue(0.0);

    // 在图片显示一段时间后开始淡出
    QTimer::singleShot(4000, this, [fadeOut]() {
        fadeOut->start();
    });

    // 在淡出动画结束后关闭标签
    connect(fadeOut, &QPropertyAnimation::finished, splashLabel, &QLabel::close);
}
