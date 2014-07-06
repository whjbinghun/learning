#ifndef DRAWTITLE_H
#define DRAWTITLE_H

#include <QMainWindow>
#include <QToolButton>
#include <QLabel>
#include <QHBoxLayout>

//其中pBtn代表被设置图片的按钮，而eBtnMoustState是一个枚举值，代表该按钮当前的状态，枚举定义如下所示：
//枚举，按钮状态
enum eBtnMoustState{
     eBtnStateNone,//无效
     eBtnStateDefault,//默认值(如按钮初始显示)
     eBtnStateHover,//鼠标移到按钮上状态
     eBtnStatePress//鼠标按下按钮时状态
 };

namespace Ui {
class DrawTitle;
}

class DrawTitle : public QMainWindow
{
    Q_OBJECT

public:
    explicit DrawTitle(QWidget *parent = 0);
    ~DrawTitle();

    void CreateWidget();
    void SetWidgetStyle();
    void CreateLayout();
    void SetBtnIcon(QToolButton *pBtn, eBtnMoustState state, bool bInit);
    const QString GetBtnImagePath(QToolButton *pBtn, bool bInit);
    void CreateEventFiter();
    bool eventFilter(QObject *obj, QEvent *event);
public slots:
    void slot_btnclick();
private:
    Ui::DrawTitle *ui;
    QLabel *mp_label_icon;
    QLabel *mp_label_title;
    QLabel *mp_label_version;
    QToolButton *mp_btn_skin;
    QToolButton *mp_btn_menu;
    QToolButton *mp_btn_min;
    QToolButton *m_pBtnMax;
    QToolButton *m_pBtnClose;
    QHBoxLayout *m_pLayout;

};

#endif // DRAWTITLE_H
