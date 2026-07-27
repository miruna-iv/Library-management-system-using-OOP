#include "Carte.h"
#include "Exceptii.h"
#include <iostream>
using namespace std;
int Carte::nrCartiInventar = 0;

std::string categorieToString(Categorie c) {
    switch(c) {
        case FICTIUNE: return "Fictiune";
        case ROMANCE: return "Romance";
        case THRILLER: return "Thriller";
        case SCIENCE_FICTION: return "Science Fiction";
        case FANTASY: return "Fantasy";
        case HORROR: return "Horror";
        case AVENTURA: return "Aventura";
        case ISTORIE: return "Istorie";
        case POEZIE: return "Poezie";
        case DRAMATURGIE: return "Dramaturgie";
        case DEZVOLTARE_PERSONALA: return "Dezvoltare personala";
        case BIOGRAFIE: return "Biografie";
        default: return "Necunoscut";
    }
}

//constructor fara parametrii
// vector<float> se initializeaza gol implicit
Carte::Carte(): titlu("Anonim"), id(++nrCartiInventar), scriitor(), anPublicare(0), nrPag(0), categorie(NECUNOSCUT), pret(0.0){}

//constructor de copiere care acum cu trecerea la stl nu mai este necesar
//Carte::Carte(const Carte& carte): titlu(carte.titlu), id(++nrCartiInventar), scriitor(carte.scriitor), anPublicare(carte.anPublicare), nrPag(carte.nrPag), categorie(carte.categorie), pret(carte.pret), rating(carte.rating){}

//constructor cu toti parametrii
Carte::Carte(const std::string& t, int an, int pag, Categorie cat, float p, const std::vector<float>& r,  Autor a): titlu(t), id(++nrCartiInventar), scriitor(a), anPublicare(an), nrPag(pag), categorie(cat), pret(p), rating(r){}

//constructor doar cu parametrii titlu, autor si categorie
Carte::Carte(const std::string& t, Autor a, Categorie c): titlu(t), id(++nrCartiInventar), scriitor(a), anPublicare(0), nrPag(0), categorie(c), pret(0.0){}

//constructor doar cu parametrii titlu si pret
Carte::Carte(const std::string& t, float p): titlu(t), id(++nrCartiInventar), scriitor(), anPublicare(0), nrPag(0), categorie(NECUNOSCUT), pret(p){}

//destructor care acum cu trecerea la stl nu mai este necesar
//Carte::~Carte(){}

//supraincarcarea operatorului =
Carte& Carte::operator=(const Carte& carte){
    if(this==&carte) return *this;
    this->titlu=carte.titlu;
    this->anPublicare=carte.anPublicare;
    this->nrPag=carte.nrPag;
    this->categorie=carte.categorie;
    this->pret=carte.pret;
    this->rating = carte.rating;
    this->scriitor=carte.scriitor;

    return *this;
}

//supraincarcarea operatorilor <<, respectiv >>
std::ostream& operator<<(std::ostream& out, const Carte& carte){
    out<<"Detalii despre titlul "<<(carte.titlu.empty() ? "Nespecificat" : carte.titlu)<<":"<<endl;
    out<<"Informatii despre autor: "<<carte.scriitor;
    out<<"Categorie: "<<categorieToString(carte.categorie)<<endl;
    out<<"Anul publicarii: "<<carte.anPublicare<<endl;
    out<<"Nr. de pagini: "<<carte.nrPag<<endl;
    if(!carte.rating.empty()){
    out<<carte.rating.size()<<" rating-uri, de: ";
    for(float r: carte.rating){
        out<<r<<" ";
    }}
    else out<<"Niciun rating disponibil momentan";
    out<<endl<<"Pret: "<<carte.pret<<endl;
    
    return out;
}

istream& operator>>(istream& in, Carte& carte){
    cout<<"----Informatii despre carte---"<<endl;
    cout<<"Titlul: ";
    in.ignore();
    getline(in, carte.titlu);

    cout<<endl<<"Anul publicarii: ";
    in>>carte.anPublicare;

    cout<<endl<<"Nr. de pagini: ";
    in>>carte.nrPag;

    int optiune;
    cout << "Categorie (0:NECUNOSCUT, 1:FICTIUNE, 2:ROMANCE, 3:THRILLER, 4:SF, 5:FANTASY, \n"
        << "           6:HORROR, 7:AVENTURA, 8:ISTORIE, 9:POEZIE, 10:DRAMA, 11:DEZV_PERS, 12:BIOGRAFIE): ";
    in >> optiune;
    if (optiune >= 0 && optiune <= 12) {
        carte.categorie = (Categorie)optiune;
    } else {
        carte.categorie = NECUNOSCUT;
    }

    cout<<endl<<"Pret: ";
    in>>carte.pret;

    cout<<endl<<"Cate Rating-uri introduceti? ";
    int nrR=0;
    in>>nrR;
    carte.rating.clear();
    for(int i=0; i<nrR; i++){
        float r;
        cout<<endl<<"Rating "<<i+1<<": ";
        in>>r;
        carte.rating.push_back(r);
        
    }
    cout<<endl<<"---Date Autor---"<<endl;
    in>>carte.scriitor;

    return in;
}

//supraincarcarea operatorului de indexare
float& Carte::operator[](int index){
    assert(index >= 0 && index < this->rating.size());
    return this->rating[index];
}

//supraincarcarea operatorilor ++ si -- prefixat: crestem / scadem pretul cu verificarea sa nu fie <0
Carte& Carte::operator++(){
    ++(this->pret);
    return *this; 
}

Carte& Carte::operator--(){
    if(this->pret-1.0>0) --(this->pret);
    else cout<<"Nu se poate scadea pretul acestei carti"<<endl;
    return *this; 
}

//supraincarcarea operatorilor matematici
//carte + float -> adauga un rating nou
//carte + carte -> aduna preturile celor doua carti in cazul in care cineva cumpara mai multe titluri diferite
//carte * int -> carte.pret * int in cazul in care ineva cumpara mai multe exemplare ale aceleiasi carti

Carte Carte::operator+(float r) const{
    Carte copie = *this;
    copie.rating.push_back(r);
    return copie;
}

float Carte::operator*(int n) const{
    if(n>0) return this->pret*(float)n;
    else return 0.0f;
}

float Carte::operator+(const Carte& c) const{
    return this->pret + c.pret;
}

//supraincarcarea operatorului ==
//doua carti sunt egalse daca au acelasi titlu, acelasi autor si acelasi an al publicarii
bool Carte::operator==(const Carte& c1)const{
    if(this->anPublicare!=c1.anPublicare) return false;
    if(this->categorie!=c1.categorie) return false; 
    if(!(this->scriitor==c1.scriitor)) return false;
    if(this->titlu != c1.titlu) return false;

    return true;
}

//supraincarcarea operatorilor >, <, >=, <= 
//comparam pretul cartilor in eventualitatea in care vom vrea mai tarziu sa le ordonam in functie de pret 

bool Carte::operator<(const Carte& c) const{
    return this->pret<c.pret;
}
bool Carte::operator<=(const Carte& c) const{
    return this->pret<=c.pret;
}
bool Carte::operator>(const Carte& c) const{
    return this->pret>c.pret;
}
bool Carte::operator>=(const Carte& c) const{
    return this->pret>=c.pret;
}

//getteri, setteri

const std::string& Carte::getTitlu() const{
    return titlu;
}
int Carte::getAnPublicare() const{
    return this->anPublicare;
}
int Carte::getNrPag() const{
    return this->nrPag;
}
int Carte::getId() const{
    return this->id;
}

//pentru compatibilitate cu Database.h care asteapta float*
float* Carte::getRating() const {
    if (rating.empty()) return nullptr;
    float* copie = new float[rating.size()];
    for (int i = 0; i < (int)rating.size(); i++) copie[i] = rating[i];
    return copie;
}

Categorie Carte::getCategorie() const{
    return this->categorie;
}
float Carte::getPret() const{
    return this->pret;
}
Autor Carte::getScriitor() const{
    return this->scriitor;
}
int Carte::getNrRatinguri() const{
    return (int)rating.size();
}

void Carte::setTitlu(const std::string& t){
    titlu=t;
}
void Carte::setAnPublicare(int an){
    if(an <= 0) throw ValidationError("anPublicare", (float)an);
    this->anPublicare=an;
}
void Carte::setNrPag(int n){
    if(n <= 0) throw ValidationError("nrPag", (float)n);
    this->nrPag=n;
}
void Carte::setPret(float n){
    if(n <= 0) throw ValidationError("pret", n);
    this->pret=n;
}
void Carte::setCategorie(Categorie c){
    this->categorie=c;
}
void Carte::setRatinguri(const vector<float>& r){
    rating.clear();
    rating=r;
}
void Carte::setScriitor(Autor a){
    this->scriitor=a;
}

float operator*(int cantitate, const Carte& c) {
    return c.getPret() * (float)cantitate; 
}