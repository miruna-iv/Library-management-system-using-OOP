#ifndef EXCEPTII_H
#define EXCEPTII_H
 
#include <stdexcept>
#include <string>
#include <iostream>

class LibrarieError: public std::runtime_error{
public:
    explicit LibrarieError(const std::string& msj) : std::runtime_error(msj){};
    ~LibrarieError() noexcept override = default;

};

class ValidationError: public LibrarieError{
    std::string camp;
    float val; //valoarea care a cauzat eroarea
    static std::string mesajEroare(const std::string camp, float val){
        return "(Eroare validare) Campul " + camp + " are o valoare invalida: " + std::to_string(val);
    }
public:
    ValidationError(const std::string camp, float val): LibrarieError(mesajEroare(camp, val)), camp(camp), val(val){};
    ~ValidationError() noexcept override = default;
    const char* what() const noexcept override {
        return std::runtime_error::what();
    }
    const std::string& getCamp(){return camp;};
    float getVal(){return val;};
};

class NotFoundError: public LibrarieError{
    int idCautat;
    std::string locatie; //poate fi stoc / clienti / angajati etc
    static std::string mesajEroare(const std::string locatie, int idCautat){
        return "(Eroare cautare) ID-ul " + std::to_string(idCautat) + " nu a fost gasit in lista de: " + locatie;
    }

public:
    NotFoundError(int id, const std::string& loc): LibrarieError(mesajEroare(loc, id)), idCautat(id), locatie(loc){};
    ~NotFoundError() noexcept override = default;
    const char* what() const noexcept override {
        return std::runtime_error::what();
    }
    const std::string& getLocatie(){return locatie;};
    int getId(){return idCautat;};

};

class InsufficientBudgetError: public LibrarieError{
    float bugetDisponibil, pretCerut;
    static std::string mesajEroare(int buget, int pret){
        return "(Fonduri insuficiente) Clientul a incercat sa faca o achizitie in valoare de " + std::to_string(pret) + " ,dar are un buget disponibil de: " + std::to_string(buget) 
        + "\nDiferenta este de: " +std::to_string(pret-buget) ;
    }

public:
    InsufficientBudgetError(float pret, float buget): LibrarieError(mesajEroare(buget, pret)), bugetDisponibil(buget), pretCerut(pret){};
    ~InsufficientBudgetError() noexcept override = default;
    const char* what() const noexcept override {
        return std::runtime_error::what();
    }
    float getBuget(){return bugetDisponibil;};
    float getPret(){return pretCerut;};
    float getDiferenta(){return pretCerut - bugetDisponibil;}

};


//pt demonstrarea stack unwinding
class ResourceGuard {
    std::string nume;
public:
    explicit ResourceGuard(const std::string& n) : nume(n) {
        std::cout << "[StackUnwinding] ResourceGuard '" << nume << "' creat\n";
    }
    // destructorul este noexcept implicit — apelat automat la propagarea exceptiei
    ~ResourceGuard() noexcept {
        std::cout << "[StackUnwinding] ResourceGuard '" << nume<< "' distrus\n";
    }
};



#endif