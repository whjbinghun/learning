#include "picpreconfig.h"
#include <QDebug>

#define SPACE 20
#define IMAGE_WIDTH 100

PicPreConfig::PicPreConfig(QWidget *parent) :
    QWidget(parent)
    ,mp_pic_view( NULL )
    ,ms_move_image_name( "move image")
    ,mp_move_frame( NULL )
    ,mp_str_image_name( NULL )
    ,mp_list_show_info( NULL )
{

}

PicPreConfig::~PicPreConfig()
{

}

void PicPreConfig::mouseMoveEvent( QMouseEvent *event ) {

    /*if( *mp_press_status == true ) {
        if( event->x()>mp_move_frame->x() && event->x()<=mp_move_frame->x()+mp_move_frame->width() \
                && event->y()>mp_move_frame->y() && event->y()<=mp_move_frame->y()+mp_move_frame->height() ) {

        }
    }*/
    qDebug() << "PicPreConfig::mouseMoveEvent( QMouseEvent *event ) ";
}

void PicPreConfig::mousePressEvent( QMouseEvent *event ) {
    if ( event->button() == Qt::LeftButton ) {
        qDebug()<<"global"<<m_pt_press_global<<m_pt_press_global.x()<<m_pt_press_global.y()<<"event"<<event->x()<<event->y();
        if( event->x()>this->x() && event->x()<= this->x()+this->width() && event->y()>this->y() && event->y()<=this->y()+this->height() ) {
            set_frame( event );
        }
    }

    qDebug()<<"PicPreConfig::mousePressEvent( QMouseEvent *event )"<<"点击";


}

void PicPreConfig::set_frame( QMouseEvent *event ) {
    int n_size = mp_pic_view->get_list_value()->size();
    QList<PicPreviewInfo>::iterator i;
    int j=0;
    for( i=mp_pic_view->get_list_value()->begin(), j=0; i!=mp_pic_view->get_list_value()->end(), j<n_size; i++, j++ ) {
        if( ( event->x()>( IMAGE_WIDTH+SPACE )*j+SPACE-mp_pic_view->get_scroll_len()+this->x() && event->x()<=IMAGE_WIDTH+SPACE )*j+SPACE-mp_pic_view->get_scroll_len() +IMAGE_WIDTH+this->x()&& \
            ( event->y()>SPACE+this->y() && event->y()<= SPACE+height()-SPACE*3+this->y() ) ) {
            ms_move_image_name = (*i).str_filename;
            //mb_mouse_press = true;
            update();
        }
        //ms_image_name = 1;

    }

}

void PicPreConfig::mouseReleaseEvent( QMouseEvent *event ) {
    if( mp_pic_view->get_press_status() ) {
        if( event->x()>mp_move_frame->x() && event->x()<=mp_move_frame->x()+mp_move_frame->width() \
                && event->y()>mp_move_frame->y() && event->y()<=mp_move_frame->y()+mp_move_frame->height() ) {
            //那么把list中的图片，放入当前位置
            int n_size = mp_pic_view->get_list_value()->size();
            QList<PicPreviewInfo>::iterator i;
            int j=0;
            int n_show_size = mp_list_show_info->size();
            QList<PicPreviewShowInfo>::iterator i_show;
            int j_show=0;
            for( i=mp_pic_view->get_list_value()->begin(), j=0; i!=mp_pic_view->get_list_value()->end(), j<n_size; i++,j++ ) {
                if( *mp_str_image_name == i->str_filename ) {
                    i_show = mp_list_show_info->begin();
                    i_show->p_image = i->p_image;
                    i_show->str_filename = i->str_filename;
                    i_show->str_path = i->str_path;
                    i_show->str_sn = i->str_sn;
                    qDebug()<<"list_show"<<*mp_str_image_name<<i->str_filename;
                    update();
                    mp_pic_view->set_press_status( false );
                    break;
                }
            }
        }
        mp_pic_view->set_press_status( false );
    }
}

void PicPreConfig::set_view( PicPreview *pic_pre_view, QFrame *move_frame, QString* ps_image_name, QList<PicPreviewShowInfo> *p_show_list_info )
{
    mp_pic_view = pic_pre_view;
    mp_move_frame = move_frame;
    mp_str_image_name = ps_image_name;
    mp_list_show_info = p_show_list_info;
}

