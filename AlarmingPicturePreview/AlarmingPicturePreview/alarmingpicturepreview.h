#ifndef ALARMINGPICTUREPREVIEW_H
#define ALARMINGPICTUREPREVIEW_H

#include <QWidget>

namespace Ui {
class AlarmingPicturePreview;
}

class AlarmingPicturePreview : public QWidget
{
    Q_OBJECT

public:
    explicit AlarmingPicturePreview(QWidget *parent = 0);
    ~AlarmingPicturePreview();

private:
    Ui::AlarmingPicturePreview *ui;
};

#endif // ALARMINGPICTUREPREVIEW_H
