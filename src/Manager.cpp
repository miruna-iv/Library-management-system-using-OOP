#include "Manager.h"
#include "Exceptii.h"
#include <iostream>
using namespace std;

Manager::Manager(): Persoana("Anonim", 0, 'N'), Angajat("Anonim", 0, 'N', 0.0f, 0, MANAGEMENT), bonus(0.0f), nrSubalterni(0)
{
    // Ordinea de constructie: Persoana -> Angajat -> Manager
    fout << "Manager creat." << endl;
}

Manager::Manager(const Manager& m): Persoana(m), Angajat(m), bonus(m.bonus), nrSubalterni(m.nrSubalterni)
{
    fout << "Constructor copiere manager: " << (getNume().empty() ? "Anonim" : getNume()) << endl;
}

Manager::Manager(const std::string& n, int v, char g, float sal, int anAng, Departament dep, float b, int nrSub): Persoana(n, v, g), Angajat(n, v, g, sal, anAng, dep), bonus(b), nrSubalterni(nrSub){
    fout << "Constructor manager cu parametri: " << (getNume().empty() ? "Anonim" : getNume()) << endl;
}

Manager::~Manager()  {
    // Distrugerea se face in ordine inversa: Manager -> Angajat -> Persoana
    fout << "Destructor manager" << endl;
}

void Manager::afisare() const {
    //apelam Baza::metoda()
    Angajat::afisare();
    cout << "Bonus:       " << bonus << " RON\n";
    cout << "Subalterni:  " << nrSubalterni << "\n";
}

bool Manager::validare() const {
    return Angajat::validare() && bonus >= 0 && nrSubalterni >= 0;
}

Persoana* Manager::clone() const {
    return new Manager(*this);
}
std::string Manager::toString() const {
    return "Manager[id=" + std::to_string(getId()) + "| nume= " + getNume() + "| salariu= " + std::to_string(getSalariu()) + " RON" + "| bonus= " + std::to_string(bonus) + " RON" + "| subalterni= " + std::to_string(nrSubalterni) + "]";
}

Manager& Manager::operator=(const Manager& m) {
    if (this == &m) return *this;
    Angajat::operator=(m);
    bonus = m.bonus;
    nrSubalterni = m.nrSubalterni;
    return *this;
}

ostream& operator<<(ostream& out, const Manager& m) {
    out << static_cast<const Angajat&>(m);
    out << "Bonus:       " << m.bonus << " RON\n";
    out << "Subalterni:  " << m.nrSubalterni << "\n";
    return out;
}

istream& operator>>(istream& in, Manager& m) {
    //am ales sa nu mai fac upcasting, intrucat in Angajat operator>>, intreba departamentul,
    //iar eu vreau ca in cazul managerului sa fie ales implicit departamentul management, fara ca utilizatorul sa mai fie intrebat

    cout << "--- Date manager ---" << endl;
    cout << "Nume: ";
    char buffer[256];
    in.ignore();
    in.getline(buffer, 256);
    m.setNume(buffer);
    cout << "Gen (M/F/N): "; 
    in>>m.gen;
    cout << "Varsta: ";
    in>>m.varsta;
    cout << "Salariu (RON): ";
    float s;
    in >> s;
    m.setSalariu(s);
    cout << "An angajare: ";
    int an;
    in >> an;
    m.setAnAngajare(an);
    m.setDepartament(MANAGEMENT);
    cout << "Bonus (RON): ";
    in >> m.bonus;
    cout << "Nr subalterni: ";
    in >> m.nrSubalterni;
    return in;
}

float Manager::getBonus() const { return bonus; }
int Manager::getNrSubalterni() const { return nrSubalterni; }

void Manager::setBonus(float b) {
    if (b < 0) throw ValidationError("bonus", b);
    bonus = b;
}
void Manager::setNrSubalterni(int n) { 
    if (n < 0) throw ValidationError("nrSubalterni", (float)n);
    nrSubalterni = n;
}