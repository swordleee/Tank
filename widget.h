#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QPainter>
#include<QVector>
#include<QKeyEvent>
#include<QTimer>
#include<QTimerEvent>
#include<QPushButton>
#include<QMouseEvent>
#include"mytank.h"
#include"home.h"
#include"bullet.h"
#include"enemytank.h"

#define W 36
#define H 24
#define cell 20

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void paintEvent(QPaintEvent *event);
    //画坦克
    void drawTank(QPainter &p);
    //画敌方坦克
    void drawEnemyTank(QPainter &p);
    //画墙
    void drawBrick(QPainter &p);
    //画老家
    void drawHome(QPainter &p);
    //画子弹
    void drawBullet(QPainter &p);
    //键盘按下事件
    void keyPressEvent(QKeyEvent *event);
    //定时器事件
    void timerEvent(QTimerEvent *event);
    //判定子弹是否销毁
    void BulletIsDestroy(QVector<Bullet> &vb);
    //判断我方坦克是否销毁
    void MyTankIsDestroy(MyTank *myt);
    //判断敌方坦克是否销毁
    void EnemyTankIsDestroy(QVector<EnemyTank> &ve);
    //判断我方坦克移动范围
    bool myTankIsMove(MyTank *myt);
    bool myTIsOk(int x,int y,MyTank *myt);
    //判断敌方坦克移动范围
    bool enTankIsMove(EnemyTank *ent);
    bool enTIsOk(int x,int y,EnemyTank *ent);
    //鼠标按下事件
    void mousePressEvent(QMouseEvent *event);
    //按钮事件
    void drawBackClick();
    void startGameClick();
    void wallClick();
    void steelClick();


private:
    Ui::Widget *ui;
    //定时器ID
    int timerId1;
    int timerId2;
    int timerId3;
    //创建一个坦克
    MyTank *mytank;
    MyTank *mytank2;
    //创建老家
    Home *home;
    //自己坦克发的子弹
    QVector<Bullet> myBullet;
    QVector<Bullet> myBullet2;
    //创建敌方坦克
    QVector<EnemyTank> enemyTank;
    QTimer *ti;//产生敌方坦克定时器
    //创建敌方坦克子弹
    QVector<Bullet> enemyBullet;
    QTimer *tenbu;//子弹运行定时器
    //定义一个数组放墙(1)、钢砖(2)、老窝四周（3）。。。
    int stone[W][H];
    //定义绘制地图按钮、开始游戏按钮、不同砖的按钮
    QPushButton *drawBack;
    QPushButton *startGame;
    QPushButton *wall;
    QPushButton *steel;
    //选择了那种墙
    int wallStyle;
    //开始游戏
    bool isStart;
};

#endif // WIDGET_H
