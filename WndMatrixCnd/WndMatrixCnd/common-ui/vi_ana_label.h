#ifndef VI_ANA_LABEL_H
#define VI_ANA_LABEL_H

#include <QLabel>
#include <QMouseEvent>

class WndViPreview;

typedef struct _tagViBtnInfo
{
    int n_type;
    QString str_text;
    QString str_image;

    _tagViBtnInfo()
        :n_type(0)
        , str_text("")
        , str_image(""){}
} ViBtnInfo;

class Vi_Ana_Label : public QLabel
{
    Q_OBJECT
public:
    explicit Vi_Ana_Label(QWidget *parent = 0);
    ~Vi_Ana_Label();

public:

    void init_ir_widget();
    void paintEvent( QPaintEvent *event );
    void mousePressEvent( QMouseEvent *event );
    void mouseMoveEvent( QMouseEvent *event );
    void mouseReleaseEvent( QMouseEvent *event );
    void press_status_shape( QPoint pt );

    int get_mouse_press_status();
    void set_mouse_press_status( int n_press_status );

    void add_btn( QString str_txt, int n_type, QString str_image = "" );
    int get_wnd_width();

    void set_fullscreen_text();//设置全屏显示时，工具栏上的文本
    void set_fullscreen_status( bool b_fullscreen );//设置全屏显示时的状态
    bool get_fullscreen_status();
signals:

public slots:

private:
    QList<ViBtnInfo> m_list;
    WndViPreview *mp_vi_preview;
    int mn_press_status;

    bool mb_fullscreen_show;//全屏显示
};

#endif // VI_ANA_LABEL_H
