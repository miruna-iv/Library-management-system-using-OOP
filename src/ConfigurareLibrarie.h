#ifndef CONFIGURATIE_LIBRARIE_H
#define CONFIGURATIE_LIBRARIE_H

#include <string>
#include <iostream>
#include "Exceptii.h"

// Singleton ConfiguratieLibrarie — varianta Meyers
// Stocheaza setarile globale ale aplicatiei (TVA, reducere maxima, moneda)
// O singura instanta in tot programul — orice clasa care are nevoie de configuratie o acceseaza prin getInstance().


class ConfiguratieLibrarie {
    float tva; // procent TVA aplicat preturilor
    float reducereMaxima; // reducerea maxima permisa (%)
    float salariu_minim; // salariul minim acceptat pentru angajati
    std::string moneda; // moneda folosita in afisari

    // constructor privat — nimeni nu poate crea instante direct
    ConfiguratieLibrarie()
        : tva(19.0f), reducereMaxima(50.0f),
          salariu_minim(2000.0f), moneda("RON") {}

public:
    static ConfiguratieLibrarie& getInstance() {
        static ConfiguratieLibrarie instance;
        return instance;
    }

    // dezactivate explicit cu = delete
    ConfiguratieLibrarie(const ConfiguratieLibrarie&) = delete;
    ConfiguratieLibrarie& operator=(const ConfiguratieLibrarie&) = delete;

    //getteri
    float getTVA() const { return tva; }
    float getReducereMaxima() const { return reducereMaxima; }
    float getSalariuMinim() const { return salariu_minim; }
    const std::string& getMoneda() const { return moneda; }

    //setteri
    void setTVA(float t) {
        if(t < 0 || t > 100) throw ValidationError("TVA", t);
        tva = t;
    }
    void setReducereMaxima(float r) {
        if(r < 0 || r > 100) throw ValidationError("reducereMaxima", r);
        reducereMaxima = r;
    }
    void setSalariuMinim(float s) {
        if(s < 0) throw ValidationError("salariuMinim", s);
        salariu_minim = s;
    }
    void setMoneda(const std::string& m) {
        if(m.empty()) throw ValidationError("moneda", 0);
        moneda = m;
    }

    //afisarea configurarii curente
    void afiseaza() const {
        std::cout << "------- Configuratie Librarie -------\n";
        std::cout << "TVA: " << tva << "%\n";
        std::cout << "Reducere maxima: " << reducereMaxima << "%\n";
        std::cout << "Salariu minim: " << salariu_minim << " " << moneda << "\n";
        std::cout << "Moneda: " << moneda << "\n";
        std::cout << "--------------------------------------\n";
    }

    // calcularea pretului cu TVA
    float aplicaTVA(float pret) const {
        return pret * (1.0f + tva / 100.0f);
    }

    // verificare reducere
    bool reducerePermisa(float procent) const {
        return procent >= 0 && procent <= reducereMaxima;
    }

    // verificare salariu
    bool salariuValid(float salariu) const {
        return salariu >= salariu_minim;
    }
};

// macro pentru acces rapid
#define CONFIG ConfiguratieLibrarie::getInstance()

#endif

// Demonstrare ca nu se pot crea doua instante:
// ConfiguratieLibrarie c2; // EROARE — constructor privat
// ConfiguratieLibrarie c3 = ConfiguratieLibrarie::getInstance(); // EROARE — copy deleted