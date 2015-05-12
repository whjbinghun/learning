#ifndef IR_MATRIX_ANALYSIS_BUTTON_H
#define IR_MATRIX_ANALYSIS_BUTTON_H

#include <QPushButton>

class Ir_Matrix_Analysis_Button : public QPushButton
{
    Q_OBJECT
public:
    explicit Ir_Matrix_Analysis_Button(QWidget *parent=0);
    explicit Ir_Matrix_Analysis_Button( const QString title,QWidget *parent=0);
protected:
    void paintEvent( QPaintEvent *event );
};

#endif // IR_MATRIX_ANALYSIS_BUTTON_H
