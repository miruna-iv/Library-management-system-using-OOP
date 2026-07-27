#ifndef LIBRARIE_H
#define LIBRARIE_H

#include <iostream>
#include <cstring>
#include <cassert>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

#include "Carte.h"
#include "Cititor.h"
#include "Angajat.h"
#include "AngajatCititor.h"
#include "Persoana.h"
#include "Manager.h"

class Librarie{
    const int idLibrarie;
    std::string denumire;
    std::string adresa;
    std::vector<Carte> stoc;
    std::vector<Cititor> clienti;
    std::vector<Angajat> angajati;
    static int nrLibrariiDeschise;

    //pentru polimorfism
     std::vector<Persoana*> persoane;
     //pt cautarea unei carti in O(logn)
     std::map<std::string, int> indexTitlu;

    public:
        // constructori
        Librarie();
        Librarie(const Librarie& l);
        Librarie(const std::string& d, const std::string& a, const std::vector<Carte>& s, const std::vector<Cititor>& c, const std::vector<Angajat>& angajati, const std::vector<Persoana*> p);
        Librarie(const std::string& d, const std::string& a);
        Librarie(const std::string& d, const std::string& a, const std::vector<Carte>& s);

        // destructor
        ~Librarie();

        void adaugaPersoana(Persoana* p);
        void afiseazaPersoane() const;
        // operator =
        Librarie& operator=(const Librarie& l);

        // I/O
        friend std::ostream& operator<<(std::ostream& out, const Librarie& l);
        friend std::istream& operator>>(std::istream& in, Librarie& l);

        // []
        Carte& operator[](int index);

        // ++ / --
        Librarie& operator++();
        Librarie& operator--();

        // aritmetice
        Librarie operator+(const Carte& c) const;
        Librarie operator-(float reducere) const;

        friend Librarie operator+(Carte c, const Librarie& l);

        // relationali
        bool operator==(const Librarie& l) const;
        bool operator<(const Librarie& l) const;
        bool operator>(const Librarie& l) const;
        bool operator<=(const Librarie& l) const;
        bool operator>=(const Librarie& l) const;


        //algotitmi stl
        void sorteazaAngajatiDupaSalariu();
        void sorteazaCartiDupaPret();
        std::vector<Carte> filtreazaCartiDupaPret(float pretMax) const;
        std::vector<Cititor> clientiCuCard() const;
        int  nrClientiCuBugetPeste(float limita) const;
        Carte* cautaCarte(const std::string& titlu);
        void stergeCartiIeftine(float pretMin); 

        // getteri
        const std::string& getDenumire() const;
        const std::string& getAdresa() const;
        int getIdLibrarie() const;
        static int getNrLibrariiDeschise();
        std::vector<Carte> getStoc() const;
        std::vector<Cititor> getClienti() const;
        std::vector<Angajat> getAngajati() const;
        std::vector<Persoana*> getPersoane() const;
        Cititor& getClient(int index);
        Angajat& getAngajat(int index);
        Carte&   getCarte(int index);

        // setteri
        void setDenumire(const std::string& c);
        void setAdresa(const std::string& c);
        void adaugaCarte(const Carte& c);
        void adaugaClient(const Cititor& c);
        void adaugaAngajat(const Angajat& a);
        void setStoc(const std::vector<Carte>& v);
        void setClienti(const std::vector<Cititor>& v);
        void setAngajati(const std::vector<Angajat>& v);
        void setPersoane(const std::vector<Persoana*>& v);
        void stergeClient(int index);
        void stergeAngajat(int index);
        void stergeCarte(int index);
};

#endif