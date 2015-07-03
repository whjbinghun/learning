#include "wndgroupbox.h"
#include <QLineEdit>

WndGroupBox::WndGroupBox( QWidget *parent )
    :QGroupBox( parent )
{
    QLineEdit *mp_line_edit = new QLineEdit( this );
    mp_line_edit->move( 10, 10 );
    mp_line_edit->resize( 100, 100 );
    //mp_line_edit->setStyleSheet("QLineEdit{ background-color: yellow }");
}

WndGroupBox::~WndGroupBox()
{

}
