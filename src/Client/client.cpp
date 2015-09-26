#include "client.h"

Client::Client()
{
    email=new QString;
    pseudo=new QString;
    phrase_comment=new QString;
    age=new quint16;
}


Client::Client(QString *email, QString *pseudo,QString *phrase,quint16 *age){
    this->email=email;
    this->pseudo=pseudo;
    this->phrase_comment=phrase;
    this->age=age;
}
