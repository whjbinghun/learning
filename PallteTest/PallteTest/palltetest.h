#ifndef PALLTETEST_H
#define PALLTETEST_H

#include <QMainWindow>

namespace Ui {
class PallteTest;
}

class PallteTest : public QMainWindow
{
    Q_OBJECT

public:
    explicit PallteTest(QWidget *parent = 0);
    ~PallteTest();
public:
    void init_pallte();
    void auto_send();
public slots:
    void slot_send_data();
    void slot_palette_change_clr( int clr );
    void slot_palette_auto();
    void slot_palette_change_range( float f_level, float f_levelspan );
private:
    Ui::PallteTest *ui;
    QTimer *mp_timer;
};

#endif // PALLTETEST_H
