#include "snakegame.h"
/*
QList<QLabel*>  datas;
QLabel  *  food;
DIRE      dire;
QTimer  *timer;
int          footLen;
int          maxLen;
*/
#include <QTime>
SnakeGame::SnakeGame(){
    this->resize(700,600);
    //qsrand 随机函数
    qsrand(QTime::currentTime().msec());
    //枚举
    dire=D_RIGHT;
    timer=new QTimer();
    timer->setInterval(200);
    timer->start();
    footLen=30;
    maxLen=5;
    datas.push_back(getFood());
    food=getFood();
    //
    connect(timer,SIGNAL(timeout()),
            this,SLOT(snakeMove()));
}
SnakeGame::~SnakeGame(){

}
/*定时调用的槽函数  函数根据方向改变位置*/

#include <QtWidgets/QMessageBox>
void  SnakeGame::snakeMove(){
    /*蛇头原来的位置*/
    int  x=datas[0]->x();
    int  y=datas[0]->y();
    /*吃食物*/
    if(x<0 || y <0 ||x>(this->width())||
       y>(this->height())){
       QMessageBox   msg;
       msg.setText("game over!!");
       msg.setStandardButtons(QMessageBox::Yes);
       if(msg.exec()==QMessageBox::Yes){
            this->close();
       }
    }
    if(food->x()==x && food->y()==y){
       datas.push_back(food);
       food=getFood();
    }
    /*食物跟着蛇头跑  datas.size()
    后边的跟着前面的跑*/
    for(int i=datas.size()-1;i>0;i--){
         datas[i]->move(datas[i-1]->x(),
                                   datas[i-1]->y());
    }
    /*根据方向改变蛇头位置*/
    switch(dire){
    case D_UP:
        y=y-footLen; break;
    case D_DOWN:
        y=y+footLen;break;
    case D_LEFT:
        x=x-footLen; break;
    case D_RIGHT:
        x=x+footLen;break;
    }
    /*蛇头移动到新位置*/
    datas[0]->move(x,y);
}
#include <QPalette>
#include <QColor>
QLabel * SnakeGame::getFood(){
    food=new QLabel(this);
    food->resize(footLen,footLen);
    /*给食物涂颜色*/
    food->setFrameShape(QFrame::Box);
    food->setAutoFillBackground(true);
    food->setPalette(QPalette(QColor(qrand()%255,
                                     qrand()%255,qrand()%255)));
    food->show();
    /*食物的位置要随机  界面范围内  footLen
   的整数倍*/
    int   w=this->width()-footLen;
    int   h=this->height()-footLen;
    food->move((qrand()%w)/footLen*footLen
                      ,(qrand()%h)/footLen*footLen);
    return food;
}
/*键盘事件 控制方向*/
#include <QKeyEvent>
void  SnakeGame::keyPressEvent(QKeyEvent *e){
    switch(e->key()){
    case  Qt::Key_Up:
        dire=D_UP;
        break;
    case  Qt::Key_Down:
        dire=D_DOWN;
        break;
    case  Qt::Key_Left:
        dire=D_LEFT;
        break;
    case  Qt::Key_Right:
        dire=D_RIGHT;
        break;
    }
}
