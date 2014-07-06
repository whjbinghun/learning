#ifndef NEWSPAPER_H
#define NEWSPAPER_H
#include <QObject>

class Newspaper : public QObject{
    Q_OBJECT
public:
    Newspaper(const QString & name) : ms_name(name) {

    }
    void send(){
        emit newPaper(ms_name);
    }

signals:
    void newPaper(const QString &name);
private:
    QString ms_name;
};
#endif // NEWSPAPER_H
