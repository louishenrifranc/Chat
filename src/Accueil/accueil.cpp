#include "accueil.h"
#include "ui_accueil.h"
#include<QSslKey>

Accueil::Accueil(const QString &pseudo,const QString &pass,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Accueil)
{                                                       // Style pour les ZAI


    pile_Text_CSS="border-style: solid;\
            border-width:1px; \
            border-color:black;\
            background-color: green;\
           font: italic 11pt \"MV Boli\"";

    ui->setupUi(this);

    socket=new /**QSslSocket**/ QTcpSocket(this);
    port=5983;
    ip="127.0.0.1";
    this->pseudo=pseudo;
    this->password=pass;
    ui->chat->setReadOnly(true);
    ui->saisie->setFocus();                             // Met le focus sur la saisie
    setLayout(ui->horizontalLayout_2);
    stacklayout=new QStackedLayout;

    ui->verticalLayout->addLayout(stacklayout);                          // Ajoute au layout vertical

    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    /**
    QFile file(QCoreApplication::applicationDirPath()+"\\Cle\\client-key.txt");

    if(! file.open(QFileDevice::ReadOnly )){
        qDebug() <<" La clé du client n'a pa pu être chargé";
        return;
         }
    else{
        qDebug() << "La clé du client a pu etre chargé";
    }

    QSslKey key(&file, QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey,"");
    file.close();
    socket->setPrivateKey(key);

    socket->setLocalCertificate( QQCoreApplication::applicationDirPath()+"\\Cle\\client-crt.txt");

    if( ! socket->addCaCertificates(QCoreApplication::applicationDirPath()+"\\Cle\\ca.txt")){
        qDebug() << "Ouverture du certifiat de la ca du client impossible";
    }
    else{
        qDebug() <<"Ouverture du certificat de la ca du client impossible";

    }

    socket->setPeerVerifyMode(QSslSocket::VerifyNone);
    socket->ignoreSslErrors();
    socket->abort();
    socket->connectToHostEncrypted(ip,port);
    socket->startClientEncryption();
    **/


    socket->abort();                                            // Coupe toute les connections prece
    socket->connectToHost(ip,port);                             // Se connecte
    if(socket == 0) return;                                     // Si impossible Arrete

    connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(deconnecter()));
    connect(socket,SIGNAL(connected()),this,SLOT(connecte()));

    connect(ui->saisie,SIGNAL(textEdited(QString)),this,SLOT(     // Des que le texte est modifié
                envoyer_Instantanne(QString)));
    connect(ui->saisie,SIGNAL(returnPressed()),this,              // Raccourci si l'on appuie sur Enter
            SLOT(envoyer_Message()));
    connect(ui->send,SIGNAL(clicked(bool)),
            this,SLOT(envoyer_Message()));

    connect(ui->listWidget,SIGNAL(currentRowChanged(int)),
            stacklayout,SLOT(setCurrentIndex(int)));
                                                                // Si l'on clique sur le nom
                                                                // d'un utilisateur,affiche ce qu'il tape

    connect(ui->listWidget,SIGNAL(customContextMenuRequested(QPoint)),SLOT(customMenuRequested(QPoint)));


    connect(ui->actionEnregistrer_la_discussion,SIGNAL(triggered(bool)),    // Possibilité d'enregistrer la discussion
            this,SLOT(enregistrer()));
    connect(ui->actionChanger_couleur,SIGNAL(triggered(bool)),              // Possibilite de changer son pseudo
            SLOT(changerCouleurPseudo()));
}

/**
  Fonction qui s'occupe de recevoir des donnes et de les afficher
**/

void Accueil::readyRead(){

        QString line=QString::fromUtf8(             //Des que recoit de nouvelles donnes, les
                    socket->readLine()).trimmed();  //met dans line

        QRegExp messageRegex("^([^:]+):(.*)$");     // Motif pour les messages finaux
        QRegExp usersRegex("^/users:(.*)$");        // Motif pour les messages nouveau users
        QRegExp message_InstantaneRegex(            // Motif pour les messages intermediaires
                    "^/x0924:([^:]+):(.*)$");


        if(usersRegex.indexIn(line) != -1){         // Si c'est une requete nouveau User
            ui->listWidget->clear();                // Vide la liste d'utilisateur

            QStringList liste_user =                // Recupere ce qu'il y a entre parenthes du split
                    usersRegex.cap                  // cad une liste de user sépares par des parentheses
                    (1).split(",");

            utilisateursToInt.clear();              // Supprime les equivalences users-index
            qDebug() <<"Nouvelle requete utilisateur";
            int i(0);
            users.clear();

            foreach(QString nouvelle_user,liste_user){
                new QListWidgetItem(                                // Cree un item dans la liste au nom de
                            QPixmap(":/Images/user.png.png"),       // l'user
                            nouvelle_user, ui->listWidget);
                utilisateursToInt.insert(nouvelle_user,i++);        // Equivalence user - index
                users << nouvelle_user;
                QTextEdit *buffer_TextEdit=new QTextEdit;           // Cree un pointeur vers un QtextEdit
                buffer_TextEdit->setStyleSheet(pile_Text_CSS);      // Modifie son CSS
                buffer_TextEdit->setReadOnly(true);
                stacklayout->addWidget(buffer_TextEdit);            // Ajoute au stacklayout
            }

        }

        else if(message_InstantaneRegex.indexIn(line) != -1){       // Si c'est une requete messageI
            qDebug() <<"Nouvelle requete message instantanée";

            QString user=message_InstantaneRegex.cap(1);
            QString message=message_InstantaneRegex.cap(2);
            QMap<QString,int>::iterator index=
                    utilisateursToInt.find(user);                   // Cherche l'user dans la liste de correspondance
            if(index == utilisateursToInt.end()) return;            // Si il existe pas,quitte(pour eviter des problemes
            else{

                int indice=utilisateursToInt[user];                 // On est sur qu'il existe
                QTextEdit *textedit=qobject_cast<QTextEdit *>
                        (stacklayout->widget(indice));              // Recupere le QtextEdit de cet user
                QRegExp buffer_mregExp3("^(.*)/x0924(.*)$");        // Un probleme quand l'on tape trop vite, le motif x0924 s'affiche
                if(buffer_mregExp3.indexIn(message) == -1)          // Gere le probleme
                textedit->setText(message+" ...");                         // Modifie la zone de texte
            }
        }
        else if(messageRegex.indexIn(line) != -1){              // Si c'est une requete message complet
                                                                // Meme principe que au dessus, sauf vide la QTextEdit et ajoute a la fenetre de chat

            QString user=messageRegex.cap(1);
            QString message=messageRegex.cap(2);
            qDebug() <<"Nouvelle requete message"+user+" "+message;
            QMap<QString,int>::iterator index=utilisateursToInt.find(user);
            if(index == utilisateursToInt.end()){
                qDebug() << "utilisateurToInt vide";
                return;
            }
            else{
                int indice=utilisateursToInt[user];
                QTextEdit *textedit=qobject_cast<QTextEdit *>(stacklayout->widget(indice));
                textedit->setText("");

                ui->chat->append("<p><font color="+couleur_User+">"+user+"</font></p>"+" : "+message);
            }

        }
        else{
            qDebug() << "Message recu par la client impossible a lire";
        }

}

void Accueil::deconnecter(){
    ui->chat->append("Deconnecte!");
}


/**
 * @brief Accueil::envoyer_Message
 * Envoyer message complet
*/
void Accueil::envoyer_Message(){

    QString message=ui->saisie->text().trimmed();
    if(!message.isEmpty()){
        socket->write(QString(message+"\n").toUtf8());
    }
    ui->saisie->clear();
}

/**
 * @brief Accueil::envoyer_Instantanne
 * @param texte
 * Envoyer message Instantannée
*/
void Accueil::envoyer_Instantanne(const QString &texte){
    QString message=texte;
 // if(!message.isEmpty())

    socket->write(QString("/x0924:"+message+" ").toUtf8());
}

void Accueil::connecte(){
    socket->write(QString("/me:"+this->pseudo+"\n").toUtf8());
    ui->chat->append("Connecte");
}


void Accueil::on_send_clicked(){
    envoyer_Message();
}



void Accueil::enregistrer(){
    QString nomfichier=QFileDialog::getSaveFileName(this,"Sauvegarder la discussion",QString(),"html files (*.html)");
    if(nomfichier != ""){
        QFile file(nomfichier);
        if(file.open(QIODevice::ReadWrite)){
            QTextStream stream(&file);
            stream << ui->chat->toPlainText();
            file.flush();
            file.close();
        }
        else{
            QMessageBox::critical(this,"Erreur","Impossible d'enregistrer le texte");
        }
    }
}


void Accueil::changerCouleurPseudo(){
        QColor couleur = QColorDialog::getColor(Qt::white, this);
        couleur_User=couleur.name();
        qDebug() <<"La couleur a changé"+couleur_User;
}


void Accueil::customMenuRequested(const QPoint &pos){
    QPoint globalPos= ui->listWidget->mapToGlobal(pos);
    QMenu *menu=new QMenu(this);
    QAction *discussionPrivee=new QAction("Discussion Privee",this);
    QAction *discussionPublic=new QAction("Discussion Public",this);
    menu->addAction(discussionPublic);
    menu->addAction(discussionPrivee);
    QAction *selected_Item=menu->exec(globalPos);
    if(selected_Item == discussionPrivee ){
        ListClient *choixClient=new ListClient(users);
        connect(choixClient,SIGNAL(nouvelleListeClient(QStringList)),this,SLOT(nouvelleListePrivee(const QStringList &)));



    }
    else if(selected_Item == discussionPublic){
        socket->write(QString("/listepublic\n").toUtf8());
    }
}

void Accueil::nouvelleListePrivee(const QStringList & liste){
    socket->write(QString("/listeprivee:"+liste.join(",")+"\n").toUtf8());
}
