#include "Cititor.h"

// static
int Cititor::nrTotalCititori = 0;

//constructor fara parametrii
//vector<float> este initializat automat gol
Cititor::Cititor(): Persoana("Anonim", 0, 'N'), idCititor(++nrTotalCititori), buget(0.0f), areCardFidelitate(false){
    fout<<"Cititor creat!"<<"\n";
}

//constructor de copiere
Cititor::Cititor(const Cititor& c): Persoana(c), idCititor(++nrTotalCititori), buget(c.buget), areCardFidelitate(c.areCardFidelitate), idCartiCumparate(c.idCartiCumparate){
    fout << "Constructor copiere cititor: " << (getNume().empty() ? "Anonim" : getNume()) << "\n";
}

//constructor cu toti parametrii
Cititor::Cititor(const std::string& n, int v, float b,  char g, bool cf, const std::vector<int>& c): Persoana(n, v, g), idCititor(++nrTotalCititori), buget(b), areCardFidelitate(cf), idCartiCumparate(c){
    fout << "Constructor Cititor cu parametri: " << (n.empty() ? "Anonim" : n) << "\n";
}

//constructor doar cu parametrul buget (pentru clientii anonimi care nu vor sa introduca datele personale)
Cititor::Cititor(float b): Persoana("Anonim", 0, 'N'), idCititor(++nrTotalCititori), buget(b), areCardFidelitate(false){
    fout << "Constructor Cititor anonim cu buget"<< "\n";
}

//constructor cu parametrii nume si gen pentru clientii care inca nu s au decis la un buget
Cititor::Cititor(const std::string& n, char g): Persoana(n, 0, g), idCititor(++nrTotalCititori), buget(0.0f), areCardFidelitate(false){
    fout << "Constructor Cititor cu parametri nume si gen: " << (n.empty() ? "Anonim" : n ) <<"\n";
}


//destructor
//odata cu trecerea la stl nu mai este necesar, doar pt ilustratrea ordinii de distrugere al obiectelor
Cititor::~Cititor(){
    fout<<"Destructor cititor"<< "\n";
}
//metode virtuale implementate
void Cititor::afisare() const {
    //apelarea metodei din baza pentru a extinde comportamentul
    //Persoana::afisare();
    std::cout << *this;
}
 
bool Cititor::validare() const {
    return !getNume().empty() && buget >= 0;
}
 
Persoana* Cititor::clone() const {
    return new Cititor(*this);
}

std::string Cititor::toString() const {
    return "Cititor[id= " + std::to_string(getId()) + "| nume= " + getNume() + "| buget= " + std::to_string(buget) + " RON" + "| card= " + (areCardFidelitate ? "Da" : "Nu") + "]";
}

//supraincarcarea operatorului =
Cititor& Cititor::operator=(const Cititor& c){
    if(this==&c) return *this;
    Persoana::operator=(c);

    this->buget=c.buget;
    this->idCartiCumparate=c.idCartiCumparate;
    this->areCardFidelitate=c.areCardFidelitate;
    return *this;
}

//supraincarcarea operatorilor >>, <<
std::istream& operator>>(std::istream& in, Cititor& c){
    std::cout<<"Va rugam sa introduceti datele clientului: "<<"\n";
    std::cout << "Nume complet: ";
    char buffer[256];
    in.ignore();
    in.getline(buffer, 256);
    c.setNume(buffer); 

    char g;
    std::cout<<"Gen: ";
    in>>g;
    c.setGen(g);

    std::cout<<"Varsta: ";
    int v; in >> v;
    c.setVarsta(v);

    std::cout<<"Buget: ";
    in>>c.buget;
    std::cout<<"Card de fidelitate? (1/0): ";
    in>>c.areCardFidelitate;

    std::cout<<"Cate carti are deja in colectie? ";
    int nrC;
    in>>nrC;
    c.idCartiCumparate.clear();
    for (int i=0; i<nrC; i++) {
        std::cout << "ID Carte cumparata [" << i + 1 << "]: ";
        int ind;
        in >> ind;
        c.idCartiCumparate.push_back(ind);
    }
    return in;
    
}

std::ostream& operator<<(std::ostream& out, const Cititor& c){
    out<<"Profilul Cititorului "<<(c.nume.empty() ? "Anonim" : c.nume)<<" (ID "<<c.idCititor<<"):"<<"\n";
    out << "Gen:     " << c.gen << "\n";
    out << "Varsta:  " << c.varsta << "\n";
    out << "Buget:   " << c.buget << " RON\n";
    out << "Statut:  " << (c.areCardFidelitate ? "Client fidel" : "Client temporar") << "\n";
    if(!c.idCartiCumparate.empty()){
        out << "Istoric: ";
        for(int i=0; i<(int)c.idCartiCumparate.size(); i++)
            out << c.idCartiCumparate[i] << " ";
        out << "\n";
    } else out << "Istoric: Nicio achizitie\n";

    return out;

}

//supraincarcarea operatorului de indexare []
//returnam id ul cartii cumparate cu indexul respectiv
int Cititor::operator[](int index){
    assert(index>=0 && index<idCartiCumparate.size());
    return idCartiCumparate[index];
}

//supraincarcarea operatorilor ++, -- prefixati
//se va modifica bugetul cititorului in functie de operatie

Cititor& Cititor::operator++(){
    this->buget+=10.0f;
    return *this;
}
Cititor& Cititor::operator--(){
    if(this->buget>=5.0f){
        this->buget-=5.0f;
    }
    else std::cout<<"Buget insuficient pentru a scadea suma"<<"\n";
    return *this;
}

//supraincarcarea operatorilor matematici
//cititor + float -> adauga float ul la buget
//cititor - float -> scade float ul din buget
//cititor + int -> adauga un nou id de carte cumparata

Cititor Cititor::operator+(float s) const{
    Cititor copie=*this;
    if(s>0) copie.buget+=s;
    return copie;
}

Cititor Cititor::operator-(float s) const {
    Cititor copie=*this;
    if(s>0 && copie.buget-s>=0) copie.buget-=s;
    return copie;
}

Cititor Cititor::operator+(int id) const{
    Cititor copie=*this;
    copie.idCartiCumparate.push_back(id);
    return copie;
}

//supraincarcarea operatorului ==
//doi cititori sunt egali daca au acelasi nume, acelasi gen si acelasi buget
bool Cititor::operator==(const Cititor& c) const{
    if(this->buget != c.buget) return false;
    if(this->gen != c.gen) return false;
    
    if(this->nume != c.nume) return false;

    return true;
}

//supraincarcarea >, <, >=, <=
//compararea se face in functie de buget

bool Cititor::operator>(const Cititor& c) const{
    return this->buget > c.buget;
}
bool Cititor::operator<(const Cititor& c) const{
    return this->buget < c.buget;
}
bool Cititor::operator>=(const Cititor& c) const{
    return this->buget >= c.buget;
}
bool Cititor::operator<=(const Cititor& c) const{
    return this->buget <= c.buget;
}

//setteri si getteri
const std::string& Cititor::getNume() const{
    return nume;
}

float Cititor::getBuget() const{
    return this->buget;
}


std::vector<int> Cititor::getCartiCumparate() const{
    std::vector<int> copie =this->idCartiCumparate;
    return copie;
}

char Cititor::getGen() const{
    return this->gen;
}

bool Cititor::getStatus() const{
    return this->areCardFidelitate;
}

int Cititor::getId() const{
    return this->idCititor;
}


void Cititor::setBuget(float b){
    if(b >= 0.0) this->buget=b;
}

void Cititor::setCartiCumparate(const std::vector<int>& c){
    this->idCartiCumparate=c;
}

void Cititor::setStatus(bool status){
    this->areCardFidelitate=status;
}

Cititor operator+(float suma, const Cititor& c) {return c + suma; }