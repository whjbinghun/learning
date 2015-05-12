#ifndef IR_ANA_LABEL_H
#define IR_ANA_LABEL_H

#include <QLabel>
#include <QMouseEvent>
#include <QTimer>

typedef struct _tagBtnInfo
{
    int n_type;
    QString str_text;
    QString str_image;

    _tagBtnInfo()
        :n_type(0)
        , str_text("")
        , str_image(""){}
} BtnInfo;

class Ir_Ana_Label : public QLabel
{
    Q_OBJECT
public:
    explicit Ir_Ana_Label( QWidget *parent = 0, bool b_hide_warn = false, bool b_hide_calibration = false );
    ~Ir_Ana_Label();

public:
    void paintEvent( QPaintEvent *event );
    void resizeEvent( QResizeEvent *event );
    void mousePressEvent( QMouseEvent *event );
    void mouseMoveEvent( QMouseEvent *event );
    void mouseReleaseEvent( QMouseEvent *event );

    void init_ir_btn();
    void add_btn( QString str_txt, int n_type = 0, QString str_image = "" );
    void set_ir_ana_num( int n_num );
    int pt_in_btn( QPoint pt, BtnInfo &btn_info );

    int get_wnd_width();
    void clear_down_status();

    //更新按下按钮的位置
    int get_press_btn_pos();
    void set_press_btn_pos( int n_down_btn );
    void update_ir_ana();
signals:
    void sig_add_point();
    void sig_add_line();
    void sig_add_rect();
    void sig_modify_far();
    void sig_modify_near();
    void sig_modify_calibration();
    void sig_modify_warning();
    void sig_add_matrix_ana();
    void sig_clear_btn_click();

public slots:
    void slot_timer_focus_far_or_near();
private:
    QList< BtnInfo > m_list_btn;
    int mn_down_btn;
    bool mb_hide_warn;
    bool mb_hide_calibration;//隐藏拉近，拉远和校准按钮
    QTimer *mp_timer_focus;
    bool mb_focus_far;
};

#endif // IR_ANA_LABEL_H
