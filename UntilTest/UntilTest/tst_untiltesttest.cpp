#include <QString>
#include <QtTest>

class UntilTestTest : public QObject
{
    Q_OBJECT

public:
    UntilTestTest();

private Q_SLOTS:
    //每个private slot函数都是会被自动调用的测试函数
    void testCase1();
    //以“_data”结尾的函数向对应的测试函数提供测试数据
    void testCase1_data();
};

UntilTestTest::UntilTestTest()
{
}

void UntilTestTest::testCase1()
{
    QFETCH(QString, data);
    QVERIFY2(true, "Failure");
}

void UntilTestTest::testCase1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}

QTEST_APPLESS_MAIN(UntilTestTest)

#include "tst_untiltesttest.moc"
