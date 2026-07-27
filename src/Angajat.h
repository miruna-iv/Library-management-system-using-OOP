#ifndef ANGAJAT_H
#define ANGAJAT_H

#include "Persoana.h"
#include <iostream>
#include <string>

enum Departament {
    MANAGEMENT,
    VANZARI,
    ACHIZITII,
    CONTABILITATE,
    IT,
    RESURSE_UMANE
};

std::string departamentToString(Departament d);

//Angajat mpsteneste virtual persoana pentru a rezolva ambiguitatea in cazul mostenirii in diamant
class Angajat : public virtual Persoana {
    //parametri private by default, accesibili doar din interiorul clasei, deoarece nicio clasa derivata nu are nevoie sa acceseze direct campurile
    const int idAngajat;
    float salariu;
    int anAngajare;
    Departament departament;
    static int nrTotalAngajati;

public:
    // constructori
    Angajat();
    Angajat(const Angajat& a);
    Angajat(const std::string& n, int varsta, char g,
            float salariu, int anAngajare, Departament dep);

    // destructor
    ~Angajat() override;

    // operator =
    Angajat& operator=(const Angajat& a);

    // implementarea metodelor pure virtuale din Persoana
    void afisare() const override;
    bool validare() const override;
    Persoana* clone() const override;
    std::string toString() const override;
    
    // operatori I/O
    friend std::ostream& operator<<(std::ostream& out, const Angajat& a);
    friend std::istream& operator>>(std::istream& in, Angajat& a);

    // operatori relationali (dupa salariu)
    bool operator==(const Angajat& a) const;
    bool operator<(const Angajat& a)  const;
    bool operator<=(const Angajat& a) const;
    bool operator>(const Angajat& a)  const;
    bool operator>=(const Angajat& a) const;

    // operator ++ / -- prefixat: marire / reducere salariu cu 100
    Angajat& operator++();
    Angajat& operator--();

    // getteri si setteri
    float getSalariu() const;
    int getAnAngajare() const;
    Departament getDepartament() const;
    int getIdAngajat() const;

    static int getNrTotalAngajati();
    void setSalariu(float s);
    void setAnAngajare(int an);
    void setDepartament(Departament d);
};

#endif