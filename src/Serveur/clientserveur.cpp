#include "clientserveur.h"
/**
 * @brief ClientServeur::ClientServeur
 * @param descriptionSocket
 * @param parent
    Est appele par le signal de la classe Serveur pour creer une connection securisée entre le serveur et le socket

*/






ClientServeur::ClientServeur(int descriptionSocket, QObject *parent) :
    QSslSocket(parent)
{
    connect(this,SIGNAL(disconnected()),SLOT(deleteLater()));
    connect(this,SIGNAL(ssLError(QSring)),SLOT(sslErreur(QList<QSslError>)));

    if(!setSocketDescriptor(descriptionSocket)){                                // Ouvre un socket pour la nouve
        qDebug() << "N'a pas pu creer un nouveau socket";                                                                        // connection
        return ;
    }

    QFile file(QCoreApplication::applicationDirPath()+"\\Cle\\server-key.pem");     // Ouvre la clé du serveur

    if( ! file.open(QIODevice::ReadOnly)){
        qDebug() << "Ouverture de la clé du serveur impossible "+file.error();
        return;
    }
    else{
        qDebug() << "La clé du serveur a pu être ouverte";
    }
    QSslKey key(&file, QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey,"");                  // La transforme en objet clé
    file.close();
    setPrivateKey(key);                                                             // Definit la privateKey

    if(!addCaCertificates(QCoreApplication::applicationDirPath()+"\\Cle\\ca.pem")){
        qDebug() <<"Ouverture du certificat de la ca pour le serveur impossible";
        return ;
    }
    else{
        qDebug() << "Ouverture du certificat de la ca pour le serveur reussi";
    }

    setLocalCertificate(QCoreApplication::applicationDirPath()+"\\Cle\\server-crt.pem");

    setPeerVerifyMode(QSslSocket::VerifyNone);
    ignoreSslErrors();
    startServerEncryption();

}
void ClientServeur::sslErreur(const QList<QSslError> &err){
    foreach(const QSslError &error,err){
         emit sslError(QString::number(error.error()));
    }
}
