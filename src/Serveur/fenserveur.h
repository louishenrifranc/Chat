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
#include"client.h"

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

    QSet<QTcpSocket *> clients;
    QMap</**QSslSocket ***/QTcpSocket *,QString> users;
    QLabel *etatServeur;
    QPushButton *quitter;
    /**QSslSocket **/QTcpServer *serveur;
    quint16 port=5983;

};

#endif // FENSERVEUR_H
