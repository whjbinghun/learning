#include "configclienttest.h"
#include "ui_configclienttest.h"
#include <QtTest>
#include <QDate>


ConfigClientTest::ConfigClientTest(QObject *parent) {

}

ConfigClientTest::~ConfigClientTest()
{
}

void ConfigClientTest::testAdd()
{
  QDate date( 1979, 5, 16 );

  QFETCH( int, addYear );
  QFETCH( int, addMonth );
  QFETCH( int, addDay );

  QDate next = date.addYears( addYear ).addMonths( addMonth ).addDays( addDay );

  QTEST( next.year(), "year" );
  QTEST( next.month(), "month" );
  QTEST( next.day(), "day" );
}

void ConfigClientTest::testAdd_data ()
{
  QTest::addColumn<int>( "addYear" );
  QTest::addColumn<int>( "addMonth" );
  QTest::addColumn<int>( "addDay" );
  QTest::addColumn<int>( "year" );
  QTest::addColumn<int>( "month" );
  QTest::addColumn<int>( "day" );

  QTest::newRow( "Start date" )    << 0 << 0 << 0 << 1979 << 5 << 16;
  QTest::newRow( "One day" )       << 0 << 0 << 1 << 1979 << 5 << 17;
  QTest::newRow( "Twenty days" )   << 0 << 0 << 20 << 1979 << 6 << 5;
  QTest::newRow( "366 days" )      << 0 << 0 << 366 << 1980 << 5 << 16;
  QTest::newRow( "One month" )     << 0 << 1 << 0 << 1979 << 6 << 16;
  QTest::newRow( "Twelve months" ) << 0 << 12 << 0 << 1980 << 5 << 16;
  QTest::newRow( "28 years" )      << 28 << 0 << 0 << 2007 << 5 << 16;
}

void ConfigClientTest::testValid()
{
  QFETCH( int, year );//#define QFETCH(type, name)   type为类型或者类，name表示变量名或者对象
  QFETCH( int, month );
  QFETCH( int, day );

  QDate date( year, month, day );
  QTEST( date.isValid(), "valid" );
}

void ConfigClientTest::testValid_data()
{
  QTest::addColumn<int>( "year" );
  QTest::addColumn<int>( "month" );
  QTest::addColumn<int>( "day" );
  QTest::addColumn<bool>( "valid" );

  QTest::newRow( "Valid, normal" ) << 1973 << 8 << 16 << true;
  QTest::newRow( "Invalid, normal" ) << 1973 << 9 << 31 << false;
  QTest::newRow( "Valid, leap-year" ) << 1980 << 2 << 29 << true;
  QTest::newRow( "Invalid, leap-year" ) << 1981 << 2 << 29 << false;
}
