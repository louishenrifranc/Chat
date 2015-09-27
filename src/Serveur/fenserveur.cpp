#include "fenserveur.h"

FenServeur::FenServeur() : QWidget()
{

    code_newusers="/me";
    code_messageinstantannee="/x0924";
    code_envoi_liste_clients="/users";
    code_discussion_public="/listepublic";
    code_discussion_prive="/listeprivee";
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
       QRegExp meRegex("^"+code_newusers+":(.*)$");
       QRegExp meRegex2("^"+code_messageinstantannee+":(.*)$"); // Si le message est un message provient
                                                                // d'un nouvel utilisateur
       QRegExp meRegex3("^"+code_discussion_prive+":(.*)$");
       QRegExp meRegex4("^"+code_discussion_public+"$");

       if(meRegex.indexIn(line) != -1){
                   user=meRegex.cap(1);
                   qDebug()<< "Message de nouveau utilisateur: "+user;
                   users[socket]= user;
                   listeUsers.insert(user);
                   foreach(QString clients,listeUsers){                         /*************************** ****/
                       listeConexion[user].insert(clients);
                       if(clients != user) listeConexion[clients].insert(user);
                   }
                   sendUserList();
    }
                                                         // Si l'utilisateur est déja connu

       else if (users.contains(socket)){
            QString message;
            user=users[socket];

            if(meRegex3.indexIn(line) != -1){
               listeConexion[user].clear();
               QStringList newliste= meRegex3.cap(1).split(",");    /************************* **************/
               foreach(QString privateUsers,newliste){
                   listeConexion[user].insert(privateUsers);
                   listeConexion[user].insert(user);
               }
            }
            else if(meRegex4.indexIn(line) != -1){
                listeConexion[user].clear();
                foreach(QString clients,listeUsers){                         /*************************** ****/
                    listeConexion[user].insert(clients);
                }
            }

            // Si c'est un message instantannée
           else if(meRegex2.indexIn(line) != -1){
               message=meRegex2.cap(1);
               qDebug() << "Message partiel de "+user+" : "+message;
               foreach(QString receiver,listeConexion[user]){
                   users.key(receiver)->write(QString(code_messageinstantannee+":"+user+":"+message).toUtf8());
               }
           }
           else {                                        // Si c'est un message complet
               message=line;
               qDebug() <<"Message complet de "+user;
               foreach(QString receiver,listeConexion[user]){
                   users.key(receiver)->write(QString(user+":"+message).toUtf8());
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
        client->write(QString(code_envoi_liste_clients+":"+userList.join(",")+"\n").toUtf8());
}

void FenServeur::deconnexionClient()
{
    QTcpSocket/**QSslSocket**/ *client = qobject_cast</**QSslSocket**/QTcpSocket*>(sender());
    qDebug() << "Client disconnected:" << client->peerAddress().toString();

    clients.remove(client);             // Supprime le client de la liste des QTCpSocket

    QString user = users[client];       // et de la correspondance qtcpsocket - qstring
    if(users.count(client)) users.remove(client);
    if(listeUsers.contains(user)) listeUsers.remove(user);
    if(listeConexion.contains(user)) listeConexion.remove(user);

    sendUserList();                     // renvoit la nouvelle liste

    foreach(QTcpSocket *client, clients)
        client->write(QString("Server:" + user + " has left.\n").toUtf8());
}
