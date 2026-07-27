#ifndef ABSTRACT_FACTORY_H
#define ABSTRACT_FACTORY_H

#include <string>
#include <memory>
#include "Carte.h"
#include "Cititor.h"

// Produs abstract 1: Carte
class ICarteFactory {
public:
    virtual Carte creeaza(const std::string& titlu, float pret, Autor autor) const = 0;
    virtual std::string descrieAbonament() const = 0;
    virtual ~ICarteFactory() = default;
};

// Produs abstract 2: Cititor
class ICititorFactory {
public:
    virtual Cititor creeaza(const std::string& nume, char gen, float buget) const = 0;
    virtual std::string descrieAbonament() const = 0;
    virtual ~ICititorFactory() = default;
};

// FAMILIA 1: subscription basic
// Carte la pret intreg, Cititor fara card fidelitate

class CarteBasicFactory : public ICarteFactory {
public:
    // pret intreg, fara nicio reducere aplicata
    Carte creeaza(const std::string& titlu, float pret, Autor autor) const override {
        return Carte(titlu, autor, NECUNOSCUT);
    }
    std::string descrieAbonament() const override {
        return "Basic (pret intreg, fara reduceri)";
    }
};

class CititorBasicFactory : public ICititorFactory {
public:
    // fara card fidelitate, buget exact cat introduce userul
    Cititor creeaza(const std::string& nume, char gen, float buget) const override {
        return Cititor(nume, 0, buget, gen, false, {});
    }
    std::string descrieAbonament() const override {
        return "Basic (fara card fidelitate, fara reduceri)";
    }
};

// ─────────────────────────────────────────────────────────────────
// FAMILIA 2: subscription vip
// Carte cu reducere 20% aplicata automat, Cititor cu card fidelitate
// si buget bonus de 100 RON
// ─────────────────────────────────────────────────────────────────

class CarteVIPFactory : public ICarteFactory {
public:
    // reducere 20% aplicata automat la creare
    Carte creeaza(const std::string& titlu, float pret, Autor autor) const override {
        Carte c(titlu, autor, NECUNOSCUT);
        c.setPret(pret * 0.8f);
        return c;
    }
    std::string descrieAbonament() const override {
        return "VIP (reducere 20% la carti)";
    }
};

class CititorVIPFactory : public ICititorFactory {
public:
    // card fidelitate inclus automat + 100 RON bonus la buget
    Cititor creeaza(const std::string& nume, char gen, float buget) const override {
        return Cititor(nume, 0, buget + 100.0f, gen, true, {});
    }
    std::string descrieAbonament() const override {
        return "VIP (card fidelitate inclus, +100 RON bonus)";
    }
};

// FABRICA ABSTRACTA
class IAbonamentFactory {
public:
    virtual ICarteFactory* getCarteFactory() const = 0;
    virtual ICititorFactory* getCititorFactory() const = 0;
    virtual std::string numeAbonament() const = 0;
    virtual ~IAbonamentFactory() = default;
};

// FABRICA CONCRETA 1: Basic
class AbonamentBasicFactory : public IAbonamentFactory {
    CarteBasicFactory carteFab;
    CititorBasicFactory cititorFab;
public:
    ICarteFactory* getCarteFactory() const override { return const_cast<CarteBasicFactory*>(&carteFab); }
    ICititorFactory* getCititorFactory() const override { return const_cast<CititorBasicFactory*>(&cititorFab); }
    std::string numeAbonament() const override { return "Basic"; }
};

// FABRICA CONCRETA 2: VIP
class AbonamentVIPFactory : public IAbonamentFactory {
    CarteVIPFactory carteFab;
    CititorVIPFactory cititorFab;
public:
    ICarteFactory* getCarteFactory() const override { return const_cast<CarteVIPFactory*>(&carteFab); }
    ICititorFactory* getCititorFactory() const override { return const_cast<CititorVIPFactory*>(&cititorFab); }
    std::string numeAbonament() const override { return "VIP"; }
};

#endif