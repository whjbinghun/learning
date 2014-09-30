#include "readwriteconfigfile.h"
#include <QApplication>
#include <QSettings>
#include <QDebug>
#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

QMap<QString, QString> m_ColumnMap;

void WriteSettings()
{
    //QSettings settings("Software Inc", "Spreadsheet"); // windows在注册表中建立建 Software Inc -> Spreadsheet
    QSettings settings("Option.ini", QSettings::IniFormat); // 当前目录的INI文件
    settings.setIniCodec( "UTF8" );
    settings.beginGroup("DevOption");
    settings.setValue("mainFun", 25);
    settings.setValue("subFun", 40);
    settings.setValue("service", 1);
    settings.endGroup();
}

void ReadSettings()
{
    QSettings settings("Option.ini", QSettings::IniFormat);

    int mainFun = settings.value("DevOption/mainFun").toInt();
    int subFun = settings.value("DevOption/subFun").toInt();
    QString service = settings.value("DevOption/service").toString();

    qDebug() << "service" << service;
    qDebug() << "mainFun" << mainFun;
    qDebug() << "subFun" << subFun;
}

/*bool SaveColumnJson()
{
    int count = m_ColumnMap.count();
    QJsonArray jsonArray;
    for(int i=0;i<count;i++)
    {
        QJsonObject jsonObject;
        jsonObject.insert("mainFun",m_ColumnMap[i].GetColumnText());
        jsonObject.insert("subFun",m_ColumnMap[i].IsShow());
        jsonObject.insert("service",m_ColumnMap[i].IsForbidHide());
        jsonObject.insert("isPicture",m_ColumnMap[i].IsPicture());
        jsonArray.append(jsonObject);
    }
    QJsonObject root;
    root.insert("columnSetting",jsonArray);
    QFile file(m_ColumnJsonFile);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text))
        return false;
    QTextStream stream(&file);
    QJsonDocument jsonDoc;
    jsonDoc.setObject(root);
    stream << jsonDoc.toJson();
    file.close();

    return true;
}

bool LoadColumnJson()
{

    QFile file( "E:/learning-master/ReadWriteConfigFile/Debug/Option.ini" );
    if(!file.exists())
        return false;
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
        return false;
    QTextStream stream(&file);
    QString json = stream.readAll();
    if(json.isEmpty())
        return false;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(json.toUtf8());

    QJsonObject jsonObject = jsonDoc.object();
    QJsonArray jsonArray = jsonObject.value("DevOption").toArray();

    int count = jsonArray.count();
    for(int i=0;i<count;i++)
    {
        QJsonObject columnObject = jsonArray[i].toObject();
        int mainFun = columnObject.value("mainFun").toInt();
        int subFun = columnObject.value("subFun").toInt();
        QString service = columnObject.value("service").toString();
        m_ColumnMap.insert(i,ColumnObject(mainFun,subFun,service));
        qDebug() << "service2" << service;
        qDebug() << "mainFun2" << mainFun;
        qDebug() << "subFun2" << subFun;
    }

    return true;
}
*/
/*Column.json的格式如下：
{
    "columnSetting": [
        {
            "columnText": "TradingCode",
            "isForbidHide": true,
            "isPicture": false,
            "isShow": true
        },
        {
            "columnText": "CurrentTime",
            "isForbidHide": false,
            "isPicture": false,
            "isShow": true
        },
        {
            "columnText": "R/F",
            "isForbidHide": true,
            "isPicture": true,
            "isShow": true
        },
        {
            "columnText": "CurrentBidPrice",
            "isForbidHide": false,
            "isPicture": false,
            "isShow": true
        },
        {
            "columnText": "CurrentAskPrice",
            "isForbidHide": false,
            "isPicture": false,
            "isShow": true
        },
        {
            "columnText": "PreviousBidPrice",
            "isForbidHide": false,
            "isPicture": false,
            "isShow": true
        },
        {
            "columnText": "PreviousAskPrice",
            "isForbidHide": false,
            "isPicture": false,
            "isShow": true
        },
        {
            "columnText": "PreviousHightPrice",
            "isForbidHide": false,
            "isPicture": false,
            "isShow": true
        },
        {
            "columnText": "PreviousLowPrice",
            "isForbidHide": false,
            "isPicture": false,
            "isShow": true
        },
        {
            "columnText": "PreviousClosePrice",
            "isForbidHide": false,
            "isPicture": false,
            "isShow": true
        },
        {
            "columnText": "PreviousChangedPrice",
            "isForbidHide": false,
            "isPicture": false,
            "isShow": true
        },
        {
            "columnText": "OpenPrice",
            "isForbidHide": false,
            "isPicture": false,
            "isShow": true
        },
        {
            "columnText": "BuyRate",
            "isForbidHide": false,
            "isPicture": false,
            "isShow": true
        },
        {
            "columnText": "SellRate",
            "isForbidHide": false,
            "isPicture": false,
            "isShow": true
        }
    ]
}

//读方式
 {
        QSettings set( str, QSettings::IniFormat );
        set.setIniCodec( "GB2312" );
        QString str_hao = set.value( "zu/hao").toString();
        qDebug() << str_hao;
    }
    //写方式，格式是GB2312
    {
        QSettings set( str, QSettings::IniFormat );
        set.setIniCodec( "GB2312" );
        set.beginGroup( "zu" );
        set.setValue( "hao", "组号测试" );
        set.endGroup();
    }

*/

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    WriteSettings();
    ReadSettings();


    return a.exec();
}

/*int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ReadWriteConfigFile w;
    w.show();

    return a.exec();
}*/
