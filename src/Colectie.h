#ifndef COLECTIE_H
#define COLECTIE_H
 
#include <iostream>
#include <string>
#include <stdexcept>
#include "Exceptii.h"

// Clasa template Colectie<T, N>
// T = tipul elemlor
// N = dimensiunea fixa a colectiei


template<typename T, int N>
class Colectie{
    T elem[N];
    int nrElem;
public:
    Colectie(): nrElem(0){};

    void adauga(const T& e){
        if(nrElem >= N){
            throw std::overflow_error("Colectia este plina. Capacitate maxima: "+ std::to_string(N));
        }
        elem[nrElem++]=e;
    }
    void elimina(){
        if(nrElem==0){
            throw std::underflow_error("Colectia este goala. Nu se poate elimina nimic momentan");
        }
        nrElem--;
    }
    T& get(int i) {
        if (i < 0 || i >= nrElem)
            throw std::out_of_range("Index invalid: " + std::to_string(i) +" (dimensiune curenta: " + std::to_string(nrElem) + ")");
        return elem[i];
    }
    const T& get(int i) const {
        if (i < 0 || i >= nrElem)
            throw std::out_of_range("Index invalid: " + std::to_string(i) + " (dimensiune curenta: " + std::to_string(nrElem) + ")");
        return elem[i];
    }

    int size() const { return nrElem; }
    int capacity() const { return N; }
    bool empty() const { return nrElem == 0; }
    bool full() const { return nrElem == N; }
    void clear() { nrElem = 0; }


    //metoda template in interiorul clasei template
    template<typename U, typename Convertor>
    Colectie<U, N> transforma(Convertor fn) const {
        Colectie<U, N> rezultat;
        for (int i = 0; i < nrElem; i++)
            rezultat.adauga(fn(elem[i]));
        return rezultat;
    }

    T* begin() { return elem; }
    T* end()   { return elem + nrElem; }
    const T* begin() const { return elem; }
    const T* end()   const { return elem + nrElem; }
 
    friend std::ostream& operator<<(std::ostream& out, const Colectie<T,N>& c) {
        out << "Colectie [" << c.nrElem << "/" << N << "]:\n";
        for (int i = 0; i < c.nrElem; i++)
            out << "  " << i << ". " << c.elem[i] << "\n";
        return out;
    }
};


//functii template independente

// cautare generica — returneaza indexul primului element care satisface predicatul, sau -1 daca nu exista
template<typename T, int N, typename Predicat>
int cautaIn(const Colectie<T, N>& col, Predicat pred) {
    for (int i = 0; i < col.size(); i++)
        if (pred(col.get(i))) return i;
    return -1;
}

//sortare generala (cu bubble sort)
template<typename T, int N, typename Comparator>
void sorteazaColectie(Colectie<T, N>& col, Comparator cmp) {
    for (int i = 0; i < col.size() - 1; i++)
        for (int j = i + 1; j < col.size(); j++)
            if (!cmp(col.get(i), col.get(j))) {
                T temp = col.get(i);
                col.get(i) = col.get(j);
                col.get(j) = temp;
            }
}

//specializare totala pentru Colectie<std::string, N>
#include <algorithm>
#include <string>

template<int N>
int cautaIn(const Colectie<std::string, N>& col, const std::string& titlu) {
    std::string titluLower = titlu;
    std::transform(titluLower.begin(), titluLower.end(), titluLower.begin(), 
        [](char c) {
            if (c >= 'A' && c <= 'Z') {
                return static_cast<char>(c + ('a' - 'A'));
            }
            return c;
        });

    for (int i = 0; i < col.size(); i++) {
        std::string elemLower = col.get(i);
        std::transform(elemLower.begin(), elemLower.end(), elemLower.begin(), 
            [](char c) {
                if (c >= 'A' && c <= 'Z') {
                    return static_cast<char>(c + ('a' - 'A'));
                }
                return c;
            });
            
        if (elemLower == titluLower) return i;
    }
    return -1;
}

#endif