#include "LineEdit.h"
#include <QDebug>

LineEdit::LineEdit(QWidget *parent)
    : QLineEdit(parent)
{
    initCtrl();
    initConnect();
}

void LineEdit::slot_cursorPositionChanged( int nOldPosition, int nNewPosition )
{
    qDebug()<<"LineEdit::slot_cursorPositionChanged"<< nOldPosition<<nNewPosition;
}

void LineEdit::initCtrl()
{

}

void LineEdit::initConnect()
{
    //setCursorPosition() and cursorPosition().
    // cursorPositionChanged(int,int) ：当光标移动时，这个信号被发出。前一位置是旧的，新的位置由新的。
    connect( this, SIGNAL(cursorPositionChanged(int,int)), this, SLOT( slot_cursorPositionChanged(int,int) ) );
}

