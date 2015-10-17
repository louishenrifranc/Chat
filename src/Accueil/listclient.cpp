#include "listclient.h"



ListClient::ListClient(const QStringList & list)
    : QWidget()
{
    QVBoxLayout *layout =new QVBoxLayout;
    QStringList liste(list);
    modele=new QStringListModel(liste);

    vue=new QListView;
    vue->setModel(modele);
    vue->setSelectionMode(QAbstractItemView::ExtendedSelection);

    bouton = new QPushButton("Discussion privée");
    layout->addWidget(vue);
    layout->addWidget(bouton);

    setWindowTitle("Contact pour discussion privée");
    setLayout(layout);
    connect(bouton,SIGNAL(clicked(bool)),this,SLOT(clickSelection()));
    this->show();
}
void ListClient::clickSelection(){
    QItemSelectionModel *selection=vue->selectionModel();
    QModelIndexList listeSelections = selection->selectedIndexes();

    for(int i(0);i<listeSelections.size();i++){
        QVariant elementSelectionne = modele->data(listeSelections[i],Qt::DisplayRole);
        elementsSelectionnes << elementSelectionne.toString();
    }
    if( ! elementsSelectionnes.isEmpty()) emit nouvelleListeClient(elementsSelectionnes);   // Si la liste n'est pas vide, emet un
                                                                                            // signal et envoit la liste des personnes
                                                                                            // a qui ont veut discuter de maniere privée

    this->close();
}


