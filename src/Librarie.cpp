#include "Librarie.h"
#include "Exceptii.h"
#include "ConfigurareLibrarie.h"

#include <cstring>
#include <cassert>

Librarie::Librarie(): denumire("Librarie noua"), adresa("Adresa necunoscuta"), idLibrarie(++nrLibrariiDeschise){}

//constructor de copiere
Librarie::Librarie(const Librarie& l): denumire(l.denumire), adresa(l.adresa), idLibrarie(++nrLibrariiDeschise), clienti(l.clienti), stoc(l.stoc), angajati(l.angajati), indexTitlu(l.indexTitlu){
    for (Persoana* p : l.persoane)
        persoane.push_back(p->clone());
}

//constructor cu toti parametrii
Librarie::Librarie(const std::string& d, const std::string& a, const std::vector<Carte>& s, const std::vector<Cititor>& c, const std::vector<Angajat>& angajati, const std::vector<Persoana*> p): denumire(d), adresa(a), idLibrarie(++nrLibrariiDeschise), clienti(c), stoc(s), angajati(angajati){
    for (int i = 0; i < (int)stoc.size(); i++)
        indexTitlu[stoc[i].getTitlu()] = i;
    for (Persoana* p : p)
        persoane.push_back(p->clone());

}

//constructor cu denumire si adresa
Librarie::Librarie(const std::string& d, const std::string& a): denumire(d), adresa(a), idLibrarie(++nrLibrariiDeschise){}

//constructor cu denumire, adresa si stoc initial in care si construim indexTitlu
Librarie::Librarie(const std::string& d, const std::string& a, const std::vector<Carte>& s)
    : idLibrarie(++nrLibrariiDeschise), denumire(d), adresa(a), stoc(s) {
    // reconstruim indexul titlu->pozitie
    for (int i = 0; i < (int)stoc.size(); i++)
        indexTitlu[stoc[i].getTitlu()] = i;
}

//destructor apelat manual pt fiecare Persoana* din vector
Librarie::~Librarie(){
    for (Persoana* p : persoane)
        delete p;
    persoane.clear();
}

void Librarie::adaugaPersoana(Persoana* p){
    persoane.push_back(p);
}

void Librarie::afiseazaPersoane() const {
    bool ceva = false;
    if (!angajati.empty()) {
        std::cout << "--- Angajati ---\n";
        std::for_each(angajati.begin(), angajati.end(),
            [](const Angajat& a) {
                 a.afisare(); 
                 std::cout << "\n"; 
            });
        ceva = true;
    }

    if(!clienti.empty()){
        std::cout << "--- Clienti ---\n";
        std::for_each(clienti.begin(), clienti.end(), 
        [](const Cititor& c){
            c.afisare();
            std::cout<<"\n";
        });
        ceva=true;
    }

    if(!persoane.empty()){
        std::cout << "--- Angajati-Cititori & Manageri---" << "\n";
        for(Persoana* p : persoane){
            p->afisare();
            std::cout<<"\n";
        }
        ceva = true;
    }

    if(!ceva)
        std::cout << "Nu exista persoane.\n";
}


//supraincarcarea operatorului =
Librarie& Librarie::operator=(const Librarie& l){
    if(this == &l) return *this;
    denumire=l.denumire;
    adresa=l.adresa;
    stoc=l.stoc;
    angajati=l.angajati;
    clienti=l.clienti;
    indexTitlu=l.indexTitlu;
    persoane.clear();
    for (Persoana* p : l.persoane)
        persoane.push_back(p->clone());

    return *this;
}

// I/O
std::ostream& operator<<(std::ostream& out, const Librarie& l){
    out<<"--------------------------------------------------"<<"\n";
    out<<"Libraria: "<<(l.denumire.empty() ? "Nume necunoscut" : l.denumire)<<"\n";
    out<<"ID: "<<l.idLibrarie<<"\n";
    out<<"Adresa: "<<(l.adresa.empty() ? "Adresa necunoscuta" : l.adresa)<<"\n";
    out<<"--------------------------------------------------"<<"\n";

    if(!l.angajati.empty()){
        out<<"Angajati disponibili: "<<l.angajati.size()<<"\n";
        for(const auto& a: l.angajati)
            out<<a<<"\n";
    }
    else out<<"Niciun angajat disponibil"<<"\n";

    if(!l.stoc.empty()){
        out<<"Carti in stoc: "<<l.stoc.size()<<"\n";
        for(const auto& c: l.stoc)
            out<<c<<"\n";
    }
    else out<<"Stocul este momentan gol"<<"\n";

    if(!l.clienti.empty()){
        out<<"Clienti disponibili: "<<l.clienti.size()<<"\n";
        for(const auto& c: l.clienti)
            out<<c<<"\n";
    }
    else out<<"Niciun client inregistrat"<<"\n";

    return out;
}

std::istream& operator>>(std::istream& in, Librarie& l){
    std::cout<<"-------Introduceti date despre librarie-------"<<"\n";
    std::cout<<"Denumire: ";
    char buffer[256];
    in.ignore();
    in.getline(buffer, 256);
    l.denumire=buffer;

    std::cout<<"Adresa: ";
    in.getline(buffer, 256);
    l.adresa=buffer;

    std::cout<<"Cate carti adaugati in stoc? ";
    int nrS;
    in>>nrS;
    l.stoc.clear();
    for(int i=0; i<nrS; i++){
        std::cout<<"\n"<<"Cartea "<<i+1<<": ";
        Carte c;
        in>>c;
        l.stoc.push_back(c);
    }

    std::cout<<"Cati clienti inregistrati? ";
    int nrC;
    in>>nrC;
    l.clienti.clear();
    for(int i=0; i<nrC; i++){
        std::cout<<"\n"<<"Clientul "<<i+1<<": ";
        Cititor c;
        in>>c;
        l.clienti.push_back(c);
    }

    std::cout<<"Cati angajati inregistrati? ";
    int nrA;
    in>>nrA;
    l.angajati.clear();
    for(int i=0; i<nrA; i++){
        std::cout<<"\n"<<"Angajatul "<<i+1<<": ";
        Angajat a;
        in>>a;
        l.angajati.push_back(a);
    }
    return in;
}

//supraincarcarea operatorului de indexare
//accesam o carte din stoc

Carte& Librarie::operator[](int index){
    assert(index>=0 && index<(int)stoc.size());
    return stoc[index];
}

//supraincarcarea operatorilor ++, -- prefixat
//eliminam sau adaugam o carte in stoc
Librarie& Librarie::operator++(){
    stoc.push_back(Carte());
    return *this;
}

Librarie& Librarie::operator--(){
    //se elimina ultima carte
    if (!stoc.empty()) stoc.pop_back();
    return *this;
}

//supraincarcarea operatorilor aritmetici 
//librarie + carte -> se adauga cartea respectiva in stocul librariei
//librarie - float -> se va aplica reducrerea reprezentata de float peste intreg stocul

Librarie Librarie::operator+(const Carte& c) const {
    Librarie copie=*this;
    copie.stoc.push_back(c);
    return copie;
}

Librarie Librarie::operator-(const float reducere) const{
    //folosim Singleton ConfiguratieLibrarie pentru a verifica faptul ca reducerea nu depaseste limita maxima permisa
    if(!CONFIG.reducerePermisa(reducere))
        throw ValidationError("reducere", reducere);

    Librarie copie = *this;
    for(auto& c : copie.stoc)
        if(c.getPret() > 0)
            c.setPret(c.getPret() * (1.0f - reducere / 100.0f));
    return copie;
}
Librarie operator+(const Carte& c, const Librarie& l) { return l + c; }
//supraincarcareaoperatorului ==
//doua librarii sunt egale daca au aceeasi denumire si acelasi nume de carti

bool Librarie::operator==(const Librarie& l) const{
    if(this->stoc.size() != l.stoc.size()) return false;
    if(this->denumire!=l.denumire) return false;

    return true;
}

//supraincarcarea operatorilor <, >, <=, >=
//compararea se face in functie de numarul de carti de pe stoc

bool Librarie::operator<(const Librarie& l) const{
    return this->stoc.size()<l.stoc.size();
}

bool Librarie::operator>(const Librarie& l) const{
    return this->stoc.size()>l.stoc.size();
}

bool Librarie::operator<=(const Librarie& l) const{
    return this->stoc.size()<=l.stoc.size();
}

bool Librarie::operator>=(const Librarie& l) const{
    return this->stoc.size()>=l.stoc.size();
}

//getteri si setteri
const std::string& Librarie::getDenumire() const{
    return denumire;
}

const std::string& Librarie::getAdresa() const{
    return adresa;
}

int Librarie::getIdLibrarie() const{
    return this->idLibrarie;
}

std::vector<Carte> Librarie::getStoc() const{
    return stoc;
}

std::vector<Cititor> Librarie::getClienti() const{
    return clienti;
}
std::vector<Angajat> Librarie::getAngajati() const{
    return angajati;
}

int Librarie::getNrLibrariiDeschise() {
    return nrLibrariiDeschise;
}

std::vector<Persoana*> Librarie::getPersoane() const {
        std::vector<Persoana*> copie;
        for (Persoana* p : persoane)
            copie.push_back(p->clone());
        return copie;
    }

Cititor& Librarie::getClient(int index) {
    if (index < 0 || index >= (int)clienti.size())
        throw std::out_of_range("Index client invalid");
    return clienti[index];
}
 
Angajat& Librarie::getAngajat(int index) {
    if (index < 0 || index >= (int)angajati.size())
        throw std::out_of_range("Index angajat invalid");
    return angajati[index];
}
 
Carte& Librarie::getCarte(int index) {
    if (index < 0 || index >= (int)stoc.size())
        throw std::out_of_range("Index carte invalid");
    return stoc[index];
}

void Librarie::setDenumire(const std::string& c){
    if(c.empty()) throw ValidationError("denumire", 0);
    this->denumire=c;
}

void Librarie::setAdresa(const std::string& c){
    if(c.empty()) throw ValidationError("adresa", 0);
    this->adresa=c; 
}

void Librarie::adaugaCarte(const Carte& c)  { stoc.push_back(c); }
void Librarie::adaugaClient(const Cititor& c)  { clienti.push_back(c); }
void Librarie::adaugaAngajat(const Angajat& a) { angajati.push_back(a); }

void Librarie::setStoc(const std::vector<Carte>& v) {
    stoc = v;
    indexTitlu.clear();
    for (int i = 0; i < (int)stoc.size(); i++)
        indexTitlu[stoc[i].getTitlu()] = i;
}
void Librarie::setClienti(const std::vector<Cititor>& v)  { clienti = v; }
void Librarie::setAngajati(const std::vector<Angajat>& v) { angajati = v; }
void Librarie::setPersoane(const std::vector<Persoana*>& v) {
    for (Persoana* p : persoane)
        delete p;
    persoane.clear();
    for (Persoana* p : v)
        persoane.push_back(p->clone());
}

void Librarie::stergeClient(int index) {
    if (index < 0 || index >= (int)clienti.size())
        throw std::out_of_range("Index client invalid");
    clienti.erase(clienti.begin() + index);
}
 
void Librarie::stergeAngajat(int index) {
    if (index < 0 || index >= (int)angajati.size())
        throw std::out_of_range("Index angajat invalid");
    angajati.erase(angajati.begin() + index);
}
 
void Librarie::stergeCarte(int index) {
    if (index < 0 || index >= (int)stoc.size())
        throw std::out_of_range("Index carte invalid");
    indexTitlu.erase(stoc[index].getTitlu());
    stoc.erase(stoc.begin() + index);

    //pt a reface indexTitlu
    for (int i = 0; i < (int)stoc.size(); i++)
        indexTitlu[stoc[i].getTitlu()] = i;
}




//Algoritmi STL
// 1. sort angajati dupa salariu crescator
void Librarie::sorteazaAngajatiDupaSalariu() {
    std::sort(angajati.begin(), angajati.end(), [](const Angajat& a, const Angajat& b){
        return a.getSalariu() < b.getSalariu();
    });
}

// 2. sort carti dupa pret crescator
void Librarie::sorteazaCartiDupaPret() {
    std::sort(stoc.begin(), stoc.end(), [](const Carte& a, const Carte& b){
        return a.getPret() < b.getPret();
    });
}

// 3. filtreaza carti cu pret <= pretMax 
std::vector<Carte> Librarie::filtreazaCartiDupaPret(float pretMax) const {
    std::vector<Carte> rez;
    //back_inserter apeleaza de fiecare data .push_back pe vectorul rez pt fiecare rezultat care trece testul de filtrare
    std::copy_if(stoc.begin(), stoc.end(), std::back_inserter(rez), 
        [pretMax](const Carte& c){
            return c.getPret()<=pretMax;
        });
    return rez;
}

// 4. filtreaza clienti care au card de fidelitate 
std::vector<Cititor> Librarie::clientiCuCard() const {
    std::vector<Cititor> rez;
    std::copy_if(clienti.begin(), clienti.end(), std::back_inserter(rez),
     [](const Cititor& c){
        return c.getStatus();
    });
    return rez;
}

//5. lienti cu buget peste limita
int Librarie::nrClientiCuBugetPeste(float limita) const {
    return (int)std::count_if(clienti.begin(), clienti.end(), 
    [limita](const Cititor& c){
        return c.getBuget() >limita;
    });
}

//6. gasirea unei carti dupa un titlu
Carte* Librarie::cautaCarte(const std::string& titlu) {
    auto it=std::find_if(stoc.begin(), stoc.end(), [&titlu](const Carte& c){
        return c.getTitlu()==titlu;
    });
    return (it != stoc.end()) ? &(*it) : nullptr;
}

//7. eliminam cartile care au pretul sub un anumit pret minim
void Librarie::stergeCartiIeftine(float pretMin) {
    stoc.erase(
        std::remove_if(stoc.begin(), stoc.end(),
            [pretMin](const Carte& c) { return c.getPret() < pretMin; }),
        stoc.end());
    
    indexTitlu.clear();
    for (int i = 0; i < (int)stoc.size(); i++)
        indexTitlu[stoc[i].getTitlu()] = i;
}

int Librarie::nrLibrariiDeschise = 0;
