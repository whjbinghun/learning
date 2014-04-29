#include "mainwindow.h"
//#include "ui_mainwindow.h"
#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QToolBar>
#include <QTextEdit>
#include <QFileDialog>
#include <QTextStream>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)//,
    //ui(new Ui::MainWindow)
{
    //ui->setupUi(this);


    setWindowTitle(tr("Main Window"));

    openAction = new QAction(QIcon(":/images/file-open"), tr("&Open..."),this);
    openAction->setShortcuts(QKeySequence::Open);
    openAction->setStatusTip(tr("Open an existing file"));
    //connect(openAction, &QAction::triggered, this, &MainWindow::open);

    saveAction = new QAction(QIcon(":/images/file-save"), tr("&Save..."), this);
    saveAction->setShortcuts(QKeySequence::Save);
    saveAction->setStatusTip(tr("Save a new file"));
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);

    QMenu *file = menuBar()->addMenu(tr("&File"));
    file->addAction(openAction);

    QToolBar *toolBar = addToolBar(tr("&File"));
    toolBar->addAction(openAction);

    //QToolBar *toolBar2 = addToolBar(tr("Tool Bar 2"));
    //toolBar2->addAction(openAction);


    QStatusBar *status_bar = statusBar();
    status_bar->addAction(openAction);

    //文本编辑
    textEdit = new QTextEdit(this);
    setCentralWidget(textEdit);


}

MainWindow::~MainWindow()
{
    //delete ui;

}

void MainWindow::open() {
    if (QMessageBox::Yes == QMessageBox::question(this,tr("Question"),tr("Are you OK?"), \
                                         QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes)) {
        QMessageBox::information(this, tr("Hmmm..."), tr("I'm glad to hear that!"));
    } else {
        QMessageBox::information(this, tr("Hmmm..."), tr("I'm sorry!"));
    }
    //dialog建立在栈上
    /*//QDialog dialog(this);
    //QDialog dialog;
    dialog.setWindowTitle(tr("Hello, dialog!"));
    dialog.exec();
    //QMessageBox::information(this, tr( "Information" ), tr("Open"));
    */
    //dialog建立在堆上
    QDialog *dialog = new QDialog;
    //dialog 使用 new 在堆上分配空间, 却一直没有 delete。可以调用可以调用 deleteLater() 函数,或者是设置 dialog 的 WindowAttribute来delete dialog。
    dialog->setAttribute(Qt::WA_DeleteOnClose); // 或者 dialog->deleteLater();
    dialog->setWindowTitle(tr("hello, dialog!"));
    //dialog->show();
    dialog->open();

}

void MainWindow::openFile(){
    //使用 QFileDialog::getOpenFileName() 来获取需要打开的文件的路径。
    QString path = QFileDialog::getOpenFileName(this, tr("Open File"), ".", tr("Text Files(*.txt)"));

    if(!path.isEmpty()) {
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::warning(this, tr("Read File"), tr("Cannot open file:\n%1").arg(path));
            return;
        }
        QTextStream in(&file);
        textEdit->setText(in.readAll());
        file.close();

    } else {
        QMessageBox::warning(this, tr("Path"), tr("You did not select any file."));
    }

}

void MainWindow::saveFile() {
    QString path = QFileDialog::getSaveFileName(this, tr("Save File"), ".", tr("Text Files(*.txt)"));

    if(!path.isEmpty()) {
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::warning(this, tr("Write File"), tr("Cannot save file:\n%1").arg(path));
            return;
        }
        QTextStream out(&file);
        out << textEdit->toPlainText();
        file.close();
    } else {
        QMessageBox::warning(this, tr("Path"), tr("You did not select any file."));
    }

}

/*void MainWindow::showUserAgeDialog()
{
    UserAgeDialog *dialog = new UserAgeDialog(this);
    connect(dialog, &UserAgeDialog::userAgeChanged, this,&MainWindow::setUserAge);

    dialog->show();
}

void MainWindow::setUserAge(int age)
{
    userAge = age;
}
*/
