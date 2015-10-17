#ifndef CONNEXION_H
#define CONNEXION_H

#include <QWidget>
#include<QMessageBox>
#include<ctype.h>
#include<QDebug>
#include<QString>
#include<QtSql/QSqlDatabase>
#include<QtSql/QSqlQuery>
#include <QApplication>
#include<QCoreApplication>
#include<QApplication>
#include "src/Accueil/accueil.h"
namespace Ui {
class Connexion;
}

class Connexion : public QWidget
{
    Q_OBJECT

public:
    explicit Connexion(QWidget *parent = 0);
    ~Connexion();
public slots:
    void afficherEmail();
    void connexion();
    void email_Check();                                         // Verification instantannée
private:
    Ui::Connexion *ui;
    bool verifierPseudo(const QString &);                       // Verifie le pseudo
    bool verifierEmail(const QString &);                        // ...
    bool verifierPassword(const QString &);
    bool createConnexionSQL();                                  //
    bool connect_Check();                                       //
    QString rouge,blanc,vert,rouge_fonce;


};

#endif // CONNEXION_H
