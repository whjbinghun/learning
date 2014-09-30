#ifndef GROUPNUMVIEW_H
#define GROUPNUMVIEW_H

#include <QListView>
#include <QLabel>

class SwitchWinNum;

class GroupNumView : public QListView
{
    Q_OBJECT
public:
    explicit GroupNumView(QWidget *parent = 0);
public:
    void mouseMoveEvent( QMouseEvent *event );
    void mouseReleaseEvent( QMouseEvent *event );
    void set_wnd( QLabel *p_lab, SwitchWinNum *p_switch_win );
    void set_press( bool b_press );
    bool get_press();
signals:

public slots:
    void slot_pressed( QModelIndex model_index );
private:
    SwitchWinNum *mp_switchwin;
    QLabel *mp_lab_move;
    QString ms_group_name;
    bool mb_press;
};

#endif // GROUPNUMVIEW_H
