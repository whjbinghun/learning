#include "CashBoxTest.h"
#include "ui_CashBoxTest.h"
#include <QLibrary>
#include <QMessageBox>

static const QString c_sKPosAdDll2Path = "pos_ad_dll.dll";
int m_nPrintPort = 5;
int m_nPrintBaud = 38400;
int m_nWeighPort = 2;
int m_nWeighBaud = 9600;

#define HS_OK 240
//打开钱箱
typedef int (* openCashDrawerEx)(void);

CashBoxTest::CashBoxTest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CashBoxTest)
{
    ui->setupUi(this);

    initCtrl();
    initConnect();
}

CashBoxTest::~CashBoxTest()
{
    delete ui;
}

void CashBoxTest::initCtrl()
{

}

void CashBoxTest::initConnect()
{
    connect( ui->OpenCashBoxBtn, SIGNAL(clicked()), this, SLOT( slot_openCashBox() ) );
    connect( ui->OpenCashBoxBtn2, SIGNAL(clicked()), this, SLOT( slot_openCashBox2() ) );
}

void CashBoxTest::slot_openCashBox()
{
    openCashBox();
}

void CashBoxTest::slot_openCashBox2()
{
    openCashBox2();
}

int CashBoxTest::openCashBox()
{
    QLibrary lib("pos_ad_dll.dll");
    if (!lib.load())
    {
        QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("动态库pos_ad_dll.dll加载失败"));
        return -1;
    }

    openCashDrawerEx openCashDrawer = (openCashDrawerEx)lib.resolve("OpenCashDrawerEx_stdcall");//获取该方法的地址
    int n = openCashDrawer();
    QMessageBox::information(NULL, QStringLiteral("提示"), "openCashDrawerEx_stdcall" + QString::number( n ) );
    return  n;
}

int CashBoxTest::openCashBox2()
{
    QLibrary lib( c_sKPosAdDll2Path );
    if (!lib.load())
    {
        QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("WinDrawer.dll加载失败"));
        return -1;
    }

    // 打开 AD 板串口。
    typedef bool (*OpenADCom)(int port, int baud);
    OpenADCom openADCom = (OpenADCom)lib.resolve("OpenADCom");
    bool bOpenSuccess = openADCom(m_nPrintPort, m_nPrintBaud);
    if (!bOpenSuccess)
    {
        QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("打印机端口打开失败"));
        return -1;
    }

    openCashDrawerEx openCashDrawer = (openCashDrawerEx)lib.resolve("OpenCashDrawerEx");//获取该方法的地址

    int n = openCashDrawer();//240为成功，255为失败
    QMessageBox::information(NULL, QStringLiteral("提示"), "openCashDrawerEx" + QString::number( n ) );


    typedef void (*CloseADCom)();
    CloseADCom closeADCom = (CloseADCom)lib.resolve("CloseADCom");
    closeADCom();

    return n;
}
