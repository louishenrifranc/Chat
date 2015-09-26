#ifndef CLIENTSERVEUR_H
#define CLIENTSERVEUR_H
#include<QSslSocket>
#include<QtNetwork>
#include<QDebug>
#include<QtWidgets>
#include<QWidget>
#include<QFileDevice>

class ClientServeur : public QSslSocket
{
    Q_OBJECT
public:
    ClientServeur(int descriptionSocket,QObject *parent);

signals:
    void sslError(const QString &erreur);
public slots:
    void sslErreur(const QList<QSslError> &err);
};

#endif // CLIENTSERVEUR_H
