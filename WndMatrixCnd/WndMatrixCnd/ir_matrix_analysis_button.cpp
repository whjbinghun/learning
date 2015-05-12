#include "ir_matrix_analysis_button.h"
#include <QMenu>
#include <QAction>
#include <QCursor>
#include <QPalette>
#include <QPainter>

Ir_Matrix_Analysis_Button::Ir_Matrix_Analysis_Button(QWidget *parent) :
    QPushButton(parent)
{

}

Ir_Matrix_Analysis_Button::Ir_Matrix_Analysis_Button(const QString title, QWidget *parent) :
    QPushButton(title, parent)
{
    // connect(this,SIGNAL(clicked()), this, SLOT(on_clicked_slot()));
    // setMenu(m_menu);
    //setStyleSheet(tr("font-color:rgb(50,50,50);"));
    setFlat(true);
//    init_menu();
   // setText(tr("阵列分析"));

//    QPalette pe;
//    pe.setColor(QPalette::WindowText,Qt::white);
//    this->setPalette(pe);

    //resize( width()-40, height());

}

void Ir_Matrix_Analysis_Button::paintEvent(QPaintEvent *event)
{
    QPainter draw;
    draw.begin( this );

    int n_btn_width = width();
    int n_btn_height = height();

    draw.fillRect( 0, 0, n_btn_width, n_btn_height, QBrush( QColor(50,50,50) ) );

    draw.setPen( Qt::white );
    draw.drawText( 0, 0, n_btn_width, n_btn_height, Qt::AlignCenter, text() );
    draw.drawRect( 0, 0, n_btn_width-1, n_btn_height-1 );

    draw.end();
}


