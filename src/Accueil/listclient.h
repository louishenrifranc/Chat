#ifndef LISTCLIENT_H
#define LISTCLIENT_H
#include<QtWidgets>
/**
 * Classe affichant la liste des utilisateurs que l'on veut choisir pour discuter avec
 */
class ListClient : public QWidget
{
    Q_OBJECT
public:
    ListClient(const QStringList &);
    QStringList get_elements_Selectionnes();
signals:
    void nouvelleListeClient(const QStringList &);

private slots:
    void clickSelection();
private:
    QListView *vue;
    QStringListModel *modele;
    QPushButton *bouton;
    QStringList elementsSelectionnes;
};

#endif // LISTCLIENT_H
