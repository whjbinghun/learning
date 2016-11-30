#include "LineEditTest.h"
#include "ui_LineEditTest.h"

LineEditTest::LineEditTest(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LineEditTest)
{
    ui->setupUi(this);

    initCtrl();
    initConnect();
}

LineEditTest::~LineEditTest()
{
    delete ui;
}

void LineEditTest::initCtrl()
{
    ui->birthdayDateEdit->setDisplayFormat( "yyyy.MM.dd" );
    ui->birthdayDateEdit->setButtonSymbols( QAbstractSpinBox::NoButtons );
}

void LineEditTest::initConnect()
{
    connect( ui->birthdayDateEdit, SIGNAL(dateChanged(QDate)), this, SLOT( slot_dateChanged() ) );
}
