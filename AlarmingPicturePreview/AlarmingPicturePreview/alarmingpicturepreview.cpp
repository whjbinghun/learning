#include "alarmingpicturepreview.h"
#include "ui_alarmingpicturepreview.h"

AlarmingPicturePreview::AlarmingPicturePreview(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AlarmingPicturePreview)
{
    ui->setupUi(this);
}

AlarmingPicturePreview::~AlarmingPicturePreview()
{
    delete ui;
}
