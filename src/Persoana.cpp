#include "Persoana.h"
std::ofstream fout("logs.txt");
int Persoana::nrTotalPersoane = 0;
int IObject::contor = 0;

Persoana::Persoana():  nume("Anonim"), varsta(0), idPersoana(++nrTotalPersoane), gen('N') {
    fout<<"Persoana creata"<<"\n";
}

Persoana::Persoana(const std::string& nume, int varsta, char g):  nume(nume), varsta(varsta), idPersoana(++nrTotalPersoane), gen(g) {
    fout << "Constructor Persoana cu parametri: " << this->nume << "\n";
}

Persoana::Persoana(const Persoana& p): nume(p.nume), varsta(p.varsta), idPersoana(++nrTotalPersoane), gen(p.gen) {
    fout << "Constructor copiere Persoana: " << (this->nume.empty() ? "Anonim" : this->nume) << "\n";
}

Persoana::~Persoana() {
    fout << "Destructor Persoana"<< "\n";
}

Persoana& Persoana::operator=(const Persoana& p) {
    nume=p.nume;
    varsta = p.varsta;
    gen = p.gen;
    return *this;
}

std::ostream& operator<<(std::ostream& out, const Persoana& p) {
    std::cout << "Nume:    " << p.nume << "\n";
    std::cout << "Varsta:  " << p.varsta << "\n";
    std::cout << "Gen:     " << p.gen << "\n";
    return out;
}

std::istream& operator>>(std::istream& in, Persoana& p) {
    std::cout << "Nume: ";
    in.ignore();
    getline(in, p.nume);

    std::cout << "Varsta: ";
    in >> p.varsta;
    in.ignore(256, '\n');
    return in;
}

void Persoana::afisare() const {
    std::cout << "Nume: " << nume << "\n";
    std::cout << "Varsta: " << varsta << "\n";
    std::cout << "Gen: " << gen << "\n";
}

const std::string& Persoana::getNume() const {
    return nume;
}

int Persoana::getVarsta() const {
    return varsta;
}
char Persoana::getGen() const{
    return gen;
}

int Persoana::getId() const {
    return idPersoana;
}

int Persoana::getNrTotalPersoane() {
    return nrTotalPersoane;
}

void Persoana::setNume(const std::string& t){
    this->nume=t;
}

void Persoana::setVarsta(int v) {
    varsta = v;
}
void Persoana::setGen(char g){
    if(g == 'M' || g == 'F' || g=='N') this->gen=g;
}

