#ifndef DATATEST_H
#define DATATEST_H

#include <QObject>

class DataTest : public QObject
{
    Q_OBJECT
public:
    explicit DataTest(QObject *parent = 0);

private slots:
  void testAdd();
  void testAdd_data();

  void testValid();
  void testValid_data();

};

#endif // DATATEST_H
