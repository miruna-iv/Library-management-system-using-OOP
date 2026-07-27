#ifndef MANAGER_H
#define MANAGER_H
#include "Angajat.h"

class Manager : public Angajat {
private:
    //al treilea nivel al ierarhiei
    // private: accesibil doar prin getteri/setteri, nu direct din exterior
    //deoarece nu avem clase care sa mosteneasca Manager si sa aiba neoie de acces la campuri
    float bonus;
    int nrSubalterni;

public:
    // Constructori
    Manager();
    Manager(const Manager& m);
    Manager(const std::string& n, int v, char g, float sal, int anAng, Departament dep, float bonus, int nrSub);
    ~Manager() override ;

    // suprascrie metodele pure virtuale din Persoana
    // apeleaza Angajat::afisare() pentru a extinde comportamentul
    void afisare() const override;
    bool validare() const override;
    Persoana* clone() const override;
    std::string toString() const override;

    Manager& operator=(const Manager& m);

    friend std::ostream& operator<<(std::ostream& out, const Manager& m);
    friend std::istream& operator>>(std::istream& in, Manager& m);

    // Getteri si setteri
    float getBonus() const;
    int getNrSubalterni() const;
    void setBonus(float b);
    void setNrSubalterni(int n);
};

#endif