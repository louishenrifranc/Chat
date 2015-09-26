#ifndef SERVEUR_H
#define SERVEUR_H
#include <QTcpServer>

#include "clientserveur.h"

class Serveur: public QTcpServer
{
    Q_OBJECT
public:
    Serveur(QObject *parent = 0);

signals:
    void nouveauClient(ClientServeur *);

    void sslErreur(const QString &);
public slots:
private:
protected:
    void incomingConnection(int descriptionSocket);

};

#endif // SERVEUR_H
