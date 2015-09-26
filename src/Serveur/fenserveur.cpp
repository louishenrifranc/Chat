#include "fenserveur.h"

FenServeur::FenServeur() : QWidget()
{
    QVBoxLayout *layout=new QVBoxLayout;
    etatServeur=new QLabel;

    quitter=new QPushButton                                                 // Cree le bouton quitter
            ("Fermer le serveur");
    connect(quitter,
            SIGNAL(clicked(bool)),qApp,SLOT(quit()));

    layout->addWidget(etatServeur);
    layout->addWidget(quitter);
    setLayout(layout);

    serveur=new /**Serveur**/QTcpServer(this);      // Cree un serveur QTCp
                                                    // Qui utilise la fonction incomingConnection
                                                    // Pour creer un nouveau ClientServeur qui gere les certificats
                                                    // et demarre l'encryption entre le client et le socket



    if(!serveur->listen(QHostAddress::Any,port)){

        etatServeur->setText("Le serveur n'a pas pu être lancer");
        return ;
    }
    else {
        etatServeur->setText("Le serveur a pu être lancer");


        connect(serveur,SIGNAL(newConnection())/**nouveauClient(ClientServeur*)**/,
                this,SLOT(nouvelleConnexion()));
      //  connect(serveur,SIGNAL(sslErreur(QString)),this,SLOT(sslErreur(QString)));



    }
}

/**
void FenServeur::sslErreur(const QString &erreur){
    QMessageBox::warning(this,"Erreur",erreur);
}
**/
void FenServeur::nouvelleConnexion(){

    qDebug() << "Nouvelle Connexion";

    QTcpSocket *nouvelle_connexion=
            serveur->nextPendingConnection();                           // La derniere connexion en attente

    clients.insert(nouvelle_connexion);

    qDebug() << "Un nouveau client a été rajouté a la liste";
    // Ajoute le socket a la Qliste
    connect(nouvelle_connexion,SIGNAL(readyRead())
            ,this,SLOT(donneesRecues()));                               // Signal que le client a envoyer des donnees
                                                                        // Est appellé a chaque nouveau packet reçu

    connect(nouvelle_connexion,SIGNAL(disconnected()),              // Signal de deconnection d'un socket
            this,SLOT(deconnexionClient()));
}

void FenServeur::donneesRecues(){
    QTcpSocket/**QSslSocket**/ *socket=qobject_cast</**QSslSocket**/ QTcpSocket *>(sender());
    if(socket == 0 ) return;
    QString line,user;
   socket->canReadLine();

       line=QString::fromUtf8(socket->readLine().trimmed());
       qDebug() <<line;
       QRegExp meRegex("^/me:(.*)$");
       QRegExp meRegex2("/x0924:(.*)$");
                                                        // Si le message est un message provient
                                                        // d'un nouvel utilisateur
       if(meRegex.indexIn(line) != -1){
                   user=meRegex.cap(1);
                   qDebug()<< "Message de nouveau utilisateur: "+user;
                   users[socket]= user;
                   foreach (QTcpSocket *client,clients) {
                       client->write(QString("Server: "
                                 +user+" has joined"+"\n")
                                     .toUtf8());
                   }
                   sendUserList();
    }
                                                         // Si l'utilisateur est déja connu

       else if (users.contains(socket)){
            QString message;
                                                       // Si c'est un message instantannée
           if(meRegex2.indexIn(line) != -1){
               message=meRegex2.cap(1);
               user=users[socket];
               qDebug() << "Message partiel de "+user+" : "+message;
               foreach (QTcpSocket *client,clients) {
                   client->write(QString("/x0924:"+user+":"+message).toUtf8());
               }
           }
           else {                                        // Si c'est un message complet
               message=line;
               user=users[socket];
               foreach (QTcpSocket *client,clients) {
                   qDebug() << "Message complet de "+user+" : "+message;
                   client->write(QString(user+":"+message).toUtf8());
               }
           }

       }
       else{
           qDebug() <<"Got bad message from Client";

    }


}


void FenServeur::sendUserList(){

    QStringList userList;
    foreach(QString user,users.values())
        userList << user;

    foreach(QTcpSocket *client,clients)
        client->write(QString("/users:"+userList.join(",")+"\n").toUtf8());
}

void FenServeur::deconnexionClient()
{
    QTcpSocket/**QSslSocket**/ *client = qobject_cast</**QSslSocket**/QTcpSocket*>(sender());
    qDebug() << "Client disconnected:" << client->peerAddress().toString();

    clients.remove(client);             // Supprime le client de la liste des QTCpSocket

    QString user = users[client];       // et de la correspondance qtcpsocket - qstring
    users.remove(client);

    sendUserList();                     // renvoit la nouvelle liste

    foreach(QTcpSocket *client, clients)
        client->write(QString("Server:" + user + " has left.\n").toUtf8());
}
