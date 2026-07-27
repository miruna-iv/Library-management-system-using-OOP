#ifndef AUTOR_H
#define AUTOR_H

#include <iostream>
#include <string>
#include <cassert>

class Autor {
private:
    std::string nume;
    int anNastere;
    bool arePremii;
    int nrPremii;
    int nrScrieri;

public:
    // constructori
    Autor();
    //Autor(const Autor& autor);
    Autor(const std::string& n, int an, bool premii, int nr_premii, int nr_scrieri);
    Autor(const std::string& n, int an);
    Autor(const std::string& n, int an, int nr_scrieri);

    // destructor
    //~Autor();

    // operator =
    Autor& operator=(const Autor& autor);

    // operatori de input/output
    friend std::ostream& operator<<(std::ostream& out, const Autor& autor);
    friend std::istream& operator>>(std::istream& in, Autor& autor);

    // operator indexare
    char& operator[](int index);

    // operatori ++ si --
    Autor& operator++();       // prefix
    Autor& operator--();       // prefix
    Autor operator++(int);     // postfix
    Autor operator--(int);     // postfix

    // operatori + si -
    int operator+(const Autor &autor) const;
    int operator-(const Autor &autor) const;

    // operator ==
    bool operator==(const Autor& autor) const;

    // operatori de comparare
    bool operator<(const Autor& autor) const;
    bool operator<=(const Autor& autor) const;
    bool operator>(const Autor& autor) const;
    bool operator>=(const Autor& autor) const;

    // getteri
    const std::string& getNume() const;
    int getAnNastere() const;
    bool getArePremii() const;
    int getNrPremii() const;
    int getNrScrieri() const;

    // setteri
    void setNume(const std::string& n);
    void setAn(const int an);
    void setPremii(const int nr);
    void setScrieri(const int nr);
};

#endif