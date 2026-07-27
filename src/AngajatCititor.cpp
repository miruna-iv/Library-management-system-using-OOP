#include "AngajatCititor.h"
#include <iostream>

AngajatCititor::AngajatCititor() : Persoana(), Angajat(), Cititor() {
    fout<<"Angajat-Cititor creat"<<"\n";
}

AngajatCititor::AngajatCititor(const AngajatCititor& ac) : Persoana(ac), Angajat(ac), Cititor(ac) {
    fout << "Constructor copiere AngajatCititor: " << (getNume().empty() ? "Anonim" : getNume() ) << "\n";
}

AngajatCititor::AngajatCititor(std::string n, int v, char g, float salariu, int anAngajare, Departament dep, float buget, bool cardFidelitate, std::vector<int> idCarti)
    : Persoana(n, v, g), Angajat(n, v, g, salariu, anAngajare, dep), Cititor(n, v, buget, g, cardFidelitate, idCarti) {
        fout << "Constructor AngajatCititor cu parametri: " << (getNume().empty() ? "Anonim" : getNume() ) << "\n";
    }

AngajatCititor::~AngajatCititor() {
    //Distrugerea se face invers: AngajatCititor -> Cititor -> Angajat -> Persoana
    fout << "[Destructor] AngajatCititor: " << (getNume().empty() ? "Anonim" : getNume() ) << "\n";
}

void AngajatCititor::afisare() const {
    std::cout << "Nume:        " << getNume() << "\n";
    std::cout << "Gen:         " << getGen() << "\n";
    std::cout << "Varsta:      " << getVarsta() << "\n";
    std::cout << "Departament: " << departamentToString(getDepartament()) << "\n";
    std::cout << "An angajare: " << getAnAngajare() << "\n";
    std::cout << "Salariu:     " << getSalariu() << " RON\n";
    std::cout << "Buget:       " << getBuget() << " RON\n";
    std::cout << "Card:        " << (getStatus() ? "Da" : "Nu") << "\n";
}

bool AngajatCititor::validare() const {
    return Angajat::validare() && Cititor::validare();
}

Persoana* AngajatCititor::clone() const {
    return new AngajatCititor(*this);
}

std::string AngajatCititor::toString() const {
    return "AngajatCititor[id= " + std::to_string(getId()) + "| nume= " + getNume() + "| salariu= " + std::to_string(getSalariu()) + " RON" + "| buget= " + std::to_string(getBuget()) + " RON" + "| card= " + (getStatus() ? "Da" : "Nu") + "]";
}

AngajatCititor& AngajatCititor::operator=(const AngajatCititor& ac) {
    if (this != &ac) {
        Persoana::operator=(ac);
        Angajat::operator=(ac);
        Cititor::operator=(ac);
    }
    return *this;
}

std::ostream& operator<<(std::ostream& out, const AngajatCititor& ac) {
    out << static_cast<const Persoana&>(ac) << "\n";
    out << "Departament: " << departamentToString(ac.getDepartament()) << "\n";
    out << "An angajare: " << ac.getAnAngajare() << "\n";
    out << "Salariu:     " << ac.getSalariu() << " RON\n";
    out << "Buget:       " << ac.getBuget() << " RON\n";
    out << "Card:        " << (ac.getStatus() ? "Da" : "Nu") << "\n";
    return out;
}

std::istream& operator>>(std::istream& in, AngajatCititor& ac) {
    char buffer[256];
    int v;
    char g;
    float sal, b;
    int anAng, nr;
    Departament dep;
    bool cf;

    std::cout << "Nume: ";
    in.ignore(1000, '\n'); 
    in.getline(buffer, 256);
    ac.setNume(buffer);
    
    std::cout << "Varsta: ";
    in >> v;
    ac.setVarsta(v);
    std::cout << "Gen (m/f/n): ";
    in >> g;
    ac.setGen(g);

    std::cout << "Salariu: ";
    in >> sal;
    ac.setSalariu(sal);

    std::cout << "An angajare: ";
    in >> anAng;
    ac.setAnAngajare(anAng);

    std::cout << "Departament (0-Management, 1-Vanzari, 2-Achizitii, 3-Contabilitate, 4-IT, 5-Resurse Umane): ";
    int d;
    in >> d;
    dep = static_cast<Departament>(d);
    ac.setDepartament(dep);

    std::cout << "Buget: ";
    in >> b;
    ac.setBuget(b);

    std::cout << "Nr carti cumparate: ";
    in >> nr;
    std::vector<int> idTemp;
    for(int i = 0; i < nr; i++){
        std::cout << "ID Carte cumparata "<< i+1 <<": ";
        int id;
        in>>id;
        idTemp.push_back(id);
    }
    ac.setCartiCumparate(idTemp);
    
    std::cout << "Are card fidelitate (1/0): ";
    in >> cf;
    ac.setStatus(cf);

    return in;
}

float AngajatCititor::aplicaReducere(float pret){
    return pret*0.8f;
}