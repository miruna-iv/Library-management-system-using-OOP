#ifndef ANGAJAT_CITITOR_H
#define ANGAJAT_CITITOR_H

#include "Angajat.h"
#include "Cititor.h"


class AngajatCititor : public Angajat, public Cititor {
    //AngajatCititor nu are campuri private la randul sau, deoarece mosteneste toate atributele necesare
    //de la clasa Cititor si Angajat
    // Accesul la campurile din Persoana (nume, varsta, gen) este posibil prin getteri mosteniti
public:
    // constructori
    AngajatCititor();
    AngajatCititor(const AngajatCititor& ac);
    AngajatCititor(std::string n, int v, char g, float salariu, int anAngajare, Departament dep, float buget, bool cardFidelitate, std::vector<int> idCarti);

    // destructor
    ~AngajatCititor() override;

    // metode virtuale
    void afisare() const override;
    bool validare() const override;
    Persoana* clone() const override;
    std::string toString() const override;

    // operator =
    AngajatCititor& operator=(const AngajatCititor& ac);

    // operatori I/O
    friend std::ostream& operator<<(std::ostream& out, const AngajatCititor& ac);
    friend std::istream& operator>>(std::istream& in, AngajatCititor& ac);

    float aplicaReducere(float pret);
};

#endif