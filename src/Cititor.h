#ifndef CITITOR_H
#define CITITOR_H

#include <iostream>
#include <cstring>
#include <cassert>
#include <string>
#include <vector>
#include "Persoana.h"

//Cititor mosteneste virtual persoana pentru a rezolva ambiguitatea in cazul mostenirii in diamant
class Cititor: public virtual Persoana{
    //parametri private by default, accesibili doar din interiorul clasei, deoarece nicio clasa derivata nu are nevoie sa acceseze direct campurile
    const int idCititor;
    float buget;
    std::vector<int> idCartiCumparate;
    bool areCardFidelitate;

    static int nrTotalCititori;

public:
    // constructori
    Cititor();
    Cititor(const Cititor& c);
    Cititor(const std::string& n, int v, float b, char g, bool cf, const std::vector<int>& c);
    Cititor(float b);
    Cititor(const std::string& n, char g);

    // destructor
    ~Cititor() override;

    //metode virtuale
    void afisare() const override;
    bool validare() const override;
    Persoana* clone() const override;
    std::string toString() const override;

    // operator =
    Cititor& operator=(const Cititor& c);

    // operatori I/O
    friend std::istream& operator>>(std::istream& in, Cititor& c);
    friend std::ostream& operator<<(std::ostream& out, const Cititor& c);

    // operator []
    int operator[](int index);

    // operatori ++ / --
    Cititor& operator++();
    Cititor& operator--();

    // operatori + / -
    Cititor operator+(float s) const;
    Cititor operator-(float s) const;
    Cititor operator+(int id) const;

    friend Cititor operator+(float suma, const Cititor& c);

    // operatori relationali
    bool operator==(const Cititor& c) const;
    bool operator>(const Cititor& c) const;
    bool operator<(const Cititor& c) const;
    bool operator>=(const Cititor& c) const;
    bool operator<=(const Cititor& c) const;

    // getteri
    const std::string& getNume() const;
    float getBuget() const;
    std::vector<int> getCartiCumparate() const;
    char getGen() const;
    bool getStatus() const;
    int getId() const;

    // setteri
    void setBuget(float b);
    void setNrCartiCumparate(int n);
    void setCartiCumparate(const std::vector<int>& c);
    void setStatus(bool status);
};

#endif