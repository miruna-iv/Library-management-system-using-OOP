#include "Autor.h"
#include "Exceptii.h"
// constructor fara parametrii
Autor::Autor() : nume(""), anNastere(0), arePremii(false), nrPremii(0), nrScrieri(0) {}

// constructor de copiere
//nu mai este necesar acum ca avem implementat std::string, intrucat memoria este gestionata acum manual de biblioteca
//implementarea manuala a Rule of Three nu mai este necesara
// Autor::Autor(const Autor& autor)
//     : nume(autor.nume), anNastere(autor.anNastere), arePremii(autor.arePremii),
//       nrPremii(autor.nrPremii), nrScrieri(autor.nrScrieri) {}

// destructor
//Autor::~Autor() {}

// operator =
Autor& Autor::operator=(const Autor& autor) {
    if (this == &autor) return *this;

    this->nume=autor.nume;
    anNastere = autor.anNastere;
    arePremii = autor.arePremii;
    nrPremii = autor.nrPremii;
    nrScrieri = autor.nrScrieri;

    return *this;
}



// constructor cu toti parametrii
Autor::Autor(const std::string& n, int an, bool premii, int nr_premii, int nr_scrieri)
    : nume(n), anNastere(an), arePremii(premii), nrPremii(nr_premii), nrScrieri(nr_scrieri) {}

// constructor nume + an
Autor::Autor(const std::string& n, int an): nume(n), anNastere(an), arePremii(false), nrPremii(0), nrScrieri(0) {}

// constructor nume + an + nr scrieri
Autor::Autor(const std::string& n, int an, int nr_scrieri)
    : anNastere(an), arePremii(false), nrPremii(0), nrScrieri(nr_scrieri) {}


// operator <<
std::ostream& operator<<(std::ostream& out, const Autor& autor) {
    out << (autor.nume.empty() ? "Anonim": autor.nume)
        << ", nascut in " << autor.anNastere
        << ", a scris in total " << autor.nrScrieri
        << " opere, pentru care a primit "
        << autor.nrPremii << " premii.\n";
    return out;
}

// operator >>
std::istream& operator>>(std::istream& in, Autor& autor) {
    std::cout << "Introduceti date despre autor\nNume: ";

    in.ignore();
    getline(in, autor.nume);

    std::cout << "Anul nasterii: ";
    in >> autor.anNastere;

    std::cout << "Are premii (1/0): ";
    in >> autor.arePremii;

    std::cout << "Numar premii: ";
    in >> autor.nrPremii;

    std::cout << "Numar opere: ";
    in >> autor.nrScrieri;

    return in;
}

// operator []
char& Autor::operator[](int index) {
    assert(index >= 0 && index < (int)nume.length());
    return nume[index];
}

// ++ prefix (nrScrieri)
Autor& Autor::operator++() {
    ++nrScrieri;
    return *this;
}

// -- prefix
Autor& Autor::operator--() {
    --nrScrieri;
    return *this;
}

// ++ postfix (nrPremii)
Autor Autor::operator++(int) {
    Autor copie = *this;
    nrPremii++;
    if (nrPremii > 0) arePremii = true;
    return copie;
}

// -- postfix
Autor Autor::operator--(int) {
    Autor copie = *this;
    nrPremii--;
    if (nrPremii <= 0) arePremii = false;
    return copie;
}

// operator +
int Autor::operator+(const Autor& autor) const {
    return nrScrieri + autor.nrScrieri;
}

// operator -
int Autor::operator-(const Autor& autor) const {
    return abs(anNastere - autor.anNastere);
}

// operator ==
bool Autor::operator==(const Autor& autor) const {
    if (anNastere != autor.anNastere) return false;
    if (arePremii != autor.arePremii) return false;
    if (nrPremii != autor.nrPremii) return false;
    if (nrScrieri != autor.nrScrieri) return false;
    return nume==autor.nume;
}

// comparatii
bool Autor::operator<(const Autor& autor) const {
    return nrScrieri < autor.nrScrieri;
}

bool Autor::operator<=(const Autor& autor) const {
    return nrScrieri <= autor.nrScrieri;
}

bool Autor::operator>(const Autor& autor) const {
    return nrScrieri > autor.nrScrieri;
}

bool Autor::operator>=(const Autor& autor) const {
    return nrScrieri >= autor.nrScrieri;
}

// getteri
const std::string& Autor::getNume() const {
    return nume;
}

int Autor::getAnNastere() const {
    return anNastere;
}

bool Autor::getArePremii() const {
    return arePremii;
}

int Autor::getNrPremii() const {
    return nrPremii;
}

int Autor::getNrScrieri() const {
    return nrScrieri;
}

// setteri
void Autor::setNume(const std::string& n) {
    nume=n;
}

void Autor::setAn(const int an) {
    if (an < 0 || an > 2026)
        throw ValidationError("anNastere", (float)an);
    anNastere = an;
}

void Autor::setPremii(const int nr) {
    if (nr < 0) throw ValidationError("nrPremii", (float)nr);
    nrPremii = nr;
    arePremii = (nr > 0);
}

void Autor::setScrieri(const int nr) {
    if (nr < 0) throw ValidationError("nrScrieri", (float)nr);
    nrScrieri = nr;
}