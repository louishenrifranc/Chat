#include "serveur.h"

Serveur::Serveur( QObject *parent ) :QTcpServer( parent )
{

}

//on surcharge la methode incomingConnection
//le but ici est de récupérer un QSslSocket au lieu d'un QTcpSocket
void Serveur::incomingConnection(int descriptionSocket)
{
    ClientServeur *client    =new ClientServeur( descriptionSocket, this );
    connect(client, SIGNAL(sslError(const QString&)),this,  SIGNAL(sslErreur(const QString&))  );
    qDebug() << "Appel de la méthode incoming connection";
    emit nouveauClient(client);
}


