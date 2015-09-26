#ifndef CLIENT_H
#define CLIENT_H
#include<QString>
#include<QApplication>

class Client
{
public:
    Client();
    Client(QString *email, QString *pseudo,QString *phrase,quint16 *age);

signals:

public slots:

private:
    QString *email;
    QString *pseudo;
    QString *phrase_comment;
    quint16 *age;
    };

#endif // CLIENT_H
