#ifndef TESTPIXMAP_H
#define TESTPIXMAP_H

#include <QMainWindow>
#include <QLabel>

namespace Ui {
class TestPixmap;
}

class TestPixmap : public QMainWindow
{
    Q_OBJECT

public:
    explicit TestPixmap(QWidget *parent = 0);
    ~TestPixmap();

private:
    Ui::TestPixmap *ui;
    QLabel *mp_label_icon;
    QLabel *m_pLabelTitle;
};

#endif // TESTPIXMAP_H
