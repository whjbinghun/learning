#ifndef STANDBYADVERTWIDGET_H
#define STANDBYADVERTWIDGET_H

#include <QWidget>
#include <QMap>
#include <QTimer>

namespace Ui {
class StandbyAdvertWidget;
}

class StandbyAdvertWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StandbyAdvertWidget(QWidget *parent = 0);
    ~StandbyAdvertWidget();
public:
    void setFileList( QList<QString> listFilePath );
    void setFileList( QMap<int, QString> mapFilePath );
    bool getFileList( QList<QString> &listFilePath );

    void paintEvent(QPaintEvent *event);

    void startTimer();
    void stopTimer();
private:
    void initCtrl();


private slots:
    void slot_updateImage();
private:
    Ui::StandbyAdvertWidget *ui;

    QMap<int, QString> m_mapFilePath;

    QTimer *m_pTimer;
    int m_nTimerId;

    QImage m_image;
    int m_nCurFilePos;
};

#endif // STANDBYADVERTWIDGET_H
