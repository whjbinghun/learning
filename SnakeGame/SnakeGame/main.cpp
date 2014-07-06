#include "snakegame.h"
#include <QApplication>

int main(int argc,char **argv){
   QApplication  app(argc,argv);
   SnakeGame  sg;
   sg.show();
   app.exec();
}
