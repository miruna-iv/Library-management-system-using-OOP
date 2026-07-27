#ifndef PERSOANA_H
#define PERSOANA_H

#include <iostream>
#include <cstring>
#include <cassert>
#include <fstream>
#include <string>
#include "IObject.h"

extern std::ofstream fout; 

class Persoana: public IObject {
protected:
//campuri protected pentru a fi accesibile in clasele derivate, dar nu din exterior
    std::string nume;
    int varsta;
    char gen; // m, f sau n
    const int idPersoana;
    static int nrTotalPersoane;

public:
    Persoana();
    Persoana(const std::string& nume, int varsta, char g);
    Persoana(const Persoana& p);
    virtual ~Persoana(); //asigura apelul destructorului derivat la delete prin Persoana*

    Persoana& operator=(const Persoana& p);

    //persoana are metode virtuale pure => este clasa abstracta => nu poate fi instantiata
    virtual void afisare() const = 0;
    virtual bool validare() const = 0;
    virtual Persoana* clone() const = 0;

    friend std::ostream& operator<<(std::ostream& out, const Persoana& p);
    friend std::istream& operator>>(std::istream& in, Persoana& p);

    const std::string& getNume() const;
    int getVarsta() const;
    char getGen() const;
    int getId() const;
    static int getNrTotalPersoane();

    void setNume(const std::string& n);
    void setVarsta(int v);
    void setGen(char c);
};

#endif
