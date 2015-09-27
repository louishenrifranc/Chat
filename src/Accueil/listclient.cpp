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
    if( ! elementsSelectionnes.isEmpty()) emit nouvelleListeClient(elementsSelectionnes);
    this->close();
}

QStringList ListClient::get_elements_Selectionnes(){
    return elementsSelectionnes;
}
