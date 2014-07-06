#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QTextEdit>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    //Ui::MainWindow *ui;

private:
    void open();
    void openFile();
    void saveFile();

    QAction *openAction;
    QAction *saveAction;
    QTextEdit *textEdit;
};

#endif // MAINWINDOW_H
