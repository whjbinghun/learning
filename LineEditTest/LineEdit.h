#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <QLineEdit>

class LineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit LineEdit(QWidget *parent = 0);

private slots:
    void slot_cursorPositionChanged( int nOldPosition, int nNewPosition );
private:
    void initCtrl();
    void initConnect();
private:

};

#endif // LINEEDIT_H
