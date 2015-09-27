#include "connexion.h"
#include "ui_connexion.h"
#include<QByteArray>
#include<QCryptographicHash>






Connexion::Connexion(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Connexion)
{
// Les differents CSS
    rouge_fonce="background-color: rgb(255, 255, 255)\
            ;selection-background-color: rgb(255, 255, 255);\
            color: rgb(255, 255, 255);\
            font: 75 italic 9pt \"Raavi\";\
            font: 8pt \"Comic Sans MS\";\
            border-style: outset;\
            border-width:3px;\
            border-radius: 8px;\
            border-color:rgb(154, 6, 15);";

    rouge="background-color: rgb(255, 255, 255)\
            ;selection-background-color: rgb(255, 255, 255);\
            color: rgb(255, 255, 255);\
            font: 75 italic 9pt \"Raavi\";\
            font: 8pt \"Comic Sans MS\";\
            border-style: outset;\
            border-width:3px;\
            border-radius: 8px;\
            border-color:rgb(255, 16, 60);";
    blanc="background-color: rgb(255, 255, 255)\
            ;selection-background-color: rgb(255, 255, 255);\
            color: rgb(255, 255, 255);\
            font: 75 italic 9pt \"Raavi\";\
            font: 8pt \"Comic Sans MS\";\
            border-style: outset;\
            border-width:3px;\
            border-radius: 8px;\
            border-color:white;";
    vert="background-color: rgb(255, 255, 255)\
            ;selection-background-color: rgb(255, 255, 255);\
            color: rgb(255, 255, 255);\
            font: 75 italic 9pt \"Raavi\";\
            font: 8pt \"Comic Sans MS\";\
            border-style: outset;\
            border-width:3px;\
            border-radius: 8px;\
            border-color:green;";

    ui->setupUi(this);

    ui->email->setHidden(true);                                 // Cache de base la zone d'email


    connect(ui->nouveau,SIGNAL(clicked(bool)),
            this,SLOT(afficherEmail()));
    connect(ui->connexion,SIGNAL(clicked(bool))
            ,this,SLOT(connexion()));
    connect(ui->password,SIGNAL(returnPressed()),this,SLOT(connexion()));
    connect(ui->pseudo,SIGNAL(returnPressed()),this,SLOT(connexion()));
    connect(ui->email,SIGNAL(textChanged(QString)),this,SLOT(email_Check()));
}

Connexion::~Connexion()
{
    delete ui;
}




void Connexion::afficherEmail(){                                    // Gere l'affichage de l'email

    if(ui->email->isHidden()) ui->email->setHidden(false);
    else{
        ui->email->setHidden(true);
        ui->email->clear();
    }
}


bool Connexion::verifierPseudo(const QString &pseudo){

    for(int i(0);i<pseudo.size();i++){
        // Ajouter une verification de pseudo ici

    }
    return true;
}

bool Connexion::verifierPassword(const QString & password){
    for(int i(0);i<password.size();i++){
        // Ajouter une verification de password ici
    }
    return true;
}


bool Connexion::verifierEmail(const QString &email){
    char c;
    // N'importe quel lettre-chiffre + 1@ + N'importe quel lettrechifre + 1. + ...
    int u,v,sum_1,sum_2;
    sum_1=sum_2=0;
    for(int i(0);i<email.size();i++){
        c=email[i].toLatin1();
        if (c >='A' && c <= 'Z' || c >= 'a' && c <= 'z'|| c >= '0' && c <='9' ||c =='-' || c =='+'|| c =='_'){

        }
        else if (c == '.'){
            if(sum_1 == 0) v = i;
            sum_1++;
       }
        else if ( c == '@'){
            if(sum_2 == 0) u =i;
            sum_2++;
        }
        else return false;
         }
    if(sum_1 == 1 && sum_2 == 1 && u<v) return true;
   return false;
}


void Connexion::connexion(){
    connect_Check();
}

bool Connexion::connect_Check(){

    if( !ui->email->isHidden() )
        if(!verifierEmail(ui->email->text()))
            return false;

 /* if(!verifierPassword(ui->password->text())){
        return false;
    }
    if(!verifierPseudo(ui->pseudo->text())){
    return false;
    }
    */


    QString pseudo=ui->pseudo->text();
    QString password=ui->password->text();
    QString email=ui->email->text();

    QByteArray bites(password.toUtf8());            // Tableau de bytes

    QByteArray hash_sda256= QCryptographicHash
            ::hash(bites,QCryptographicHash::Sha256);
                                                    // Creer un hash pour ce tableau

    if(!createConnexionSQL()){

        qDebug() <<"Impossible d'ouvrir la database";
        return false;
    }

    if(!ui->nouveau->isChecked()){           // Si on se connecte en tant que ancien compte

        QSqlQuery query;
        query.prepare("SELECT "                         // Requete MYSQL de recherche
                      "id FROM client "
                      "WHERE username='"
                      +pseudo+"' AND password=:pass");

        query.bindValue(":pass",hash_sda256.toHex());     // Ajoute une valeur pour pass

        int result=0;
        if(query.exec()){                                 // Si la requete renvoit une unique reponse
            while(query.next()){
            result++;
            }
        }
        if(result == 1){
            ui->pseudo->setStyleSheet(vert);
            ui->password->setStyleSheet(vert);
            Accueil *accueil=new Accueil(
                        ui->pseudo->text(),ui->password->text());
            accueil->show();
            this->close();
        }
        else{
            ui->pseudo->setStyleSheet(rouge);
            ui->password->setStyleSheet(rouge);

        }
    }
    else if(ui->nouveau->isChecked()){      // Si on se connecte en tant que nouveau compte


        QSqlQuery query("SELECT id FROM"
                        " client WHERE "
                        "username='"+pseudo
                        +"' OR email='"+email+"'");
        int result=0;
        if(query.exec()){
            while(query.next()){
            result++;
            }


        }
        if(result == 1){                                // Si un compte existe deja avec le meme email ou pseudo
            QMessageBox::warning(
                        this,"Erreur de connexion",
                        "Impossible de se connecter,"
                        " le profil existe déja");
        }
        else{
            query.clear();
            query.prepare("INSERT INTO client VALUES(NULL,'"+pseudo+"','"+email+"',:pass)");
            query.bindValue(":pass",hash_sda256.toHex());
            qDebug() << hash_sda256.toHex();
            if(query.exec()){
                QMessageBox::information(this,"Login","Bravo vous avez pu créer un nouveau compte");
                ui->pseudo->setStyleSheet(vert);
                ui->password->setStyleSheet(vert);
                Accueil *accueil=new Accueil(ui->pseudo->text(),ui->password->text());
                accueil->show();
                this->close();

            }
            else{
                QMessageBox::information(this,"Login","Impossible de creer un compte");

            }

        query.clear();
        }


    }

}


bool Connexion::createConnexionSQL(){
        QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName("localhost");
        db.setDatabaseName("chat");
        db.setUserName("root");
        db.setPassword("aX1pUtVor19");

        if (db.open()) {
            qDebug() << "La base de donnees a reussi a s'ouvrir";
            return true;
        }
        return false;
}

void Connexion::email_Check(){
    if(verifierEmail(ui->email->text())){
        ui->email->setStyleSheet(vert);

    }
    else{
        ui->email->setStyleSheet(rouge_fonce);

    }
}


