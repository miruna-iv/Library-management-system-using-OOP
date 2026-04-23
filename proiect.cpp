    #include <iostream>
#include <cstring>
#include <cassert>
#include <cmath>

using namespace std;

enum Categorie{
    NECUNOSCUT,
    FICTIUNE,
    ROMANCE,
    THRILLER,
    SCIENCE_FICTION,
    FANTASY,
    HORROR,
    AVENTURA, 
    ISTORIE,
    POEZIE,
    DRAMATURGIE,
    DEZVOLTARE_PERSONALA,
    BIOGRAFIE
};
const char* categorieToString(Categorie c) {
    switch(c) {
        case FICTIUNE: return "Fictiune";
        case ROMANCE: return "ROMANCE";
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
        default: return "NECUNOSCUT";
    }
}

class Autor{
    char* nume;
    int anNastere;
    bool arePremii;
    int nrPremii;
    int nrScrieri;

    public:
        //constructor fara parametrii
        Autor(): nume(nullptr), anNastere(0), arePremii(false), nrPremii(0), nrScrieri(0) {}

        //constructor de copiere
        Autor(const Autor& autor): anNastere(autor.anNastere), arePremii(autor.arePremii), nrPremii(autor.nrPremii), nrScrieri(autor.nrScrieri){
            if(autor.nume != nullptr){
                this->nume = new char[strlen(autor.nume)+1];
                strcpy(this->nume, autor.nume);
            } else {
                this->nume = nullptr;
            }
        }

        //constructor cu toti parametrii
        Autor(const char* n, int an, bool premii, int nr_premii, int nr_scrieri): anNastere(an), arePremii(premii), nrPremii(nr_premii), nrScrieri(nr_scrieri) {
            if(n!=nullptr){
                this->nume=new char[strlen(n)+1];
                strcpy(this->nume, n);
            }
            else this->nume=nullptr;
            
        }

        //constructor cu parametrii nume si anNastere
        Autor(const char* n, int an): anNastere(an), arePremii(false), nrPremii(0), nrScrieri(0) {
            if(n!=nullptr){
                this->nume=new char[strlen(n)+1];
                strcpy(this->nume, n);
            }
            else this->nume=nullptr;
        }

        //constructor cu parametrii nume, an si nr de scrieri
        Autor(const char* n, int an, int nr_scrieri): anNastere(an), arePremii(false), nrPremii(0), nrScrieri(nr_scrieri) {
            if(n!=nullptr){
                this->nume=new char[strlen(n)+1];
                strcpy(this->nume, n);
            }
            else this->nume=nullptr;
        }

        //destructor
        ~Autor(){
            if(nume !=nullptr){
                delete [] nume;
                nume=nullptr;
            }
        }

        //supraincarcarea operatorului =
        Autor& operator=(const Autor& autor){
            if(this==&autor) return *this;

            if (nume != nullptr) {
            delete[] nume;
            nume = nullptr;
            }
            if(autor.nume != nullptr){
                nume = new char[strlen(autor.nume)+1];
                strcpy(nume, autor.nume);
            } else {
                nume = nullptr;
            }
            anNastere=autor.anNastere;
            arePremii=autor.arePremii;
            nrPremii=autor.nrPremii;
            nrScrieri=autor.nrScrieri;
            return *this;
        }

        //supraincarcarea operatorilor <<, respectiv >>
        friend ostream& operator<<(ostream& out, const Autor& autor){
            out << (autor.nume ? autor.nume : "Anonim")
            <<", nascut in "<<autor.anNastere<<", a scris in total "
            <<autor.nrScrieri<<" opere, pentru care a primit "
            <<autor.nrPremii<<" premii."<<endl;
            return out;
        }

        friend istream& operator>>(istream& in, Autor& autor){
            cout<<"Introduceti date despre autor"<<endl<<"Nume: ";
            char buffer[256];
            in.ignore();
            in.getline(buffer, 256);
            if(autor.nume!=nullptr) delete [] autor.nume;
            autor.nume=new char[strlen(buffer)+1];
            strcpy(autor.nume, buffer);
            cout<<"Anul nasterii: ";
            in>>autor.anNastere;
            cout<<"Are premii (1/0): ";
            in>>autor.arePremii;
            cout<<"Numar premii: ";
            in>>autor.nrPremii;
            cout<<"Numar opere: ";
            in>>autor.nrScrieri;
            return in;
        }

        //operatorul de indexare pentru a obtine o anumita litera din nume (atat m a dus capul :))
        char& operator[](int index){
            assert(nume !=nullptr && index >= 0 && index < strlen(nume));
            return nume[index];
        }

        //++ si -- prefixat si postfixat
        //cu cel prefixat voi modifica nrScrieri, iar cu cel postfixat nrPremii
        Autor& operator++(){
            ++(this->nrScrieri);
            return *this;
        }

        Autor& operator--(){
            --(this->nrScrieri);
            return *this;
        }

        Autor operator++(int dummy){
            Autor old = *this;
            ++(this->nrPremii);
            if(this->nrPremii > 0) this->arePremii = true;
            return old;
        }

        Autor operator--(int dummy){
            Autor old = *this;
            --(this->nrPremii);
            if(this->nrPremii <= 0) this->arePremii = false;
            return old;
        }

        //supraincarcarea operatorilor + si -
        //+: cate scrieri au cei doi autori impreuna
        //-: diferenta de varsta dinte cei doi autori

        int operator+(const Autor &autor) const{
            return this->nrScrieri+autor.nrScrieri;
        }

        int operator-(const Autor &autor) const{
            return abs(this->anNastere-autor.anNastere);
        }

        //supraincarcarea operatorului ==: doi autori sunt egali daca 
        //toate valorile atributelor lor sunt egale

        bool operator==(const Autor& autor) const{
            if(this->anNastere != autor.anNastere) return false;
            if(this->arePremii != autor.arePremii) return false;
            if(this->nrPremii != autor.nrPremii) return false;
            if(this->nrScrieri != autor.nrScrieri) return false;

            if (this->nume == nullptr && autor.nume == nullptr) return true;
            if (this->nume == nullptr || autor.nume == nullptr) return false;
            if(strcmp(this->nume, autor.nume) != 0) return false;
            return true;
        }

        //supraincarcarea operatorilor de ordine (<, >, <=, >=)
        //voi compara autorii in functie de nrScrieri

        bool operator<(const Autor& autor) const{
            return this->nrScrieri < autor.nrScrieri;
        }

        bool operator<=(const Autor& autor) const{
            return this->nrScrieri <= autor.nrScrieri;
        }

        bool operator>(const Autor& autor) const{
            return this->nrScrieri > autor.nrScrieri;
        }

        bool operator>=(const Autor& autor) const{
            return this->nrScrieri >= autor.nrScrieri;
        }

        //getteri si setteri
        char* getNume() const{
            if(this->nume==nullptr) return nullptr;
            char* copie= new char[strlen(this->nume)+1];
            strcpy(copie, this->nume);
            return copie;
        }
        int getAnNastere() const {
            return this->anNastere;
        }

        bool getArePremii() const {
            return this->arePremii;
        }

        int getNrPremii() const {
            return this->nrPremii;
        }

        int getNrScrieri() const {
            return this->nrScrieri;
        }

        void setNume(const char* n){
            if(this->nume != nullptr){
                delete [] this->nume;
                this->nume=nullptr;
            }
            if(n!=nullptr){
                this->nume=new char[strlen(n)+1];
                strcpy(this->nume, n);
            }
            else this->nume=nullptr;
        }

        void setAn(const int an){
            if(an>=0 && an<=2026) this->anNastere=an;
        }
        void setPremii(const int nr){
            if(nr>=0) this->nrPremii=nr;
            this->arePremii=(nr>0);
        }
        void setScrieri(const int nr){
            if(nr>=0) this->nrScrieri=nr;
        }

};


class Carte{
    char* titlu;
    int anPublicare;
    int nrPag;
    const int id;
    static int nrCartiInventar;
    float* rating;
    int nrRatinguri;
    Categorie categorie;
    float pret;
    Autor scriitor;

    public:
        //constructor fara parametrii
        Carte(): id(++nrCartiInventar), scriitor(), anPublicare(0), nrPag(0), categorie(NECUNOSCUT), pret(0.0), nrRatinguri(0), rating(nullptr){
            this->titlu=new char[strlen("Anonim")+1];
            strcpy(this->titlu, "Anonim");
            }

        //constructor de copiere
        Carte(const Carte& carte): id(++nrCartiInventar), scriitor(carte.scriitor), anPublicare(carte.anPublicare), nrPag(carte.nrPag), categorie(carte.categorie), pret(carte.pret), nrRatinguri(carte.nrRatinguri){
            if(carte.titlu !=nullptr){
                this->titlu=new char[strlen(carte.titlu)+1];
                strcpy(this->titlu, carte.titlu);
            }
            else this->titlu=nullptr;
            
            if(carte.nrRatinguri>0 && carte.rating!=nullptr){
                this->rating=new float[carte.nrRatinguri];
                for(int i=0; i<carte.nrRatinguri; i++){
                    this->rating[i]=carte.rating[i];
                }
            }
            else{
                this->rating=nullptr;
            }
        }

        //constructor cu toti parametrii
        Carte(const char* t, int an, int pag, Categorie cat, float p, float* r, int nrR, Autor a): id(++nrCartiInventar), scriitor(a), anPublicare(an), nrPag(pag), categorie(cat), pret(p), nrRatinguri(nrR){
            if(t !=nullptr){
                this->titlu=new char[strlen(t)+1];
                strcpy(this->titlu, t);
            }
            else this->titlu=nullptr;

            if(nrR>0 && r!=nullptr){
                this->rating=new float[nrR];
                for(int i=0; i<nrR; i++){
                    this->rating[i]=r[i];
                }
            }
            else{
                this->rating=nullptr;
            }
        }

        //constructor doar cu parametrii titlu, autor si categorie
        Carte(const char* t, Autor a, Categorie c): id(++nrCartiInventar), scriitor(a), anPublicare(0), nrPag(0), categorie(c), pret(0.0){
            if(t !=nullptr){
                this->titlu=new char[strlen(t)+1];
                strcpy(this->titlu, t);
            }
            else this->titlu=nullptr;

            this->nrRatinguri=0;
            this->rating=nullptr;
        }

        //constructor doar cu parametrii titlu si pret
        Carte(const char* t, float p):id(++nrCartiInventar), scriitor(), anPublicare(0), nrPag(0), categorie(NECUNOSCUT), pret(p){
            if(t !=nullptr){
                this->titlu=new char[strlen(t)+1];
                strcpy(this->titlu, t);
            }
            else this->titlu=nullptr;

            this->nrRatinguri=0;
            this->rating=nullptr;
        }

        //destructor
        ~Carte(){
            if(this->titlu != nullptr){
                delete [] this->titlu;
                this->titlu=nullptr;
            }
            if(this->rating != nullptr){
                delete [] this->rating;
                this->rating=nullptr;
            }
        }

        //supraincarcarea operatorului =
        Carte& operator=(const Carte& carte){
            if(this==&carte) return *this;

            if(this->titlu != nullptr){
                delete [] this->titlu;
                this->titlu=nullptr;
            }
            if(this->rating != nullptr){
                delete [] this->rating;
                this->rating=nullptr;
            }

            if(carte.titlu !=nullptr){
                this->titlu=new char[strlen(carte.titlu)+1];
                strcpy(this->titlu, carte.titlu);
            }
            else this->titlu=nullptr;
            if(carte.nrRatinguri>0 && carte.rating!=nullptr){
                this->rating=new float[carte.nrRatinguri];
                for(int i=0; i<carte.nrRatinguri; i++){
                    this->rating[i]=carte.rating[i];
                }
            }
            else{
                this->rating=nullptr;
            }

            this->anPublicare=carte.anPublicare;
            this->nrPag=carte.nrPag;
            this->categorie=carte.categorie;
            this->pret=carte.pret;
            this->nrRatinguri=carte.nrRatinguri;
            this->scriitor=carte.scriitor;

            return *this;
        }

        //supraincarcarea operatorilor <<, respectiv >>
        friend ostream& operator<<(ostream& out, const Carte& carte){
            out<<"Detalii despre titlul "<<(carte.titlu ? carte.titlu : "Nespecificat")<<":"<<endl;
            out<<"Informatii despre autor: "<<carte.scriitor;
            out<<"Categorie: "<<categorieToString(carte.categorie)<<endl;
            out<<"Anul publicarii: "<<carte.anPublicare<<endl;
            out<<"Nr. de pagini: "<<carte.nrPag<<endl;
            if(carte.rating && carte.nrRatinguri>0){
            out<<carte.nrRatinguri<<" rating-uri, de: ";
            for(int i=0; i<carte.nrRatinguri; i++){
                out<<carte.rating[i]<<" ";
            }}
            else out<<"Niciun rating disponibil momentan";
            out<<endl<<"Pret: "<<carte.pret<<endl;
            
            return out;
        }

        friend istream& operator>>(istream& in, Carte& carte){
            cout<<"----Informatii despre carte---"<<endl;
            cout<<"Titlul: ";

            char buffer[256];
            in.ignore();
            in.getline(buffer, 256);

            if(carte.titlu !=nullptr){
                delete [] carte.titlu;
                carte.titlu=nullptr;
            }
            carte.titlu=new char[strlen(buffer)+1];
            strcpy(carte.titlu, buffer);
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
            in>>carte.nrRatinguri;

            if(carte.rating != nullptr){
                delete [] carte.rating;
                carte.rating=nullptr;
            }
            if(carte.nrRatinguri>0){
                carte.rating=new float[carte.nrRatinguri];
                for(int i=0; i<carte.nrRatinguri; i++){
                    cout<<endl<<"Rating "<<i+1<<": ";
                    in>>carte.rating[i];
                }
            }
            else carte.rating=nullptr;

            cout<<endl<<"---Date Autor---"<<endl;
            in>>carte.scriitor;

            return in;
        }

        //supraincarcarea operatorului de indexare
        float& operator[](int index){
            assert(index >= 0 && index < this->nrRatinguri);
            return this->rating[index];
        }

        //supraincarcarea operatorilor ++ si -- prefixat: crestem / scadem pretul cu verificarea sa nu fie <0
        Carte& operator++(){
            ++(this->pret);
            return *this; 
        }

        Carte& operator--(){
            if(this->pret-1.0>0) --(this->pret);
            else cout<<"Nu se poate scadea pretul acestei carti"<<endl;
            return *this; 
        }

        //supraincarcarea operatorilor matematici
        //carte + float -> adauga un rating nou
        //carte + carte -> aduna preturile celor doua carti in cazul in care cineva cumpara mai multe titluri diferite
        //carte * int -> carte.pret * int in cazul in care ineva cumpara mai multe exemplare ale aceleiasi carti

        Carte operator+(float r) const{
            Carte copie = *this;
            float* ratingNou = new float[copie.nrRatinguri + 1];
            for(int i=0; i<copie.nrRatinguri; i++){
                ratingNou[i]=copie.rating[i];
            }
            ratingNou[copie.nrRatinguri]=r;
            copie.nrRatinguri++;
            delete [] copie.rating;
            copie.rating=ratingNou;
            
            return copie;
        }

        float operator*(int n) const{
            if(n>0) return this->pret*(float)n;
            else return 0.0f;
        }

        float operator+(const Carte& c) const{
            return this->pret + c.pret;
        }

        //supraincarcarea operatorului ==
        //doua carti sunt egalse daca au acelasi titlu, acelasi autor si acelasi an al publicarii
        bool operator==(const Carte& c1)const{
            if(this->anPublicare!=c1.anPublicare) return false;
            if(this->categorie!=c1.categorie) return false; 
            if(!(this->scriitor==c1.scriitor)) return false;

            if(this->titlu != nullptr && c1.titlu!=nullptr){
                if(strcmp(this->titlu, c1.titlu)!=0) return false;
            }
            else if(this->titlu == nullptr || c1.titlu == nullptr) return false;

            return true;
        }

        //supraincarcarea operatorilor >, <, >=, <= 
        //comparam pretul cartilor in eventualitatea in care vom vrea mai tarziu sa le ordonam in functie de pret 

        bool operator<(const Carte& c) const{
            return this->pret<c.pret;
        }
        bool operator<=(const Carte& c) const{
            return this->pret<=c.pret;
        }
        bool operator>(const Carte& c) const{
            return this->pret>c.pret;
        }
        bool operator>=(const Carte& c) const{
            return this->pret>=c.pret;
        }

        //getteri, setteri

        char* getTitlu() const{
            if(this->titlu==nullptr) return nullptr;
            char* copie=new char[strlen(this->titlu)+1];
            strcpy(copie, this->titlu);
            return copie;
        }
        int getAnPublicare() const{
            return this->anPublicare;
        }
        int getNrPag() const{
            return this->nrPag;
        }
        int getId() const{
            return this->id;
        }

        float* getRating() const{
            if(this->rating==nullptr) return nullptr;
            float* copie=new float[this->nrRatinguri];
            for(int i=0; i<this->nrRatinguri; i++){
                copie[i]=this->rating[i];
            }
            return copie;
        }
        int getNrRatinguri() const{
            return this->nrRatinguri;
        }
        Categorie getCategorie() const{
            return this->categorie;
        }
        float getPret() const{
            return this->pret;
        }
        Autor getScriitor() const{
            return this->scriitor;
        }

        void setTitlu(const char* t){
            if(this->titlu != nullptr){
                delete [] this->titlu;
                this->titlu=nullptr;
            }
            if(t !=nullptr){
                this->titlu=new char[strlen(t)+1];
                strcpy(this->titlu, t);
            }
            else this->titlu=nullptr;
        }
        void setAnPublicare(int an){
            if(an>0) this->anPublicare=an;
        }
        void setNrPag(int n){
            if(n>0) this->nrPag=n;
        }
        void setPret(float n){
            if(n>0) this->pret=n;
        }
        void setCategorie(Categorie c){
            this->categorie=c;
        }
        void setRatinguri(int nr, float* ratinguri){
            if(this->rating!=nullptr){
                    delete [] this->rating;
                    this->rating=nullptr;
                }
            if(nr>0 && ratinguri!=nullptr){
                this->nrRatinguri=nr;
                this->rating=new float[nr];
                for(int i=0; i<nr; i++){
                    this->rating[i]=ratinguri[i];
                }
            }
            else{
                this->nrRatinguri=0;
                this->rating=nullptr;
            }
        }
        void setScriitor(Autor a){
            this->scriitor=a;
        }
};
int Carte::nrCartiInventar = 0;

float operator*(int cantitate, const Carte& c) {
    return c.getPret() * (float)cantitate; 
}

class Cititor{
    char* nume;
    const int idCititor;
    float buget;
    int nrCartiCumparate;
    int* idCartiCumparate;
    char gen; //m, f sau n(nespecificat)
    bool areCardFidelitate;
    static int nrTotalCititori;

    public:
        //constructor fara parametrii
        Cititor(): idCititor(++nrTotalCititori), buget(0.0f), nrCartiCumparate(0), idCartiCumparate(nullptr), gen('N'), areCardFidelitate(false){
            this->nume = new char[strlen("Anonim") + 1];
            strcpy(this->nume, "Anonim");
        }

        //constructor de copiere
        Cititor(const Cititor& c): idCititor(++nrTotalCititori), buget(c.buget), nrCartiCumparate(c.nrCartiCumparate), gen(c.gen), areCardFidelitate(c.areCardFidelitate){
            if(c.nume!=nullptr){
                this->nume = new char[strlen(c.nume) + 1];
                strcpy(this->nume, c.nume);
            }
            else this->nume=nullptr;
            
            if(c.nrCartiCumparate > 0 && c.idCartiCumparate != nullptr){
                this->idCartiCumparate = new int[c.nrCartiCumparate];
                for(int i=0; i<c.nrCartiCumparate; i++){
                    this->idCartiCumparate[i]=c.idCartiCumparate[i];
                }
            }
            else {
                this->idCartiCumparate = nullptr;
                this->nrCartiCumparate = 0;
            }
        }

        //constructor cu toti parametrii
        Cititor(const char* n, float b, int nr,  char g, bool cf, const int* idCarti=nullptr): idCititor(++nrTotalCititori), buget(b), nrCartiCumparate(nr), gen(g), areCardFidelitate(cf){
            
            if(n!=nullptr){
                this->nume = new char[strlen(n) + 1];
                strcpy(this->nume, n);
            }
            else this->nume=nullptr;
            if(nr > 0 && idCarti != nullptr){
                this->idCartiCumparate = new int[nr];
                for(int i=0; i<nr; i++){
                    this->idCartiCumparate[i]=idCarti[i];
                }
            } 
            else {
                this->idCartiCumparate = nullptr;
                this->nrCartiCumparate = 0;
            }
        }

        //constructor doar cu parametrul buget (pentru clientii anonimi care nu vor sa introduca datele personale)
        Cititor(float b): idCititor(++nrTotalCititori), buget(b), nrCartiCumparate(0), idCartiCumparate(nullptr), gen('N'), areCardFidelitate(false){
            this->nume = new char[strlen("Anonim") + 1];
            strcpy(this->nume, "Anonim");
        }

        //constructor cu parametrii nume si gen pentru clientii care inca nu s au decis la un buget
        Cititor(const char* n, char g): idCititor(++nrTotalCititori), buget(0.0f), nrCartiCumparate(0), idCartiCumparate(nullptr), gen(g), areCardFidelitate(false){
            if(n!=nullptr){
                this->nume = new char[strlen(n) + 1];
                strcpy(this->nume, n);
            }
            else this->nume=nullptr;
        }

        //destructor
        ~Cititor(){
            if(this->nume!=nullptr){
                delete []this->nume;
                this->nume=nullptr;
            }
            if(this->idCartiCumparate!=nullptr){
                delete [] this->idCartiCumparate;
                this->idCartiCumparate=nullptr;
            }
        }

        //supraincarcarea operatorului =
        Cititor& operator=(const Cititor& c){
            if(this==&c) return *this;
            if(this->nume!=nullptr){
                delete [] this->nume;
                this->nume=nullptr;
            }
            if(this->idCartiCumparate!=nullptr){
                delete [] this->idCartiCumparate;
                this->idCartiCumparate=nullptr;
            }

            if(c.nume!=nullptr){
                this->nume = new char[strlen(c.nume) + 1];
                strcpy(this->nume, c.nume);
            }
            else this->nume=nullptr;

            if(c.nrCartiCumparate > 0 && c.idCartiCumparate != nullptr){
                this->idCartiCumparate = new int[c.nrCartiCumparate];
                for(int i=0; i<c.nrCartiCumparate; i++){
                    this->idCartiCumparate[i]=c.idCartiCumparate[i];
                }
            }
            else {
                this->idCartiCumparate = nullptr;
                this->nrCartiCumparate=0;
            }

            this->buget=c.buget;
            this->nrCartiCumparate=c.nrCartiCumparate;
            this->gen=c.gen;
            this->areCardFidelitate=c.areCardFidelitate;
            return *this;
        }

        //supraincarcarea operatorilor >>, <<
        friend istream& operator>>(istream& in, Cititor& c){
            cout<<"Va rugam sa introduceti datele clientului: "<<endl;
            cout<<"Nume complet: ";
            char buffer[256];
            in.ignore();
            in.getline(buffer, 256);
            if(c.nume != nullptr){
                delete [] c.nume;
            }
            c.nume=new char[strlen(buffer)+1];
            strcpy(c.nume, buffer);
            cout<<"Gen: ";
            in>>c.gen;
            cout<<"Buget: ";
            in>>c.buget;
            cout<<"Card de fidelitate? (1/0): ";
            in>>c.areCardFidelitate;

            cout<<"Cate carti are deja in colectie? ";
            in>>c.nrCartiCumparate;

            if(c.idCartiCumparate != nullptr) {
                delete [] c.idCartiCumparate;
                c.idCartiCumparate=nullptr;
            }

            if(c.nrCartiCumparate > 0){
                c.idCartiCumparate=new int[c.nrCartiCumparate];
            for (int i=0; i<c.nrCartiCumparate; i++) {
                cout << "ID Carte cumparata [" << i + 1 << "]: ";
                in >> c.idCartiCumparate[i];
            }

            }
            return in;
            
        }

        friend ostream& operator<<(ostream& out, const Cititor& c){
            out<<"Profilul Cititorului "<<(c.nume ? c.nume : "Anonim")<<" (ID: "<<c.idCititor<<"):"<<endl;
            out<<"Gen: "<<c.gen<<endl;
            out<<"Buget: "<<c.buget<<endl;
            out<<"Statut: "<<(c.areCardFidelitate ? "Client fidel" : "Client temporar")<<endl;
            if(c.nrCartiCumparate>0){
                out<<"Istoric cumparaturi (id-ul cartilor): ";
                for(int i=0; i<c.nrCartiCumparate; i++){
                    out<<c.idCartiCumparate[i]<<" ";
                }
                out<<endl;
            }
            else out<<"Nicio achizitie pana acum"<<endl;

            return out;

        }

        //supraincarcarea operatorului de indexare []
        //returnam id ul cartii cumparate cu indexul respectiv
        int operator[](int index){
            assert(index>=0 && index<nrCartiCumparate);
            return idCartiCumparate[index];
        }

        //supraincarcarea operatorilor ++, -- prefixati
        //se va modifica bugetul cititorului in functie de operatie
        
        Cititor& operator++(){
            this->buget+=10.0f;
            return *this;
        }
        Cititor& operator--(){
            if(this->buget>=5.0f){
                this->buget-=5.0f;
            }
            else cout<<"Buget insuficient pentru a scadea suma"<<endl;
            return *this;
        }

        //supraincarcarea operatorilor matematici
        //cititor + float -> adauga float ul la buget
        //cititor - float -> scade float ul din buget
        //cititor + int -> adauga un nou id de carte cumparata

        Cititor operator+(float s) const{
            Cititor copie=*this;
            if(s>0) copie.buget+=s;
            return copie;
        }

        Cititor operator-(float s) const {
            Cititor copie=*this;
            if(s>0 && copie.buget-s>=0) copie.buget-=s;
            return copie;
        }

        Cititor operator+(int id) const{
            Cititor copie=*this;
            int* nou= new int[copie.nrCartiCumparate+1];
            for(int i=0; i<copie.nrCartiCumparate; i++)
                nou[i]=copie.idCartiCumparate[i];
            nou[copie.nrCartiCumparate]=id;
            delete [] copie.idCartiCumparate;
            copie.idCartiCumparate=nou;
            copie.nrCartiCumparate++;
            return copie;
        }

        //supraincarcarea operatorului ==
        //doi cititori sunt egali daca au acelasi nume, acelasi gen si acelasi buget
        bool operator==(const Cititor& c) const{
            if(this->buget != c.buget) return false;
            if(this->gen != c.gen) return false;
            
            if(this->nume!=nullptr && c.nume!=nullptr){
                if(strcmp(this->nume, c.nume)!=0) return false;
            }
            else if(this->nume == nullptr || c.nume==nullptr) return false;

            return true;
        }

        //supraincarcarea >, <, >=, <=
        //compararea se face in functie de buget

        bool operator>(const Cititor& c) const{
            return this->buget > c.buget;
        }
        bool operator<(const Cititor& c) const{
            return this->buget < c.buget;
        }
        bool operator>=(const Cititor& c) const{
            return this->buget >= c.buget;
        }
        bool operator<=(const Cititor& c) const{
            return this->buget <= c.buget;
        }

        //setteri si getteri
        char* getNume() const{
            if(this->nume==nullptr) return nullptr;
            char* copie=new char[strlen(this->nume)+1];
            strcpy(copie, this->nume);
            return copie;
        }

        float getBuget() const{
            return this->buget;
        }

        int getNrCartiCumparate() const{
            return this->nrCartiCumparate;
        }

        int* getCartiCumparate() const{
            if(this->idCartiCumparate == nullptr) return nullptr;
            int *copie=new int[this->nrCartiCumparate];
            for(int i=0; i<this->nrCartiCumparate; i++)
                copie[i]=this->idCartiCumparate[i];
            return copie;
        }

        char getGen() const{
            return this->gen;
        }

        bool getStatus() const{
            return this->areCardFidelitate;
        }

        int getId() const{
            return this->idCititor;
        }

        void setNume(const char* t){
            if(this->nume != nullptr){
                delete [] this->nume;
                this->nume=nullptr;
            }
            if(t!=nullptr){
                this->nume = new char[strlen(t) + 1];
                strcpy(this->nume, t);
            }
            else this->nume=nullptr;
        }

        void setBuget(float b){
            if(b >= 0.0) this->buget=b;
        }
        void setNrCartiCumparate(int n){
            if(n>=0) this->nrCartiCumparate=n;
        }
        void setCartiCumparate(int* carti, int nr){
            if(this->idCartiCumparate != nullptr){
                delete [] this->idCartiCumparate;
                this->idCartiCumparate=nullptr;
            }
            if(carti != nullptr && nr > 0){
                this->idCartiCumparate=new int[nr];
                for(int i=0; i<nr; i++)
                    this->idCartiCumparate[i]=carti[i];
                this->nrCartiCumparate=nr;
            }
        }
        void setGen(char g){
            if(g == 'M' || g == 'F' || g=='N') this->gen=g;
        }
        void setStatus(bool status){
            this->areCardFidelitate=status;
        }

};
int Cititor::nrTotalCititori=0;
Cititor operator+(float suma, const Cititor& c) {
    return c + suma; 
}


class Librarie{
    const int idLibrarie;
    char *denumire;
    char *adresa;
    Carte* stoc;
    int nrStoc;
    Cititor* clienti;
    int nrClienti;
    static int nrLibrariiDeschise;

    public:
        //constructor fara parametrii
        Librarie(): idLibrarie(++nrLibrariiDeschise), nrStoc(0), nrClienti(0), clienti(nullptr), stoc(nullptr){
            denumire=new char[strlen("Librarie noua")+1];
            strcpy(denumire, "Librarie noua");

            adresa=new char[strlen("Adresa necunoscuta")+1];
            strcpy(adresa, "Adresa necunoscuta");
        }

        //constructor de copiere
        Librarie(const Librarie& l): idLibrarie(++nrLibrariiDeschise), nrStoc(l.nrStoc), nrClienti(l.nrClienti), clienti(nullptr), stoc(nullptr), adresa(nullptr), denumire(nullptr) {
            if(l.denumire!=nullptr){
                this->denumire=new char[strlen(l.denumire)+1];
                strcpy(this->denumire, l.denumire);
            }
            if(l.adresa!=nullptr){
                this->adresa=new char[strlen(l.adresa)+1];
                strcpy(this->adresa, l.adresa);
            }

            if(l.nrStoc>0){
                this->stoc=new Carte[l.nrStoc];
                for(int i=0; i<l.nrStoc; i++)
                    this->stoc[i]=l.stoc[i];
            }

            if(l.nrClienti>0){
                this->clienti=new Cititor[l.nrClienti];
                for(int i=0; i<l.nrClienti; i++)
                    this->clienti[i]=l.clienti[i];
            }
        }

        //constructor cu toti parametrii
        Librarie(const char* d, const char* a, const Carte* s, int nrS, const Cititor* c, int nrC): idLibrarie(++nrLibrariiDeschise), nrStoc(nrS), nrClienti(nrC), clienti(nullptr), stoc(nullptr) , denumire(nullptr), adresa(nullptr){
            if(d!=nullptr){
                this->denumire=new char[strlen(d)+1];
                strcpy(this->denumire, d);
            }
            else this->denumire=nullptr;
            
            if(a!=nullptr){
                this->adresa=new char[strlen(a)+1];
                strcpy(this->adresa, a);
            }
            else this->adresa=nullptr;
            

            if(nrS>0){
                this->stoc=new Carte[nrS];
                for(int i=0; i<nrS; i++)
                    this->stoc[i]=s[i];
            }

            if(nrC>0){
                this->clienti=new Cititor[nrC];
                for(int i=0; i<nrC; i++)
                    this->clienti[i]=c[i];
            }

        }

        //constructor cu denumire si adresa
        Librarie(const char* d, const char* a): idLibrarie(++nrLibrariiDeschise), nrStoc(0), nrClienti(0), clienti(nullptr), stoc(nullptr), denumire(nullptr), adresa(nullptr) {

            if(d!=nullptr){
                this->denumire=new char[strlen(d)+1];
                strcpy(this->denumire, d);
            }
            else this->denumire=nullptr;
            
            if(a!=nullptr){
                this->adresa=new char[strlen(a)+1];
                strcpy(this->adresa, a);
            }
            else this->adresa=nullptr;
        }

        //constructor cu denumire, adresa si stoc initial
        Librarie(const char* d, const char* a, const Carte* s, int nrS) : idLibrarie(++nrLibrariiDeschise), nrClienti(0), clienti(nullptr) {
    
            if(d!=nullptr){
                this->denumire=new char[strlen(d)+1];
                strcpy(this->denumire, d);
            }
            else this->denumire=nullptr;
            
            if(a!=nullptr){
                this->adresa=new char[strlen(a)+1];
                strcpy(this->adresa, a);
            }
            else this->adresa=nullptr;

            this->nrStoc = nrS;
            if (nrS > 0 && s != nullptr) {
                this->stoc = new Carte[nrS];
                for (int i = 0; i < nrS; i++) {
                    this->stoc[i] = s[i];
                }
            }
            else {
                this->stoc = nullptr;
            }
        }

        //destructor
        ~Librarie(){
            if(this->denumire!=nullptr){
                delete [] this->denumire;
                this->denumire=nullptr;
            }
            if(this->adresa!=nullptr){
                delete [] this->adresa;
                this->adresa=nullptr;
            }
            if(this->stoc!=nullptr){
                delete [] this->stoc;
                this->stoc=nullptr;
            }
            if(this->clienti!=nullptr){
                delete [] this->clienti;
                this->clienti=nullptr;
            }
        }

        //supraincarcarea operatorului =
        Librarie& operator=(const Librarie& l){
            if(this == &l) return *this;

            if(this->denumire!=nullptr){
                delete [] this->denumire;
                this->denumire=nullptr;
            }
            if(this->adresa!=nullptr){
                delete [] this->adresa;
                this->adresa=nullptr;
            }
            if(this->stoc!=nullptr){
                delete [] this->stoc;
                this->stoc=nullptr;
            }
            if(this->clienti!=nullptr){
                delete [] this->clienti;
                this->clienti=nullptr;
            }

            if(l.denumire!=nullptr){
                this->denumire=new char[strlen(l.denumire)+1];
                strcpy(this->denumire, l.denumire);
            }
            else this->denumire=nullptr;
            
            if(l.adresa!=nullptr){
                this->adresa=new char[strlen(l.adresa)+1];
                strcpy(this->adresa, l.adresa);
            }
            else this->adresa=nullptr;

            this->nrClienti=l.nrClienti;
            if(this->nrClienti>0){
                this->clienti=new Cititor[l.nrClienti];
                for(int i=0; i<l.nrClienti; i++)
                    this->clienti[i]=l.clienti[i];
            }
            else this->clienti=nullptr;

            this->nrStoc=l.nrStoc;
            if(this->nrStoc>0){
                this->stoc=new Carte[l.nrStoc];
                for(int i=0; i<l.nrStoc; i++)
                    this->stoc[i]=l.stoc[i];
            }
            else this->stoc=nullptr;

            return *this;
        }

        friend ostream& operator<<(ostream& out, const Librarie& l){
            out<<"--------------------------------------------------"<<endl;
            out<<"Libraria: "<<(l.denumire ? l.denumire : "Fara nume")<<endl;
            out<<"ID: "<<l.idLibrarie<<endl;
            out<<"Adresa: "<<(l.adresa ? l.adresa : "Nespecificat")<<endl;
            out<<"--------------------------------------------------"<<endl;

            if(l.nrStoc > 0 && l.stoc!=nullptr){
                out<<"Carti in stoc: "<<l.nrStoc<<endl;
                for(int i=0; i<l.nrStoc; i++)
                    out<<l.stoc[i]<<endl;
            }
            else out<<"Stocul este momentan gol"<<endl;

            if(l.nrClienti > 0 && l.clienti!=nullptr){
                out<<"Numarul de clienti inregistrati:  "<<l.nrClienti<<endl;
                for(int i=0; i<l.nrClienti; i++)
                    out<<l.clienti[i]<<endl;
            }
            else out<<"Niciun client inregistrat"<<endl;

            return out;
        }

        friend istream& operator>>(istream& in, Librarie& l){
            cout<<"-------Introduceti date despre librarie-------"<<endl;
            cout<<"Denumire: ";
            char buffer[256];
            in.ignore();
            in.getline(buffer, 256);
            
            if(l.denumire!=nullptr){
                delete [] l.denumire;
                l.denumire=nullptr;
            }
            l.denumire=new char[strlen(buffer)+1];
            strcpy(l.denumire, buffer);

            cout<<"Adresa: ";
            in.getline(buffer, 256);
            
            if(l.adresa!=nullptr){
                delete [] l.adresa;
                l.adresa=nullptr;
            }
            l.adresa=new char[strlen(buffer)+1];
            strcpy(l.adresa, buffer);

            cout<<"Cate carti adaugati in stoc? ";
            in>>l.nrStoc;

            if(l.stoc != nullptr){
                delete [] l.stoc;
                l.stoc=nullptr;
            }
            if(l.nrStoc>0){
                l.stoc=new Carte[l.nrStoc];
                for(int i=0; i<l.nrStoc; i++){
                    cout<<endl<<"Cartea "<<i+1<<": ";
                    in>>l.stoc[i];
                }
            }

            cout<<"Cati clienti inregistrati? ";
            in>>l.nrClienti;

            if(l.clienti != nullptr){
                delete [] l.clienti;
                l.clienti=nullptr;
            }
            if(l.nrClienti>0){
                l.clienti=new Cititor[l.nrClienti];
                for(int i=0; i<l.nrClienti; i++){
                    cout<<endl<<"Clientul "<<i+1<<": ";
                    in>>l.clienti[i];
                }
            }
            return in;
        }

        //supraincarcarea operatorului de indexare
        //accesam o carte din stoc

        Carte& operator[](int index){
            assert(index>=0 && index<nrStoc);
            return stoc[index];
        }

        //supraincarcarea operatorilor ++, -- prefixat
        //eliminam sau adaugam o carte in stoc
        Librarie& operator++(){
            Carte* stocNou=new Carte[this->nrStoc+1];

            for(int i=0; i<this->nrStoc; i++)
                stocNou[i]=this->stoc[i];

            delete [] this->stoc;
            this->stoc=stocNou;
            this->nrStoc++;
            return *this;
        }

        Librarie& operator--(){
            //se elimina ultima carte
            if(this->nrStoc>0){
                this->nrStoc--;
                if(this->nrStoc==0){
                    delete [] this->stoc;
                    this->stoc=nullptr;
                }
                else{
                    Carte* stocNou=new Carte[this->nrStoc];
                    for(int i=0; i<this->nrStoc; i++)
                        stocNou[i]=this->stoc[i];

                    delete [] this->stoc;
                    this->stoc=stocNou;

                }
            }
            return *this;
        }

        //supraincarcarea operatorilor aritmetici 
        //librarie + carte -> se adauga cartea respectiva in stocul librariei
        //librarie - float -> se va aplica reducrerea reprezentata de float peste intreg stocul

        Librarie operator+(const Carte& c) const {
            Librarie copie= *this;
            Carte* stocNou=new Carte[copie.nrStoc+1];

            for(int i=0; i<copie.nrStoc; i++)
                stocNou[i]=copie.stoc[i];

            stocNou[copie.nrStoc]=c;
            delete [] copie.stoc;
            copie.stoc=stocNou;
            copie.nrStoc++;

            return copie;
        }

        Librarie operator-(const float reducere) const{
            Librarie copie = *this;
            if(reducere>0 && reducere <=100){
                for(int i=0; i<copie.nrStoc; i++){
                    float pretVechi=copie.stoc[i].getPret();
                    float pretNou=(pretVechi-pretVechi*(reducere/100));
                    copie.stoc[i].setPret(pretNou);
                }
            }

            return copie;
        }

        //supraincarcareaoperatorului ==
        //doua librarii sunt egale daca au aceeasi denumire si acelasi nume de carti

        bool operator==(const Librarie& l) const{
            if(this->nrStoc != l.nrStoc) return false;
            if(strcmp(this->denumire, l.denumire)!=0) return false;

            return true;
        }

        //supraincarcarea operatorilor <, >, <=, >=
        //compararea se face in functie de numarul de carti de pe stoc

        bool operator<(const Librarie& l) const{
            return this->nrStoc<l.nrStoc;
        }

        bool operator>(const Librarie& l) const{
            return this->nrStoc>l.nrStoc;
        }

        bool operator<=(const Librarie& l) const{
            return this->nrStoc<=l.nrStoc;
        }

        bool operator>=(const Librarie& l) const{
            return this->nrStoc>=l.nrStoc;
        }

        //getteri si setteri
        char* getDenumire() const{
            if(this->denumire==nullptr) return nullptr;
            char* copie=new char[strlen(this->denumire)+1];
            strcpy(copie, this->denumire);
            return copie;
        }

        char* getAdresa() const{
            if(this->adresa==nullptr) return nullptr;
            char* copie=new char[strlen(this->adresa)+1];
            strcpy(copie, this->adresa);
            return copie;
        }

        int getNrCarti() const{
            return this->nrStoc;
        }

        int getNrClienti() const{
            return this->nrClienti;
        }

        int getIdLibrarie() const{
            return this->idLibrarie;
        }

        Carte* getStoc() const{
            if(this->stoc == nullptr) return nullptr;
            Carte* copie = new Carte[this->nrStoc];
            for(int i=0; i<this->nrStoc; i++)
                copie[i]=this->stoc[i];
            return copie;
        }

        Cititor* getClienti() const{
            if(this->clienti == nullptr) return nullptr;
            Cititor* copie = new Cititor[this->nrClienti];
            for(int i=0; i<this->nrClienti; i++)
                copie[i]=this->clienti[i];
            return copie;
        }

        static int getNrLibrariiDeschise() {
            return nrLibrariiDeschise;
        }

        void setDenumire(const char* c){
            if(this->denumire!=nullptr){
                delete [] this->denumire;
                this->denumire=nullptr;
            }

            if(c!=nullptr){
                this->denumire=new char[strlen(c)+1];
                strcpy(this->denumire, c);
            }
            else this->denumire=nullptr;
            
        }

        void setAdresa(const char* c){
            if(this->adresa!=nullptr){
                delete [] this->adresa;
                this->adresa=nullptr;
            }
            
            if(c!=nullptr){
                this->adresa=new char[strlen(c)+1];
                strcpy(this->adresa, c);
            }
            else this->adresa=nullptr;
        }

        void setStoc(const Carte* c, int nr){
            if(this->stoc!=nullptr){
                    delete [] this->stoc;
                    this->stoc=nullptr;
                }
            if(c!=nullptr && nr>0){
                this->stoc=new Carte[nr];
                for(int i=0; i<nr; i++)
                    this->stoc[i]=c[i];
                this->nrStoc=nr;
            }
            else this->nrStoc=0;
        }

        void setClienti(const Cititor* c, int nr){
            if(this->clienti!=nullptr){
                    delete [] this->clienti;
                    this->clienti=nullptr;
                }
            if(c!=nullptr && nr>0){
                this->clienti = new Cititor[nr];
                for(int i=0; i<nr; i++)
                    this->clienti[i]=c[i];
                this->nrClienti=nr;
            }
            else{
                this->nrClienti=0;
            }
        }
};
int Librarie::nrLibrariiDeschise = 0;
//pentru comutativitate
Librarie operator+(const Carte& c, const Librarie& l) {
    return l+c; 
}

class Meniu{
    private:
        Librarie& librarie;

        void curatareBuffer(){
            cin.ignore(1000, '\n');
        }

        void afiseazaOptiuni(){
            cout<<"\n-----------" << librarie.getDenumire() << " -----------"<<endl;
            cout<<"0. Iesire program"<<endl;
            cout<<"1. Vizualizare inventar (carti si clienti)"<<endl;
            cout<<"2. Adauga carte in stoc"<<endl;
            cout<<"3. Inregistreaza client nou"<<endl;
            cout<<"4. Aplica reducere intregului stoc"<<endl;
            cout<<"5. Simulare Vanzare"<<endl;
            cout<<"6. Adauga suma la bugetul clientului"<<endl;
            cout<<"7. Afisare carti in ordine crescatoare / descrescatoare a pretului"<<endl;
            cout<<"8. Statistici carte"<<endl;
            cout<<"9. Modifica pretul unei carti"<<endl;
            cout<<"10. Raport librarie/autori"<<endl;
            cout<< "----------------------------------------------------------"<<endl; 
        }
    public:
        Meniu(Librarie& lib):librarie(lib){}

        void executaVanzare(){

            int nrCarti=librarie.getNrCarti();
            int nrClienti=librarie.getNrClienti();

            if(nrCarti==0 || nrClienti==0){
                cout<<"Stoc indisponibil sau lista clienti goala"<<endl;
                return;
            }

            cout<<"Selectiati clientul: "<<endl;
            Cititor *listaClienti=librarie.getClienti();
            for(int i=0; i<nrClienti; i++)
                cout<<i<<". "<<listaClienti[i].getNume()<<" (Buget: "<<listaClienti[i].getBuget()<<" RON)"<<endl;
            
            int indexClient;
            cout<<"Index-ul clientului: ";
            cin>>indexClient;

            cout<<"Selectati cartea: "<<endl;
            Carte* listaCarti=librarie.getStoc();
            for(int i=0; i<nrCarti; i++)
                cout<<i<<". "<<listaCarti[i].getTitlu()<<" - "<<listaCarti[i].getPret()<<" RON"<<endl;
            
            int indexCarte;
            cout<<"Index-ul cartii: ";
            cin>>indexCarte;

            if(indexClient>=0 && indexClient<nrClienti && indexCarte>=0 && indexCarte<nrCarti){
                float pretCarte=listaCarti[indexCarte].getPret();
                float bugetClient=listaClienti[indexClient].getBuget();

                if(bugetClient>=pretCarte){
                    //bugetul este actualizat folosindu-ne de supraincarcarea operatorului - din clasa Cititor;
                    listaClienti[indexClient]=listaClienti[indexClient]-pretCarte;

                    //adaugam cartea in istoricul de cumparaturi
                    int idCarte=listaCarti[indexCarte].getId();
                    listaClienti[indexClient]= listaClienti[indexClient]+idCarte;

                    //salvam modificarile si in clasa librarie
                    librarie.setClienti(listaClienti, nrClienti);

                }
                else cout<<"Fonduri insuficiente! Bugetul este cu "<<pretCarte-bugetClient<<" RON mai mic decat pretul cartii"<<endl;
            }
            else cout<<"Index invalid!"<<endl;

            delete [] listaCarti;
            delete [] listaClienti;
        }

        void porneste(){
            afiseazaOptiuni();
            int optiune =-1;
            while(optiune !=0){
                curatareBuffer();
                cout << "Cifra corespunzatoare optiunii: ";
                cin>>optiune;
                

                switch(optiune){
                    case 0: {
                        cout<<"Va multumim si va mai asteptam!"<<endl;
                        break;
                    }
                    case 1:{
                        cout<<librarie;
                        break;
                    }
                    case 2:{
                        Carte c;
                        cin>>c;
                        librarie=librarie+c;
                        cout<<"Cartea a fost adaugata cu succes!"<<endl;
                        break;
                    }
                    case 3:{
                        Cititor cit;
                        cin>>cit;
                        int nrClienti=librarie.getNrClienti();
                        Cititor *vechi=librarie.getClienti();
                        Cititor* nou=new Cititor[nrClienti+1];
                        for(int i=0; i<nrClienti; i++)
                            nou[i]=vechi[i];
                        nou[nrClienti]=cit;
                        librarie.setClienti(nou, nrClienti+1);
                        delete [] vechi;
                        delete [] nou;
                        cout<<"Client inregistrat!"<<endl;
                        break;
                    }
                    case 4:{
                        float p;
                        cout<<"Procent reducere (0-100): ";
                        cin>>p;
                        librarie=librarie-p;
                        cout<<"Reducere aplicata"<<endl;
                        break;

                    }
                    case 5:{
                        executaVanzare();
                        break;
                    }
                    case 6:{
                        int indexClient;
                        cout<<"Introduceti indexul clientului (0-" << librarie.getNrClienti() - 1 << "): ";
                        cin>>indexClient;
                        if(indexClient>=0 && indexClient<librarie.getNrClienti()){
                            float suma;
                            cout<<"Introduceti suma de adaugat: ";
                            cin>>suma;
                            Cititor *nou=librarie.getClienti();
                            nou[indexClient]=nou[indexClient] + suma;
                            librarie.setClienti(nou, librarie.getNrClienti());
                            delete [] nou;
                            cout<<"Suma a fost adaugata cu succes!"<<endl;
                        }
                        else cout<<"Index invalid!"<<endl;

                        break;

                    }
                    case 7:{
                        int nrCarti=librarie.getNrCarti();
                        if(nrCarti==0){
                            cout<<"Nu exista carti in librarie"<<endl;
                            break;
                        }
                        int opt;
                        cout<<"In ce ordine doriti sa fie afisate cartile? (1-Crescator, 2-Descrescator): ";
                        cin>>opt;
                        Carte* stoc= librarie.getStoc();
                        for(int i=0; i<nrCarti-1; i++)
                            for(int j=i+1; j<nrCarti; j++){
                                bool crt=(opt==1 ? stoc[i] > stoc[j] : stoc[i] < stoc[j]);
                                if(crt){
                                    Carte temp=stoc[i];
                                    stoc[i]=stoc[j];
                                    stoc[j]=temp;
                                }
                            }
                            for(int i=0; i<nrCarti; i++)
                                cout<<stoc[i]<<endl;
                            delete [] stoc;
                            break;
                    }
                    case 8:{
                        int nrCarti=librarie.getNrCarti();
                        if(nrCarti==0){
                            cout<<"Nu exista carti in librarie"<<endl;
                            break;
                        }
                        Carte* stoc=librarie.getStoc();
 
                        cout<<"Cartile disponibile:"<<endl;
                        for(int i=0; i<nrCarti; i++){
                            char* t=stoc[i].getTitlu();
                            cout<<i<<". "<<(t?t:"?")<<" - "<<stoc[i].getPret()<<" RON"<<endl;
                            delete[] t;
                        }
 
                        int idx;
                        cout<<"Alegeti o carte (0-"<<nrCarti-1<<"): ";
                        cin>>idx;
 
                        if(idx<0 || idx>=nrCarti){
                            cout<<"Index invalid!"<<endl;
                            delete[] stoc;
                            break;
                        }

                        int nrR=stoc[idx].getNrRatinguri();
                        if(nrR>0){
                            cout<<"Cartea are "<<nrR<<" ratinguri."<<endl;
                            int idxR;
                            cout<<"Alegeti indexul ratingului (0-"<<nrR-1<<"): ";
                            cin>>idxR;
                            if(idxR>=0 && idxR<nrR)
                                cout<<"Rating["<<idxR<<"] = "<<stoc[idx][idxR]<<endl;
                        } else {
                            cout<<"Aceasta carte nu are ratinguri inca."<<endl;
                        }
 
                        float ratingNou;
                        cout<<"Adaugati un rating nou (1.0-5.0): ";
                        cin>>ratingNou;
                        stoc[idx]=stoc[idx]+ratingNou;
                        cout<<"Rating adaugat! Cartea are acum "<<stoc[idx].getNrRatinguri()<<" ratinguri."<<endl;
                        librarie.setStoc(stoc, nrCarti);
 
                        int cantitate;
                        cout<<"Cate exemplare doriti sa cumparati? ";
                        cin>>cantitate;
                        float cost=stoc[idx]*cantitate;
                        float costComutativ=cantitate*stoc[idx];
                        char* titlu=stoc[idx].getTitlu();
                        cout<<cantitate<<" exemplare din \""<<(titlu?titlu:"?")<<"\" costa: "<<cost<<" RON"<<endl;
                        cout<<"(verificare comutativitate: "<<costComutativ<<" RON)"<<endl;
                        delete[] titlu;
 
                        if(nrCarti>=2){
                            int idx2;
                            cout<<"Alegeti o a doua carte pentru comparare (0-"<<nrCarti-1<<"): ";
                            cin>>idx2;
                            if(idx2>=0 && idx2<nrCarti && idx2!=idx){
                                if(stoc[idx]==stoc[idx2])
                                    cout<<"Cele doua carti sunt identice (operator==)."<<endl;
                                else if(stoc[idx]<=stoc[idx2])
                                    cout<<"Prima carte este mai ieftina sau egala (operator<=)."<<endl;
                                else
                                    cout<<"Prima carte este mai scumpa (operator>=)."<<endl;
                                float pretCombinate=stoc[idx]+stoc[idx2];
                                cout<<"Pretul combinat al celor doua carti: "<<pretCombinate<<" RON (operator+ Carte+Carte)"<<endl;
                            }
                        }
 
                        delete[] stoc;
                        break;
                    }
                    case 9:{
                        int nrCarti=librarie.getNrCarti();
                        if(nrCarti==0){
                            cout<<"Nu exista carti in librarie"<<endl;
                            break;
                        }
                        Carte* stoc=librarie.getStoc();
 
                        cout<<"Cartile disponibile:"<<endl;
                        for(int i=0; i<nrCarti; i++){
                            char* t=stoc[i].getTitlu();
                            cout<<i<<". "<<(t?t:"?")<<" - "<<stoc[i].getPret()<<" RON"<<endl;
                            delete[] t;
                        }
 
                        int idx;
                        cout<<"Alegeti cartea al carei pret vreti sa il modificati (0-"<<nrCarti-1<<"): ";
                        cin>>idx;
 
                        if(idx<0 || idx>=nrCarti){
                            cout<<"Index invalid!"<<endl;
                            delete[] stoc;
                            break;
                        }
 
                        int optPret;
                        cout<<"1. Creste pretul cu 1 RON"<<endl;
                        cout<<"2. Scade pretul cu 1 RON"<<endl;
                        cout<<"Alegeti optiunea: ";
                        cin>>optPret;
 
                        float pretVechi=stoc[idx].getPret();
                        char* titlu=stoc[idx].getTitlu();

                        if(optPret==1){
                            ++stoc[idx];
                            cout<<"Pretul cartii \""<<(titlu?titlu:"?")<<"\" a crescut de la "<<pretVechi<<" la "<<stoc[idx].getPret()<<" RON."<<endl;
                        }
                        else{
                            --stoc[idx];
                                cout<<"Pretul cartii \""<<(titlu?titlu:"?")<<"\" a scazut de la "<<pretVechi<<" la "<<stoc[idx].getPret()<<" RON."<<endl;
                        }
                        delete[] titlu;
                        librarie.setStoc(stoc, nrCarti);
                        delete[] stoc;
                        break;
                    }
                    case 10:{
                        int nrCarti=librarie.getNrCarti();
                        int nrClienti=librarie.getNrClienti();
 
                        cout<<endl<<"Raport Librarie"<<endl;
                        cout<<"Libraria curenta are "<<nrCarti<<" carti si "<<nrClienti<<" clienti."<<endl;
                        cout<<"Numar total de librarii deschise (static): "<<Librarie::getNrLibrariiDeschise()<<endl;
                        cout<<"\nIntroduceti datele unei librarii pentru comparatie:"<<endl;
                        Librarie libRef;
                        cin>>libRef;

                        if(librarie==libRef)
                            cout<<"Libraria curenta si libraria de referinta sunt identice (operator==)."<<endl;
                        else if(librarie<libRef)
                            cout<<"Libraria curenta are mai putine carti decat libraria de referinta (operator<)."<<endl;
                        else if(librarie>libRef)
                            cout<<"Libraria curenta are mai multe carti decat libraria de referinta (operator>)."<<endl;
                        else if(librarie<=libRef)
                            cout<<"Libraria curenta are cel mult tot atatea carti ca libraria de referinta (operator<=)."<<endl;
                        else
                            cout<<"Libraria curenta are cel putin tot atatea carti ca libraria de referinta (operator>=)."<<endl;
 
                        if(nrCarti>0){
                            cout<<"\nAcces direct la o carte din stoc:"<<endl;
                            int idx;
                            cout<<"Introduceti indexul cartii (0-"<<nrCarti-1<<"): ";
                            cin>>idx;
                            if(idx>=0 && idx<nrCarti){
                                cout<<librarie[idx];
 
                                float pretVechi=librarie[idx].getPret();
                                ++librarie[idx];
                                cout<<"Pret dupa ++carte: "<<pretVechi<<" RON -> "<<librarie[idx].getPret()<<" RON"<<endl;
 
                                --librarie[idx];
                                cout<<"Pret restaurat cu --carte: "<<librarie[idx].getPret()<<" RON"<<endl;
                            }
                        }
 
                        cout<<"\nRezervare slot nou in stoc:"<<endl;
                        ++librarie;
                        cout<<"Stocul are acum "<<librarie.getNrCarti()<<" slots (ultimul gol)."<<endl;
                        --librarie;
                        cout<<"Slot eliminat. Stoc curent: "<<librarie.getNrCarti()<<" carti."<<endl;
 
                        if(nrCarti>0){
                            Carte* stoc=librarie.getStoc();
                            Librarie copie1=librarie+stoc[0];
                            Librarie copie2=stoc[0]+librarie;
                            char* t=stoc[0].getTitlu();
                            cout<<"\nAdaugare carte in stoc:"<<endl;
                            cout<<"librarie + carte -> "<<copie1.getNrCarti()<<" carti"<<endl;
                            cout<<"carte + librarie -> "<<copie2.getNrCarti()<<" carti"<<endl;
                            delete[] t;
                            delete[] stoc;
                        }
 
                        if(nrClienti>0){
                            Cititor* clienti=librarie.getClienti();
 
                            int idxMax=0, idxMin=0;
                            for(int i=1;i<nrClienti;i++){
                                if(clienti[i]>clienti[idxMax]) idxMax=i;
                                if(clienti[i]<clienti[idxMin]) idxMin=i;
                            }
                            char* nMax=clienti[idxMax].getNume();
                            char* nMin=clienti[idxMin].getNume();
                            cout<<endl<<"Client cu bugetul cel mai mare: "<<(nMax?nMax:"?")<<" ("<<clienti[idxMax].getBuget()<<" RON)"<<endl;
                            cout<<"Client cu bugetul cel mai mic:  "<<(nMin?nMin:"?")<<" ("<<clienti[idxMin].getBuget()<<" RON)"<<endl;
                            delete[] nMax;
                            delete[] nMin;
 
                            if(nrClienti>=2){
                                cout<<(clienti[0]==clienti[1] ? "Primii doi clienti sunt identici." : "Primii doi clienti sunt diferiti.")<<endl;
                            }
 
                            cout<<endl<<"Istoricul de cumparaturi al clientilor:"<<endl;
                            for(int i=0;i<nrClienti;i++){
                                char* n=clienti[i].getNume();
                                int nr=clienti[i].getNrCartiCumparate();
                                cout<<(n?n:"?")<<": ";
                                if(nr==0){ cout<<"nicio achizitie"<<endl; }
                                else{ for(int j=0;j<nr;j++) cout<<"ID "<<clienti[i][j]<<" "; cout<<endl; }
                                delete[] n;
                            }
 
                            for(int i=0;i<nrClienti;i++){
                                if(clienti[i].getStatus()){
                                    float bugetVechi=clienti[i].getBuget();
                                    ++clienti[i];
                                    Cititor test=10.0f+clienti[i];
                                    char* n=clienti[i].getNume();
                                    cout<<"Bonus fidelitate (+10 RON) pentru "<<(n?n:"Client Anonim")<<": "<<bugetVechi<<" -> "<<clienti[i].getBuget()<<" RON"<<endl;
                                    cout<<"  (comutativitate float+client verificata: buget="<<test.getBuget()<<" RON)"<<endl;
                                    delete[] n;
                                }
                            }
 
                            for(int i=0;i<nrClienti;i++){
                                if(!clienti[i].getStatus()){
                                    float bugetVechi=clienti[i].getBuget();
                                    --clienti[i];
                                    char* n=clienti[i].getNume();
                                    cout<<"Taxa administrare (-5 RON) pentru "<<(n?n:"Client Anonim")<<": "<<bugetVechi<<" -> "<<clienti[i].getBuget()<<" RON"<<endl;
                                    delete[] n;
                                }
                            }
 
                            librarie.setClienti(clienti, nrClienti);
                            delete[] clienti;
                        }
 
                        if(nrCarti>=2){
                            Carte* stoc=librarie.getStoc();
                            int idxBest=0;
                            for(int i=1;i<nrCarti;i++){
                                if(stoc[i].getScriitor()>stoc[idxBest].getScriitor()) idxBest=i;
                            }
                            Autor best=stoc[idxBest].getScriitor();
                            char* nb=best.getNume();
                            char* tb=stoc[idxBest].getTitlu();
                            cout<<"\nAutorul cel mai productiv: "<<(nb?nb:"?")<<" ("<<best.getNrScrieri()<<" opere)"<<endl;
                            cout<<"Prima litera din numele sau: "<<best[0]<<endl;
                            delete[] nb; delete[] tb;
 
                            Autor a1=stoc[0].getScriitor();
                            Autor a2=stoc[1].getScriitor();
                            char* n1=a1.getNume(); char* n2=a2.getNume();
                            cout<<endl<<"Comparatie autori: \""<<(n1?n1:"?")<<"\" vs \""<<(n2?n2:"?")<<"\":"<<endl;
                            cout<<"  Total opere impreuna: "<<(a1+a2)<<endl;
                            cout<<"  Diferenta de varsta: "<<(a1-a2)<<" ani"<<endl;
                            if(a1==a2) cout<<"  Autorii sunt identici."<<endl;
                            else if(a1<a2) cout<<"  "<<(n1?n1:"?")<<" are mai putine opere."<<endl;
                            else cout<<"  "<<(n1?n1:"?")<<" are mai multe opere."<<endl;
                            delete[] n1; delete[] n2;
                            cout<<endl<<"Actualizare date autor: "<<a1;
                            ++a1;
                            cout<<"Dupa ++autor (+1 scriere): "<<a1;
                            Autor snap=a1++;
                            cout<<"Dupa autor++ (postfixat, +1 premiu): premii inainte="<<snap.getNrPremii()<<", premii dupa="<<a1.getNrPremii()<<endl;
                            --a1;
                            cout<<"Dupa --autor (-1 scriere): "<<a1;
                            snap=a1--;
                            cout<<"Dupa autor-- (postfixat, -1 premiu): premii inainte="<<snap.getNrPremii()<<", premii dupa="<<a1.getNrPremii()<<endl;
                            cout<<endl<<"Introduceti datele unui autor pentru comparatie cu primul autor din stoc:"<<endl;
                            Autor autorRef;
                            cin>>autorRef;
                            if(a1==autorRef)
                                cout<<"Autorul introdus este identic cu primul autor."<<endl;
                            else if(a1<=autorRef)
                                cout<<"Primul autor are cel mult tot atatea opere ca autorul introdus."<<endl;
                            else
                                cout<<"Primul autor are mai multe opere decat autorul introdus."<<endl;
 
                            delete[] stoc;
                        }
                        break;
                    }
                    
                    default: cout<<"Optiune invalida"<<endl;
                }

            }
        }
};

#include "Database.h"

int main(){
    Database db("librarie.db");
 
    Librarie librarie("Librarie noua", "Adresa necunoscuta");
 
    int idLibrarieAleasa = -1;
    if (db.esteConectat()) {
        idLibrarieAleasa = db.alegeLibrarie();
        if (idLibrarieAleasa != -1) {
            db.incarcaLibrarie(librarie, idLibrarieAleasa);
        }
    }
 
    Meniu meniu(librarie);
    meniu.porneste();
 
    if (db.esteConectat() && idLibrarieAleasa != -1) {
        db.salveazaLibrarie(librarie, idLibrarieAleasa);
    }
    return 0;  
}