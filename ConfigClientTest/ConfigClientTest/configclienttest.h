#ifndef CONFIGCLIENTTEST_H
#define CONFIGCLIENTTEST_H

#include <QObject>

class ConfigClientTest : public QObject
{
    Q_OBJECT

public:
    explicit ConfigClientTest(QObject *parent = 0);
    ~ConfigClientTest();
private slots:
  void testAdd();
  void testAdd_data();

  void testValid();
  void testValid_data();
};

#endif // CONFIGCLIENTTEST_H
