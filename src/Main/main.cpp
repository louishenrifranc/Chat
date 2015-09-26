#include "connexion.h"
#include"src/Serveur/fenserveur.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Connexion chat;
    chat.show();

    FenServeur serveur;
    serveur.show();
    return a.exec();
}
