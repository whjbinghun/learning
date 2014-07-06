#ifndef SNAKEGAME_H
#define SNAKEGAME_H
#include <QList>
#include <QLabel>
#include <QDialog>
#include  <QTimer>

enum  DIRE{D_UP,D_DOWN,D_LEFT,D_RIGHT};
class  SnakeGame:public  QDialog{
    Q_OBJECT
private:
    QList<QLabel*>  datas;
    QLabel  *  food;
    DIRE      dire;
    QTimer  *timer;
    int          footLen;
    int          maxLen;
public:
    SnakeGame();
    ~SnakeGame();
 /*定时调用的槽函数*/
public slots:
    void  snakeMove();
public:
    QLabel * getFood();
    /*键盘事件 控制方向*/
    void  keyPressEvent(QKeyEvent *e);
};
#endif // SNAKEGAME_H
