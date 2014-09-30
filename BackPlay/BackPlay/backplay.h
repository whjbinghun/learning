#ifndef BACKPLAY_H
#define BACKPLAY_H

#include <QMainWindow>
#include <QCalendarWidget>
#include <QGridLayout>

namespace Ui {
class BackPlay;
}

class BackPlay : public QMainWindow
{
    Q_OBJECT

public:
    explicit BackPlay(QWidget *parent = 0);
    ~BackPlay();
public:
    void createPreviewGroupBox();
    void resizeEvent( QResizeEvent *event );
    void paintEvent( QPaintEvent *event );
public slots:
    void selectedDateChanged();
private:
    Ui::BackPlay *ui;
    QCalendarWidget *mp_calendar;
    QGridLayout *mp_grid_layout;
};

#endif // BACKPLAY_H
