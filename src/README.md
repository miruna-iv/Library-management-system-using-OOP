# Proiect nr. 3 POO

**Ivan Miruna Maria, grupa 141**

---

## Descrierea aplicației

Aplicatia simuleaza managementul unei librarii. Permite gestionarea:

- Cartilor din stoc (adaugare, vizualizare, rating, reduceri, sortare, filtrare)
- Clientilor (cititori) cu istoric de cumparaturi, card de fidelitate si tipuri de abonament
- Angajatilor simpli cu salariu si departament
- Angajatilor-Cititori (atat angajati, cat si clienti ai librariei)
- Managerilor (angajati cu bonus si subalterni)

Datele sunt persistate intr-o baza de date SQLite (librarie.db).
Constructorii si destructorii scriu in logs.txt pentru a demonstra
ordinea de constructie/distrugere in ierarhia de clase.

---

## Ierarhia de clase

```
IObject
└── Persoana (abstracta)
    ├── Angajat (virtual public Persoana)
    │   └── Manager
    └── Cititor (virtual public Persoana)
        └── AngajatCititor : public Angajat, public Cititor
                (mostenire in diamant cu virtual public)
```

Am ales sa implementez mostenirea multipla in cazul AngajatCititor pentru a gestiona
eventualitatea in care un angajat este si client. Fara virtual public, AngajatCititor
ar fi avut doua copii ale clasei Persoana in memorie. Cu virtual public Persoana in
Angajat si Cititor, compilatorul garanteaza o singura instanta a lui Persoana, partajata.

---

## Concepte adaugate

### 1. Ierarhie de exceptii custom

Trei clase de exceptii care mostenesc din `std::runtime_error`:

- `LibrarieError` — baza ierarhiei
- `ValidationError` — date invalide in setteri (camp + valoarea invalida)
- `NotFoundError` — element negasit (index/id + context)
- `InsufficientBudgetError` — fonduri insuficiente (buget disponibil + pret cerut)

Stack unwinding demonstrat prin `ResourceGuard` in `executaVanzare()`.
Toti setterii arunca exceptii in loc sa ignore silentios valorile invalide.
Fiecare `case` din meniu are `try/catch`.

### 2. Containere STL

Inlocuit toate array-urile alocate dinamic cu containere STL:

| Colectie              | Container           | Justificare                              |
| --------------------- | ------------------- | ---------------------------------------- |
| `Carte* stoc`         | `vector<Carte>`     | acces O(1) la index, iterare frecventa   |
| `Cititor* clienti`    | `vector<Cititor>`   | acces O(1), sortare usoara               |
| `Angajat* angajati`   | `vector<Angajat>`   | acces O(1)                               |
| `Persoana** persoane` | `vector<Persoana*>` | polimorfism, delete manual in destructor |
| index titlu→pozitie   | `map<string,int>`   | cautare O(log n) dupa titlu              |

**Algoritmi STL cu lambda:**

1. `for_each` — afisare colectii
2. `sort` — angajati dupa salariu, carti dupa pret
3. `copy_if` — filtrare clienti cu card, carti dupa pret maxim
4. `count_if` — clienti cu buget peste limita
5. `find_if` — cautare carte dupa titlu
6. `erase-remove_if` — stergere carti ieftine
7. `transform` — conversie `Colectie<Carte,5>` → `Colectie<string,5>`

### 3. Clase si functii template

- **`Colectie<T, N>`** — clasa template cu parametru non-tip `N` (dimensiune fixa la compilare, pe stiva)
- **Metoda template in clasa**: `transforma<U>(fn)` — converteste `Colectie<T,N>` in `Colectie<U,N>`
- **Functii template libere**: `cautaIn<T,N,Predicat>` si `sorteazaColectie<T,N,Comparator>`
- **Specializare totala**: `cautaIn<int N>(Colectie<string,N>, string)` — cautare case-insensitive

### 4. Migrare la std::string

Eliminat tot codul manual de gestionare a memoriei (`new char[]`, `strcpy`, `delete[]`).
Constructorii de copiere, `operator=` si destructorii pentru siruri nu mai sunt necesari (Rule of Zero).

### 5. Interfata IObject

Implementata exact ca in Laboratorul 7:

- Contor static pentru id-uri unice
- `getId() const noexcept`
- `toString() const` — pur virtuala, implementata in fiecare clasa concreta
- `operator<<` (friend) apelat polimorfic prin `toString()`
- `operator==` pe baza de id

### 6. Singleton — ConfiguratieLibrarie

Implementat in varianta Meyers (`static` local in `getInstance()`).
Stocheaza setarile globale: TVA, reducere maxima, salariu minim, moneda.
Integrat in:

- `Librarie::operator-` — verifica ca reducerea nu depaseste `reducereMaxima`
- `Angajat::setSalariu` — verifica ca salariul respecta `salariuMinim`
- `Meniu` case 5 — optiune de vizualizare si modificare configuratie

### 7. Abstract Factory — AbonamentFactory

Doua familii de produse, doua tipuri de produse per familie:

```
IAbonamentFactory
├── AbonamentBasicFactory
│   ├── CarteBasicFactory  → Carte la pret intreg
│   └── CititorBasicFactory → Cititor fara card fidelitate
└── AbonamentVIPFactory
    ├── CarteVIPFactory    → Carte cu reducere 20%
    └── CititorVIPFactory  → Cititor cu card fidelitate + 100 RON bonus
```

Clientul (`Meniu`) lucreaza exclusiv cu interfetele abstracte.
Schimbarea familiei se face prin inlocuirea fabricii (case 8 din meniu clienti).

---

## Instructiuni de compilare

**Cerinte:** g++ cu suport C++17, biblioteca sqlite3 instalata.

**Comanda de compilare:**

```bash
g++ -std=c++17 -Wall -Wextra -o main \
    main.cpp Autor.cpp Carte.cpp Cititor.cpp \
    Librarie.cpp Persoana.cpp Angajat.cpp \
    AngajatCititor.cpp Meniu.cpp Manager.cpp \
    -lsqlite3
```

**Rulare:**

```bash
./main
```

**Note:**

- La prima rulare se creeaza automat baza de date `librarie.db`
- Fisierul `logs.txt` contine ordinea de constructie/distrugere a obiectelor
- Nu se folosesc containere STL cu `new[]` — toate colectiile sunt gestionate cu `vector`, `map` etc.
