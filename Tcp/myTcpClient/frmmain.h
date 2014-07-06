#ifndef FRMMAIN_H
#define FRMMAIN_H

#include <QWidget>
#include <QTcpSocket>

namespace Ui {
class frmMain;
}

class frmMain : public QWidget
{
    Q_OBJECT
    
public:
    explicit frmMain(QWidget *parent = 0);
    ~frmMain();
    
private slots:
    void on_btnConnect_clicked();

    void ReadData();
    void ReadError(QAbstractSocket::SocketError);

    void on_btnClear_clicked();

    void on_btnSend_clicked();

private:
    Ui::frmMain *ui;

    QTcpSocket *tcpClient;

    void Init();
};

#endif // FRMMAIN_H
