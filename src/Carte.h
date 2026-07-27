#ifndef CARTE_H
#define CARTE_H

#include <iostream>
#include <cstring>
#include <cassert>
#include <string>
#include <vector>
#include "Autor.h"

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

std::string categorieToString(Categorie c);

class Carte{
    std::string titlu;
    int anPublicare;
    int nrPag;
    const int id;
    static int nrCartiInventar;
    std::vector<float> rating;
    Categorie categorie;
    float pret;
    Autor scriitor;

    public:
        //constructori
        Carte();
        //Carte(const Carte& carte);
        Carte(const std::string& t, int an, int pag, Categorie cat, float p, const std::vector<float>& r, Autor a);
        Carte(const std::string& t, Autor a, Categorie c);
        Carte(const std::string& t, float p);
        //destructor
        //~Carte();

        //supraincarcarea operatorului =
        Carte& operator=(const Carte& carte);

        //supraincarcarea operatorilor <<, respectiv >>
        friend std::ostream& operator<<(std::ostream& out, const Carte& carte);
        friend std::istream& operator>>(std::istream& in, Carte& carte);

        //supraincarcarea operatorului de indexare
        float& operator[](int index);

        //supraincarcarea operatorilor ++ si -- prefixat: crestem / scadem pretul cu verificarea sa nu fie <0
        Carte& operator++();

        Carte& operator--();

        //supraincarcarea operatorilor matematici
        Carte operator+(float r) const;
        float operator*(int n) const;
        float operator+(const Carte& c) const;

        //supraincarcarea operatorilor relationali
        bool operator==(const Carte& c) const;
        bool operator<(const Carte& c) const;
        bool operator<=(const Carte& c) const;
        bool operator>(const Carte& c) const;
        bool operator>=(const Carte& c) const;

        //getteri, setteri

        const std::string& getTitlu() const;
        int getAnPublicare() const;
        int getNrPag() const;
        int getId() const;
        float* getRating() const;
        Categorie getCategorie() const;
        float getPret() const;
        Autor getScriitor() const;
        int getNrRatinguri() const;

        void setTitlu(const std::string& t);
        void setAnPublicare(int an);
        void setNrPag(int n);
        void setPret(float n);
        void setCategorie(Categorie c);
        void setRatinguri(const std::vector<float>& r);
        void setScriitor(Autor a);

        static int getNrCartiInventar();
};

float operator*(int cantitate, const Carte& c);

#endif