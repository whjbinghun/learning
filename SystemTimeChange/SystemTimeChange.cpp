#include "SystemTimeChange.h"
#include "ui_SystemTimeChange.h"
#include <iostream>
#include <time.h>
#include <windows.h>
#include <QDebug>
using namespace std;

SystemTimeChange::SystemTimeChange(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SystemTimeChange)
{
    ui->setupUi(this);

    //时区
    putenv("TZ=UTC-0"); /*东8时区*/
    tzset();

    setDate( 2001, 1, 31 );
}

SystemTimeChange::~SystemTimeChange()
{
    delete ui;
}

bool SystemTimeChange::setDate(int year,int mon,int day)
{
    SYSTEMTIME st;
    GetSystemTime(&st);    // Win32 API 获取系统当前时间，并存入结构体st中
    st.wYear=year;
    st.wMonth=mon;
    st.wDay=day;

    return SetSystemTime(&st);    //Win32 API 设置系统时间
}
