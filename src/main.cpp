#include <iostream>
#include <sqlite3.h>
#include "Meniu.h"
#include "Database.h"
#include "AbstractFactory.h"
using namespace std;

int main(){
    Database db("librarie.db");

    Librarie librarie("Librarie noua", "Adresa necunoscuta");

    int idLibrarieAleasa = -1;
    if (db.esteConectat()) {
        idLibrarieAleasa = db.alegeLibrarie();
        if (idLibrarieAleasa != -1) {
            db.incarcaLibrarie(librarie, idLibrarieAleasa);
            cout << "Loaded library with " << librarie.getStoc().size()
                 << " books and " << librarie.getClienti().size() << " clients." << endl;
        } else {
            cout << "No library selected." << endl;
        }
    } else {
        cout << "Database not connected." << endl;
    }

    // Alegerea familiei la runtime — demonstreaza Abstract Factory
    // Schimbarea familiei se face printr-o singura modificare: inlocuirea fabricii
    cout << "\n====== Selectati tipul de abonament ======\n";
    cout << "1. Basic (pret intreg, fara card fidelitate)\n";
    cout << "2. VIP   (reducere 20% la carti, card fidelitate inclus)\n";
    cout << "Optiune: ";
    int optFactory; cin >> optFactory;

    // Clientul (Meniu) primeste fabrica prin dependency injection
    // si lucreaza exclusiv cu interfata abstracta IAbonamentFactory
    IAbonamentFactory* factory = nullptr;
    if(optFactory == 2)
        factory = new AbonamentVIPFactory();
    else
        factory = new AbonamentBasicFactory();

    cout << "Abonament ales: " << factory->numeAbonament() << "\n";

    // Meniu primeste fabrica prin constructor (dependency injection)
    Meniu meniu(librarie, factory);
    meniu.porneste();

    if (db.esteConectat() && idLibrarieAleasa != -1) {
        db.salveazaLibrarie(librarie, idLibrarieAleasa);
    }

    delete factory;
    return 0;
}