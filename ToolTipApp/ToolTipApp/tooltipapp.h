#ifndef TOOLTIPAPP_H
#define TOOLTIPAPP_H

#include <QMainWindow>
#include <QPoint>
#include <QMouseEvent>

namespace Ui {
class ToolTipApp;
}

class ToolTipApp : public QMainWindow
{
    Q_OBJECT

public:
    explicit ToolTipApp(QWidget *parent = 0);
    ~ToolTipApp();
    void mousePressEvent( QMouseEvent *event );
    void mouseMoveEvent( QMouseEvent *event );
    void mouseReleaseEvent( QMouseEvent *event );
    void set_tool_tile_text(QPoint pt_global, QPoint pt_local );
private:
    Ui::ToolTipApp *ui;
};

#endif // TOOLTIPAPP_H
