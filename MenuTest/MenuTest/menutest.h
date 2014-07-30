#ifndef MENUTEST_H
#define MENUTEST_H

#include <QMainWindow>
#include <QMenu>
#include <QAction>

namespace Ui {
class MenuTest;
}

class MenuTest : public QMainWindow
{
    Q_OBJECT

public:
    explicit MenuTest(QWidget *parent = 0);
    ~MenuTest();
public slots:
    void show_contextmenu( const QPoint& pos );
    void slot_display_analyse_interface( bool b_menu );
    void slot_display_back_interface( bool b_menu );
    void slot_display_analyse_change();
private:
    Ui::MenuTest *ui;
    QMenu *mp_menu;
    QAction *mp_action_analyse_widget;
    bool mb_menu_status;
};

#endif // MENUTEST_H
