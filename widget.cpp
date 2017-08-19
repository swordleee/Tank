#include "widget.h"
#include "ui_widget.h"
#include<QDebug>
#include<ctime>
#include<cstring>
#include<QIcon>



Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    setWindowTitle("坦克大战");
    setWindowIcon(QIcon(":/new/image/enemyU.png"));

    this->setGeometry(200,100,(W+4)*cell,(H+4)*cell);

    wallStyle=0;
    isStart=false;
    //设置按钮
    drawBack=new QPushButton("绘制地图",this);
    drawBack->move(50,520);
    startGame=new QPushButton("开始游戏",this);
    startGame->move(150,520);

    wall=new QPushButton(this);
    wall->setIcon(QIcon(":/new/image/walls.jpg"));
    wall->setEnabled(false);
    wall->move(300,520);

    steel=new QPushButton(this);
    steel->setIcon(QIcon(":/new/image/steel.png"));
    steel->setEnabled(false);
    steel->move(350,520);

    connect(drawBack,&QPushButton::clicked,this,&Widget::drawBackClick);
    connect(startGame,&QPushButton::clicked,this,&Widget::startGameClick);
    connect(wall,&QPushButton::clicked,this,&Widget::wallClick);
    connect(steel,&QPushButton::clicked,this,&Widget::steelClick);

    timerId1=-1;
    timerId2=-1;
    timerId3=-1;

    //初始化二维数组
    memset(stone,0,sizeof(stone));
    //创建一个我方坦克
    mytank=new MyTank(W/2-4,H-2,1);
    mytank2=new MyTank(W/2+2,H-2,2);
    //创建老家
    home=new Home(W/2-2,H-3);
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<4;j++)
        {
            if(!(j>0&&j<3&&i>0))
            {
                stone[home->hx+j][home->hy+i]=3;//将老窝四周的砖置为3
            }else
                stone[home->hx+j][home->hy+i]=4;//老巢置为4
        }
    }
    //创建15敌方坦克
    ti=new QTimer(this);

    connect(ti,&QTimer::timeout,[=]()
    {
        EnemyTank enTank;
        enemyTank.push_back(enTank);
        if(enemyTank.size()==15)
        {
            ti->stop();
        }
        if(enemyTank.size()!=0)
        {
            if(timerId3==-1)
                timerId3=startTimer(500);
            if(timerId1==-1)
                timerId1=startTimer(1000);
        }
        update();

    });
    tenbu=new QTimer(this);
    connect(tenbu,&QTimer::timeout,[=]()
    {
        for(auto it=enemyBullet.begin();it!=enemyBullet.end();it++)
        {
            switch (it->state) {
            case 1:
                it->by-=5;
                break;
            case 2:
                it->by+=5;
                break;
            case 3:
                it->bx-=5;
                break;
            case 4:
                it->bx+=5;
                break;
            default:
                break;
            }
        }
        update();
    });


}


Widget::~Widget()
{
    if(mytank!=NULL)
        delete mytank;
    if(mytank2!=NULL)
        delete mytank2;
    if(home!=NULL)
        delete home;
    delete ui;
}
//重绘事件
void Widget::paintEvent(QPaintEvent *)
{
    MyTankIsDestroy(mytank);
    EnemyTankIsDestroy(enemyTank);
    BulletIsDestroy(enemyBullet);

    QPainter p(this);
    //画背景色
    QBrush br(Qt::black);
    p.setBrush(br);
    p.setPen(QPen());
    p.drawRect(0,0,W*cell,H*cell);
    //画我方坦克
    drawTank(p);
    //画老窝
    drawHome(p);
    //画砖
    drawBrick(p);
    if(isStart==true)
    {
        //画敌方坦克
        drawEnemyTank(p);
        //画子弹
        br.setColor(Qt::white);
        p.setBrush(br);
        drawBullet(p);
        //画结束画面
        if(mytank->death==true||home->destroy==true)
        {
            p.setPen(QPen(Qt::white));
            p.setFont(QFont("宋体",100));
            p.drawText(50,H*cell/2,"GAME OVER");
        }
    }


}

//按下绘制背景按钮 按钮wall ，steel解锁
void Widget::drawBackClick()
{
    wall->setEnabled(true);
    steel->setEnabled(true);
}
void Widget::startGameClick()
{
    drawBack->setEnabled(false);
    isStart=true;
    ti->start(1000);
    startGame->setEnabled(false);
    wall->setEnabled(false);
    steel->setEnabled(false);
    wallStyle=0;
}

void Widget::wallClick()
{
    wallStyle=1;
}
void Widget::steelClick()
{
    wallStyle=2;
}

//鼠标按下事件
void Widget::mousePressEvent(QMouseEvent *ev)
{
    if(wallStyle==1)
    {
        stone[ev->x()/cell][ev->y()/cell]=1;
    }else if(wallStyle==2)
    {
        stone[ev->x()/cell][ev->y()/cell]=2;
    }
    else
    {
        return;
    }
    update();
}

//画坦克
void Widget::drawTank(QPainter &p)
{
    switch (mytank->state) {
    case 1:
         p.drawPixmap(mytank->tx*cell,mytank->ty*cell,2*cell,2*cell,QPixmap(":/new/image/p1tankU.png"));
        break;
    case 2:
         p.drawPixmap(mytank->tx*cell,mytank->ty*cell,2*cell,2*cell,QPixmap(":/new/image/p1tankD.png"));
        break;
    case 3:
         p.drawPixmap(mytank->tx*cell,mytank->ty*cell,2*cell,2*cell,QPixmap(":/new/image/p1tankL.png"));
        break;
    case 4:
         p.drawPixmap(mytank->tx*cell,mytank->ty*cell,2*cell,2*cell,QPixmap(":/new/image/p1tankR.png"));
        break;
    default:
        break;
    }
    switch (mytank2->state) {
    case 1:
        p.drawPixmap(mytank2->tx*cell,mytank2->ty*cell,2*cell,2*cell,QPixmap(":/new/image/p2tankU.png"));
        break;
    case 2:
        p.drawPixmap(mytank2->tx*cell,mytank2->ty*cell,2*cell,2*cell,QPixmap(":/new/image/p2tankD.png"));
        break;
    case 3:
        p.drawPixmap(mytank2->tx*cell,mytank2->ty*cell,2*cell,2*cell,QPixmap(":/new/image/p2tankL.png"));
        break;
    case 4:
        p.drawPixmap(mytank2->tx*cell,mytank2->ty*cell,2*cell,2*cell,QPixmap(":/new/image/p2tankR.png"));
        break;
    default:
        break;
    }
}
//画敌方坦克
void Widget::drawEnemyTank(QPainter &p)
{
    for(auto it=enemyTank.begin();it!=enemyTank.end();it++)
    {
        switch (it->eState) {
        case 1:
             p.drawPixmap(it->ex*cell,it->ey*cell,2*cell,2*cell,QPixmap(":/new/image/enemyU.png"));
            break;
        case 2:
             p.drawPixmap(it->ex*cell,it->ey*cell,2*cell,2*cell,QPixmap(":/new/image/enemyD.png"));
            break;
        case 3:
             p.drawPixmap(it->ex*cell,it->ey*cell,2*cell,2*cell,QPixmap(":/new/image/enemyL.png"));
            break;
        case 4:
             p.drawPixmap(it->ex*cell,it->ey*cell,2*cell,2*cell,QPixmap(":/new/image/enemyR.png"));
            break;
        default:
            break;
        }

    }
}

//画老窝
void Widget::drawHome(QPainter &p)
{
    p.drawPixmap((home->hx+1)*cell,(home->hy+1)*cell,2*cell,2*cell,QPixmap(":/new/image/symbol.png"));
}
//画砖
void Widget::drawBrick(QPainter &p)
{
    for(int i=0;i<W;i++)
    {
        for(int j=0;j<H;j++)
        {
            if(stone[i][j]==1)
                p.drawPixmap(i*cell,j*cell,cell,cell,QPixmap(":/new/image/walls.jpg"));
            if(stone[i][j]==2)
                p.drawPixmap(i*cell,j*cell,cell,cell,QPixmap(":/new/image/steel.png"));
            if(stone[i][j]==3)
                p.drawPixmap(i*cell,j*cell,cell,cell,QPixmap(":/new/image/walls.jpg"));
        }
    }
}
//画子弹
void Widget::drawBullet(QPainter &p)
{
    for(auto it=myBullet.begin();it!=myBullet.end();it++)
    {
        p.drawEllipse(it->bx-2,it->by-2,4,4);
        //qDebug()<<it->bx<<" "<<it->by;
    }
    for(auto it=enemyBullet.begin();it!=enemyBullet.end();it++)
    {
        p.drawEllipse(it->bx-2,it->by-2,4,4);
        //qDebug()<<it->bx<<" "<<it->by;
    }
}

//键盘按下事件
void Widget::keyPressEvent(QKeyEvent *ev)
{
    if(isStart==false)
        return;
    if(ev->key()==Qt::Key_Up)
    {
        mytank->state=1;
        mytank->ty--;
        if(myTankIsMove(mytank)==false)
        {
            mytank->ty++;
        }
    }else if(ev->key()==Qt::Key_Down)
    {
        mytank->state=2;
        mytank->ty++;
        if(myTankIsMove(mytank)==false)
        {
            mytank->ty--;
        }
    }else if(ev->key()==Qt::Key_Left)
    {
        mytank->state=3;
        mytank->tx--;
        if(myTankIsMove(mytank)==false)
        {
            mytank->tx++;
        }
    }else if(ev->key()==Qt::Key_Right)
    {
        mytank->state=4;
        mytank->tx++;
        if(myTankIsMove(mytank)==false)
        {
            mytank->tx--;
        }
    }else if(ev->key()==Qt::Key_Space)
    {
        switch (mytank->state) {
        case 1:
        {
            Bullet b1((mytank->tx+1)*cell,mytank->ty*cell,mytank->state);
            myBullet.push_back(b1);
        }
            break;
        case 2:
        {
            Bullet b2((mytank->tx+1)*cell,(mytank->ty+2)*cell,mytank->state);
            myBullet.push_back(b2);
        }
            break;
        case 3:
        {
            Bullet b3(mytank->tx*cell,(mytank->ty+1)*cell,mytank->state);
            myBullet.push_back(b3);
        }
            break;
        case 4:
        {
            Bullet b4((mytank->tx+2)*cell,(mytank->ty+1)*cell,mytank->state);
            myBullet.push_back(b4);
        }
            break;
        default:
            break;
        }
    }
    if(timerId2==-1)
        timerId2=startTimer(50);
    update();

}
//定时器事件
void Widget::timerEvent(QTimerEvent *ev)
{
    if(ev->timerId()==timerId1)
    {
        for(auto it=enemyTank.begin();it!=enemyTank.end();it++)
        {
            switch (it->eState) {
            case 1:
            {
                Bullet b((it->ex+1)*cell,it->ey*cell,it->eState);
                enemyBullet.push_back(b);
            }
                break;
            case 2:
            {
                Bullet b((it->ex+1)*cell,(it->ey+2)*cell,it->eState);
                enemyBullet.push_back(b);
            }
                break;
            case 3:
            {
                Bullet b(it->ex*cell,(it->ey+1)*cell,it->eState);
                enemyBullet.push_back(b);
            }
                break;
            case 4:
            {
                Bullet b((it->ex+2)*cell,(it->ey+1)*cell,it->eState);
                enemyBullet.push_back(b);
            }
                break;
            default:
                break;
            }
            if(enemyBullet.empty()==false)
            {
                tenbu->start(50);
            }
        }
    }
    if(ev->timerId()==timerId2)
    {
        for(auto it=myBullet.begin();it!=myBullet.end();it++)
        {
            switch (it->state) {
            case 1:
                it->by-=5;
                break;
            case 2:
                it->by+=5;
                break;
            case 3:
                it->bx-=5;
                break;
            case 4:
                it->bx+=5;
                break;
            default:
                break;
            }
        }
        BulletIsDestroy(myBullet);
    }
    if(ev->timerId()==timerId3)
    {
        srand((unsigned int)time(NULL));
        for(auto it=enemyTank.begin();it!=enemyTank.end();it++)
        {
            switch (rand()%10) {
            case 1:
                it->eState=1;
                it->ey--;
                if(enTankIsMove(it)==false)
                    it->ey++;
                break;
            case 2:
                it->eState=2;
                it->ey++;
                if(enTankIsMove(it)==false)
                    it->ey--;
                break;
            case 3:
                it->eState=3;
                it->ex--;
                if(enTankIsMove(it)==false)
                    it->ex++;
                break;
            case 4:
                it->eState=4;
                it->ex++;
                if(enTankIsMove(it)==false)
                    it->ex--;
                break;
            default:
            {
                switch (it->eState) {
                case 1:
                    it->ey--;
                    if(enTankIsMove(it)==false)
                        it->ey++;
                    break;
                case 2:
                    it->ey++;
                    if(enTankIsMove(it)==false)
                        it->ey--;
                    break;
                case 3:
                    it->ex--;
                    if(enTankIsMove(it)==false)
                        it->ex++;
                    break;
                case 4:
                    it->ex++;
                    if(enTankIsMove(it)==false)
                        it->ex--;
                    break;
                default:
                    break;
                }
            }
                break;
            }

        }
    }
    update();

}
//判断子弹、砖、老窝是否销毁
void Widget::BulletIsDestroy(QVector<Bullet> &vb)
{
    for(auto it=vb.begin();it!=vb.end();)
    {
        if(it->bx<0||it->bx>W*cell||it->by<0||it->by>H*cell||it->destroy==true)
        {
           it=vb.erase(it);
        }else if(stone[it->bx/cell][it->by/cell]==1)
        {
            stone[it->bx/cell][it->by/cell]=0;
            it=vb.erase(it);
        }else if(stone[it->bx/cell][it->by/cell]==2)
        {
            it=vb.erase(it);
        }else if(stone[it->bx/cell][it->by/cell]==3)
        {
            stone[it->bx/cell][it->by/cell]=0;
            it=vb.erase(it);
        }
        else if(stone[it->bx/cell][it->by/cell]==4)
        {
            it=vb.erase(it);
            home->destroy=true;
        }else
        {
            it++;
        }
    }

}
//判断我方坦克是否销毁
void Widget::MyTankIsDestroy(MyTank *myt)
{
    for(auto it=enemyBullet.begin();it!=enemyBullet.end();it++)
    {
        if(it->bx>=myt->tx*cell&&it->bx<=(myt->tx+2)*cell&&
                it->by>=myt->ty*cell&&it->by<=(myt->ty+2)*cell)
        {
            myt->death=true;
            //qDebug()<<myt->death;
        }
    }
}

//判断敌方坦克是否销毁
void Widget::EnemyTankIsDestroy(QVector<EnemyTank> &ve)
{
    for(auto itb=myBullet.begin();itb!=myBullet.end();itb++)
    {
        for(auto it=ve.begin();it!=ve.end();)
        {
            if(itb->bx>=it->ex*cell&&itb->bx<=(it->ex+2)*cell&&
                    itb->by>=it->ey*cell&&itb->by<=(it->ey+2)*cell)
            {
               it=ve.erase(it);
               itb->destroy=true;
            }else
            {
                it++;
            }
        }
    }

}
//判断我方坦克移动范围
bool Widget::myTankIsMove(MyTank *myt)
{
    if(myt->tx<0||myt->tx+2>W||myt->ty<0||myt->ty+2>H)
    {
        return false;
    }
    switch (myt->state) {
    case 1:
        if(myTIsOk(myt->tx,myt->ty,myt)&&
        myTIsOk(myt->tx+1,myt->ty,myt))return true;
        break;
    case 2:
        if(myTIsOk(myt->tx,myt->ty+1,myt)&&
        myTIsOk(myt->tx+1,myt->ty+1,myt))return true;
        break;
    case 3:
        if(myTIsOk(myt->tx,myt->ty,myt)&&
        myTIsOk(myt->tx,myt->ty+1,myt))return true;
        break;
    case 4:
        if(myTIsOk(myt->tx+1,myt->ty,myt)&&
        myTIsOk(myt->tx+1,myt->ty+1,myt))return true;
        break;
    default:
        break;
    }
    return false;
}
bool Widget::myTIsOk(int x,int y,MyTank *myt)
{

    if(myt==mytank)
    {
        if(x>=mytank2->tx&&x<=mytank2->tx+1&&
                y>=mytank2->ty&&y<=mytank2->ty+1)
            return false;
    }else
    {
        if(x>=mytank->tx&&x<=mytank->tx+1&&
                y>=mytank->ty&&y<=mytank->ty+1)
            return false;
    }

    for(auto it=enemyTank.begin();it!=enemyTank.end();it++)
    {
        if(x>=it->ex&&x<=it->ex+1&&
                y>=it->ey&&y<=it->ey+1)
            return false;
    }
    if(stone[x][y]==1||stone[x][y]==2||stone[x][y]==3)
    {
        return false;
    }
    return true;
}

//判断敌方坦克移动范围
bool Widget::enTankIsMove(EnemyTank *ent)
{
    if(ent->ex<0||ent->ex+2>W||ent->ey<0||ent->ey+2>H)
    {
        return false;
    }
    switch (ent->eState) {
    case 1:
        if(enTIsOk(ent->ex,ent->ey,ent)&&
        enTIsOk(ent->ex+1,ent->ey,ent)) return true;
        break;
    case 2:
        if(enTIsOk(ent->ex,ent->ey+1,ent)&&
        enTIsOk(ent->ex+1,ent->ey+1,ent)) return true;
        break;
    case 3:
        if(enTIsOk(ent->ex,ent->ey,ent)&&
        enTIsOk(ent->ex,ent->ey+1,ent)) return true;
        break;
    case 4:
        if(enTIsOk(ent->ex+1,ent->ey,ent)&&
        enTIsOk(ent->ex+1,ent->ey+1,ent)) return true;
        break;
    default:
        break;
    }
    return false;
}
bool Widget::enTIsOk(int x,int y,EnemyTank *ent)
{

    if(x>=mytank->tx&&x<=mytank->tx+1&&
            y>=mytank->ty&&y<=mytank->ty+1)
        return false;
    if(x>=mytank2->tx&&x<=mytank2->tx+1&&
            y>=mytank2->ty&&y<=mytank2->ty+1)
        return false;
    for(auto it=enemyTank.begin();it!=enemyTank.end();it++)
    {
        if(ent==it) continue;
        if(x>=it->ex&&x<=it->ex+1&&
                y>=it->ey&&y<=it->ey+1)
            return false;
    }
    if(stone[x][y]==1||stone[x][y]==2||stone[x][y]==3)
    {
        return false;
    }
    return true;
}
