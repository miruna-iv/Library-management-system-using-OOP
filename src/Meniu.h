#ifndef MENIU_H
#define MENIU_H

#include <iostream>
#include <cstring>
#include <cassert>
#include <string>
#include "Librarie.h"
#include "AbstractFactory.h"

class Meniu{
    private:
        Librarie& librarie;
        void curatareBuffer();
        void afiseazaMeniuPrincipal();
        IAbonamentFactory* factory;
    public:
        // constructor cu dependency injection
        Meniu(Librarie& lib, IAbonamentFactory* f);
        void executaVanzare();
        void porneste();
        void meniuClienti();
        void meniuCarti();
        void meniuAngajati();
    };

#endif