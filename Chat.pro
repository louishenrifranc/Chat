#-------------------------------------------------
#
# Project created by QtCreator 2015-09-20T18:54:35
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Chat
TEMPLATE = app




SOURCES +=\
    src/Accueil/accueil.cpp \
    src/Connexion/connexion.cpp \
    src/Main/main.cpp \
    src/Client/client.cpp \
    src/Serveur/clientserveur.cpp \
    src/Serveur/fenserveur.cpp \
    src/Serveur/serveur.cpp

HEADERS  += \
    src/Accueil/accueil.h \
    src/Connexion/connexion.h \
    src/Client/client.h \
    src/Serveur/clientserveur.h \
    src/Serveur/fenserveur.h \
    src/Serveur/serveur.h

FORMS    += \
    src/Connexion/connexion.ui \
    src/Accueil/accueil.ui

RESOURCES += \
    ressource.qrc

DISTFILES +=
