#include "md5test.h"
#include "ui_md5test.h"
#include "md5.h"
#include <QDebug>

Md5Test::Md5Test(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Md5Test)
{
    ui->setupUi(this);
    init_md5();
}

Md5Test::~Md5Test()
{
    delete ui;
}

void Md5Test::init_md5()
{
    char c_login_id[10] = "passwd";
    char *pc_login_id = c_login_id;
    char c_ascii[1024];
    char *pc_ascii = c_ascii;
    unsigned char c_puc[1024];
    unsigned char *pc_puc = c_puc;
    get_md5( pc_login_id, pc_ascii, pc_puc );
    qDebug()<<pc_ascii;
    for( int i=0; i<16; i++ ) {
        printf( "%02x", pc_puc[i] );
    }
}
