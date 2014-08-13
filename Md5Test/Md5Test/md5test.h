#ifndef MD5TEST_H
#define MD5TEST_H

#include <QMainWindow>

namespace Ui {
class Md5Test;
}

class Md5Test : public QMainWindow
{
    Q_OBJECT

public:
    explicit Md5Test(QWidget *parent = 0);
    ~Md5Test();
public:
    void init_md5();
private:
    Ui::Md5Test *ui;
};

#endif // MD5TEST_H
