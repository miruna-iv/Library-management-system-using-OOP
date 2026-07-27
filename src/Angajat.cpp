#include "Angajat.h"
#include "Exceptii.h"
#include "ConfigurareLibrarie.h"
#include <iostream>
using namespace std;

int Angajat::nrTotalAngajati = 0;

std::string departamentToString(Departament d) {
    switch(d) {
        case MANAGEMENT: return "Management";
        case VANZARI: return "Vanzari";
        case ACHIZITII: return "Achizitii";
        case CONTABILITATE: return "Contabilitate";
        case IT: return "IT";
        case RESURSE_UMANE: return "Resurse Umane";
        default: return "Necunoscut";
    }
}

//constructori

Angajat::Angajat(): Persoana("Anonim", 0, 'N'), idAngajat(++nrTotalAngajati), salariu(0.0f), anAngajare(0), departament(VANZARI){
    fout<<"Angajat creat"<<endl;
}

Angajat::Angajat(const Angajat& a): Persoana(a), idAngajat(++nrTotalAngajati), salariu(a.salariu), anAngajare(a.anAngajare), departament(a.departament){
    fout << "Constructor copiere Angajat: " << (getNume().empty() ?  "Anonim" : getNume()) << endl;
}

Angajat::Angajat(const std::string& n, int v, char g, float sal, int anAng, Departament dep): Persoana(n, v, g), idAngajat(++nrTotalAngajati), salariu(sal), anAngajare(anAng), departament(dep){
    fout << "Constructor Angajat cu parametri: " << (getNume().empty() ?  "Anonim" : getNume()) << endl;
}

Angajat::~Angajat() {
    fout << "Destructor Angajat"<< endl;
}

//metode virtuale
void Angajat::afisare() const { 
    //aplarea metodei din baza pentru a extinde comportamentul
    //Persoana::afisare();
    cout << *this;}

bool Angajat::validare() const {
    return !getNume().empty() && salariu >= 0 && anAngajare > 1900;
}

Persoana* Angajat::clone() const {
    return new Angajat(*this);
}

std::string Angajat::toString() const {
    return "Angajat[id=" + std::to_string(getId()) + "| nume= " + getNume() + "| departament= " + departamentToString(getDepartament()) + "| salariu= " + std::to_string(salariu) + " RON]";
}

//operator =
Angajat& Angajat::operator=(const Angajat& a) {
    if (this == &a) return *this;
    Persoana::operator=(a);
    salariu = a.salariu;
    anAngajare = a.anAngajare;
    departament = a.departament;
    return *this;
}

//I/O
ostream& operator<<(ostream& out, const Angajat& a) {
    out << "Nume:        " << (a.nume.empty() ? "Anonim" : a.nume) << " (ID: " << a.idAngajat << ")\n";
    out << "Gen:         " << a.gen << "\n";
    out << "Varsta:      " << a.varsta << "\n";
    out << "Departament: " << departamentToString(a.departament) << "\n";
    out << "An angajare: " << a.anAngajare << "\n";
    out << "Salariu:     " << a.salariu << " RON\n";
    return out;
}

istream& operator>>(istream& in, Angajat& a) {
    cout << "--- Date angajat ---" << endl;
    cout << "Nume: ";
    char buffer[256];
    in.ignore();
    in.getline(buffer, 256);
    a.setNume(buffer); 

    cout << "Gen (M/F/N): ";
    char g; in >> g;
    a.setGen(g);

    cout << "Varsta: ";
    int v; in >> v;
    a.setVarsta(v);

    cout << "Salariu (RON): ";
    float s; in >> s;
    a.setSalariu(s);

    cout << "An angajare: ";
    in >> a.anAngajare;

    cout << "Departament (0:MANAGEMENT, 1:VANZARI, 2:ACHIZITII, 3:CONTABILITATE, 4:IT, 5:RESURSE_UMANE): ";
    int opt; in >> opt;
    a.departament = (opt >= 0 && opt <= 5) ? (Departament)opt : VANZARI;

    return in;
}
//operatori relationali; fac comparatia in functie de salariu
bool Angajat::operator==(const Angajat& a) const {
    if (salariu != a.salariu) return false;
    if (departament != a.departament) return false;
    if (nume!=a.nume) return false;
    return true;
}
bool Angajat::operator<(const Angajat& a)  const { return salariu <  a.salariu; }
bool Angajat::operator<=(const Angajat& a) const { return salariu <= a.salariu; }
bool Angajat::operator>(const Angajat& a)  const { return salariu >  a.salariu; }
bool Angajat::operator>=(const Angajat& a) const { return salariu >= a.salariu; }

//operatori ++, -- : adauga/scad 100 ron din salariu
Angajat& Angajat::operator++() {
    salariu += 100.0f;
    return *this;
}
Angajat& Angajat::operator--() {
    if (salariu >= 100.0f) salariu -= 100.0f;
    else cout << "Salariul nu poate fi redus sub 0." << endl;
    return *this;
}
//getteri si setteri
float Angajat::getSalariu() const { return salariu; }
int Angajat::getAnAngajare() const { return anAngajare; }
Departament Angajat::getDepartament() const { return departament; }
int Angajat::getIdAngajat() const { return idAngajat; }
int Angajat::getNrTotalAngajati() { return nrTotalAngajati; }
void Angajat::setSalariu(float s) {
    if (s < 0) throw ValidationError("salariu", s);
    salariu = s;
}
void Angajat::setAnAngajare(int an){ if (an <= 1900) throw ValidationError("anAngajare", (float)an);
    anAngajare = an; 
}
void Angajat::setDepartament(Departament d) { departament = d; }