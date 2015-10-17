#ifndef FENSERVEUR_H
#define FENSERVEUR_H
#include <QWidget>
#include<QtWidgets>
#include<QtNetwork>

#include <QSslSocket>
#include <QString>
#include<QTcpSocket>
#include<QDebug>
#include"serveur.h"
#include"clientserveur.h"
#include"src/Client/client.h"

class FenServeur: public QWidget
{
    Q_OBJECT
public:
    FenServeur();
private:
    void readyRead();

signals:

public slots:
    void nouvelleConnexion();                                   // Nouvelle connecion
    void donneesRecues();                                       // Re-envoit les donnes recus a tous les sockets actifs
    void deconnexionClient();                                   // Deconnection un client
//  void sslErreur(const QString &erreur);
    void sendUserList();
private:

    QSet<QTcpSocket *> clients;                                 // Liste des socket, JAMAIS VIDE (sauf deconnexion)
    QSet<QString > listeUsers;                                  // Liste des pseudo, JAMAIS VIDE (sauf deconnexion)
    QMap<QString,QSet<QString > > listeConexion;                // Pour chaque utilisateur, une liste des utilisateurs avec
                                                                // qui il commmunique

    QMap</**QSslSocket ***/QTcpSocket *,QString> users;         // Fait l'equivalent entre Socket et pseudo
    QLabel *etatServeur;                                        //
    QPushButton *quitter;
    /**QSslSocket **/QTcpServer *serveur;
    quint16 port=5983;
    QString code_newusers,code_messageinstantannee,              // Different code de comprehension des messages
                            code_envoi_liste_clients,
                            code_discussion_public,
                            code_discussion_prive;

};

#endif // FENSERVEUR_H
