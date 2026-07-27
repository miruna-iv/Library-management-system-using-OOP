#ifndef IOBJECT_H
#define IOBJECT_H

#include <iostream>
#include <string>

class IObject {
    static int contor; // partajat de toate instantele
    const int id; 

protected:
    //constructorul este protected: nu se instantiaza direct, numai prin derivate
    IObject() : id(++contor) {}

    //la copiere, alocam un id nou
    IObject(const IObject&) : id(++contor) {}

public:
    virtual ~IObject() {}

    int getId() const noexcept { return id; }

    virtual std::string toString() const = 0;

    friend std::ostream& operator<<(std::ostream& out, const IObject& obj) {
        return out << obj.toString();
    }

    bool operator==(const IObject& alt) const noexcept {
        return id == alt.id;
    }
};

#endif