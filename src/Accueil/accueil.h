#ifndef ACCUEIL_H
#define ACCUEIL_H
#include<QStackedLayout>
#include <QMainWindow>
#include<QSslSocket>
#include<QString>
#include<QMap>
#include<QTextEdit>
#include<QFile>
#include <QAction>
#include<QFileDialog>
#include<QMessageBox>
#include <QColor>
#include<QColorDialog>
#include<QPalette>

namespace Ui {
class Accueil;
}

class Accueil : public QMainWindow
{
    Q_OBJECT


public:
    Accueil(const QString &,const QString &,
            QWidget *parent = 0);

private slots:
    void connecte();                                // Appeller lors que le signal de QTCPServer
                                                // connected est appelé

    void on_send_clicked();                         // Si on appuie sur le bouton Send

    void readyRead();                               // Gere la reception des messages
    void deconnecter();                             // Deconnecter client
    void envoyer_Message();                         // Envoyer message
    void envoyer_Instantanne(const QString &);      // Envoyer message des que le texte est modifier
    void enregistrer();
    void changerCouleurPseudo();
    void customMenuRequested(const QPoint &);
private:
    Ui::Accueil *ui;
    /**QSslSocket**/ QTcpSocket *socket;            // Le socket
    QStringList users;                              // Le widget de liste des utilisateurs
    quint16 port;                                   // Le port de connection
    QString ip;                                     // L'addresse IP
    QMap<QString,int> utilisateursToInt;            // Permet de lier les zonesde texte à la liste users
                                                     // en enregistrant leur indice
    QStackedLayout *stacklayout;                    // Pour afficher l'ecriture en instantannée
    QString pseudo;                                 // Le pseudo du client
    QString password;                               // Le password du client
    QString pile_Text_CSS;                          // Le CSS des zone d'ecritures instantannées créés
    QString couleur_User="fuchsia";                 // Couleur de l'utilisateur

};

#endif // ACCUEIL_H
