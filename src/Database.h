#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <iostream>
#include <string>
#include <vector>
#include "Carte.h"
#include "Cititor.h"
#include "Angajat.h"
#include "AngajatCititor.h"
#include "Manager.h"
#include "Librarie.h"
using namespace std;

class Database {
private:
    sqlite3* db;

    bool executeSQL(const char* sql) {
        char* errMsg = nullptr;
        int rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
        if (rc != SQLITE_OK) {
            cout << "[DB] Eroare SQL: " << errMsg << endl;
            sqlite3_free(errMsg);
            return false;
        }
        return true;
    }

    // CARTI
    void salveazaCarte(const Carte& c, int idLibrarie) {
        std::string titlu     = c.getTitlu();
        Autor sc              = c.getScriitor();
        std::string autorNume = sc.getNume();

        char sql[1024];
        snprintf(sql, sizeof(sql),
            "INSERT INTO carti (idLibrarie, titlu, anPublicare, nrPag, categorie, pret,"
            " autorNume, autorAn, autorNrPremii, autorNrScrieri)"
            " VALUES (%d, '%s', %d, %d, %d, %.2f, '%s', %d, %d, %d);",
            idLibrarie,
            titlu.empty()     ? "Necunoscut" : titlu.c_str(),
            c.getAnPublicare(), c.getNrPag(), (int)c.getCategorie(), c.getPret(),
            autorNume.empty() ? "Anonim"     : autorNume.c_str(),
            sc.getAnNastere(), sc.getNrPremii(), sc.getNrScrieri()
        );

        if (!executeSQL(sql)) return;

        long long idCarte = sqlite3_last_insert_rowid(db);
        // getNrRatinguri() + getRating() pentru compatibilitate DB
        int nr           = c.getNrRatinguri();
        float* ratinguri = c.getRating();
        for (int i = 0; i < nr; i++) {
            char sqlR[256];
            snprintf(sqlR, sizeof(sqlR),
                "INSERT INTO ratinguri (idCarte, valoare) VALUES (%lld, %.2f);",
                idCarte, ratinguri[i]);
            executeSQL(sqlR);
        }
        if (ratinguri) delete[] ratinguri;
    }

    Carte* incarcaCartiLibrarie(int idLibrarie, int& nrCarti) {
        nrCarti = 0;
        sqlite3_stmt* stmt;
        char sql[256];

        snprintf(sql, sizeof(sql),
            "SELECT COUNT(*) FROM carti WHERE idLibrarie = %d;", idLibrarie);
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) return nullptr;
        if (sqlite3_step(stmt) == SQLITE_ROW) nrCarti = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);
        if (nrCarti == 0) return nullptr;

        Carte* carti = new Carte[nrCarti];
        snprintf(sql, sizeof(sql),
            "SELECT id, titlu, anPublicare, nrPag, categorie, pret,"
            " autorNume, autorAn, autorNrPremii, autorNrScrieri"
            " FROM carti WHERE idLibrarie = %d;", idLibrarie);
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            delete[] carti; nrCarti = 0; return nullptr;
        }

        int i = 0;
        while (sqlite3_step(stmt) == SQLITE_ROW && i < nrCarti) {
            int dbId          = sqlite3_column_int(stmt, 0);
            const char* titlu = (const char*)sqlite3_column_text(stmt, 1);
            int an            = sqlite3_column_int(stmt, 2);
            int pag           = sqlite3_column_int(stmt, 3);
            int cat           = sqlite3_column_int(stmt, 4);
            float pret        = (float)sqlite3_column_double(stmt, 5);
            const char* aNume = (const char*)sqlite3_column_text(stmt, 6);
            int aAn           = sqlite3_column_int(stmt, 7);
            int aNrPremii     = sqlite3_column_int(stmt, 8);
            int aNrScrieri    = sqlite3_column_int(stmt, 9);

            carti[i].setTitlu(titlu ? titlu : "Necunoscut");
            carti[i].setAnPublicare(an);
            carti[i].setNrPag(pag);
            carti[i].setCategorie((Categorie)cat);
            carti[i].setPret(pret);

            Autor a;
            a.setNume(aNume ? aNume : "Anonim");
            a.setAn(aAn); a.setPremii(aNrPremii); a.setScrieri(aNrScrieri);
            carti[i].setScriitor(a);

            sqlite3_stmt* stmtR;
            char sqlR[256];
            snprintf(sqlR, sizeof(sqlR),
                "SELECT valoare FROM ratinguri WHERE idCarte = %d;", dbId);
            if (sqlite3_prepare_v2(db, sqlR, -1, &stmtR, nullptr) == SQLITE_OK) {
                std::vector<float> r;
                while (sqlite3_step(stmtR) == SQLITE_ROW)
                    r.push_back((float)sqlite3_column_double(stmtR, 0));
                sqlite3_finalize(stmtR);
                if (!r.empty()) carti[i].setRatinguri(r);
            }
            i++;
        }
        sqlite3_finalize(stmt);
        cout << "[DB] " << nrCarti << " carti incarcate." << endl;
        return carti;
    }

    // CLIENTI
    void salveazaClient(const Cititor& c, int idLibrarie) {
        std::string nume = c.getNume();
        char gen[2] = { c.getGen(), '\0' };

        char sql[512];
        snprintf(sql, sizeof(sql),
            "INSERT INTO clienti (idLibrarie, nume, buget, gen, areCardFidelitate)"
            " VALUES (%d, '%s', %.2f, '%s', %d);",
            idLibrarie, nume.empty() ? "Anonim" : nume.c_str(),
            c.getBuget(), gen, (int)c.getStatus()
        );
        if (!executeSQL(sql)) return;

        long long idClient = sqlite3_last_insert_rowid(db);
        // getCartiCumparate() returneaza vector<int>
        std::vector<int> carti = c.getCartiCumparate();
        for (int id : carti) {
            char sqlC[256];
            snprintf(sqlC, sizeof(sqlC),
                "INSERT INTO carti_cumparate (idClient, idCarte) VALUES (%lld, %d);",
                idClient, id);
            executeSQL(sqlC);
        }
    }

    Cititor* incarcaClientiLibrarie(int idLibrarie, int& nrClienti) {
        nrClienti = 0;
        sqlite3_stmt* stmt;
        char sql[256];

        snprintf(sql, sizeof(sql),
            "SELECT COUNT(*) FROM clienti WHERE idLibrarie = %d;", idLibrarie);
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) return nullptr;
        if (sqlite3_step(stmt) == SQLITE_ROW) nrClienti = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);
        if (nrClienti == 0) return nullptr;

        Cititor* clienti = new Cititor[nrClienti];
        snprintf(sql, sizeof(sql),
            "SELECT id, nume, buget, gen, areCardFidelitate"
            " FROM clienti WHERE idLibrarie = %d;", idLibrarie);
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            delete[] clienti; nrClienti = 0; return nullptr;
        }

        int i = 0;
        while (sqlite3_step(stmt) == SQLITE_ROW && i < nrClienti) {
            int dbId           = sqlite3_column_int(stmt, 0);
            const char* nume   = (const char*)sqlite3_column_text(stmt, 1);
            float buget        = (float)sqlite3_column_double(stmt, 2);
            const char* genStr = (const char*)sqlite3_column_text(stmt, 3);
            bool card          = sqlite3_column_int(stmt, 4) != 0;

            clienti[i].setNume(nume ? nume : "Anonim");
            clienti[i].setBuget(buget);
            clienti[i].setGen(genStr ? genStr[0] : 'N');
            clienti[i].setStatus(card);

            sqlite3_stmt* stmtC;
            char sqlC[256];
            snprintf(sqlC, sizeof(sqlC),
                "SELECT idCarte FROM carti_cumparate WHERE idClient = %d;", dbId);
            if (sqlite3_prepare_v2(db, sqlC, -1, &stmtC, nullptr) == SQLITE_OK) {
                std::vector<int> cp;
                while (sqlite3_step(stmtC) == SQLITE_ROW)
                    cp.push_back(sqlite3_column_int(stmtC, 0));
                sqlite3_finalize(stmtC);
                // setCartiCumparate primeste vector<int>
                if (!cp.empty()) clienti[i].setCartiCumparate(cp);
            }
            i++;
        }
        sqlite3_finalize(stmt);
        cout << "[DB] " << nrClienti << " clienti incarcati." << endl;
        return clienti;
    }

    // ANGAJATI
    void salveazaAngajat(const Angajat& a, int idLibrarie) {
        std::string nume = a.getNume();
        char gen[2] = { a.getGen(), '\0' };
        char sql[512];
        snprintf(sql, sizeof(sql),
            "INSERT INTO angajati (idLibrarie, nume, varsta, gen, salariu, anAngajare, departament)"
            " VALUES (%d, '%s', %d, '%s', %.2f, %d, %d);",
            idLibrarie, nume.empty() ? "Anonim" : nume.c_str(),
            a.getVarsta(), gen, a.getSalariu(), a.getAnAngajare(), (int)a.getDepartament()
        );
        executeSQL(sql);
    }

    Angajat* incarcaAngajatiLibrarie(int idLibrarie, int& nrAngajati) {
        nrAngajati = 0;
        sqlite3_stmt* stmt;
        char sql[256];

        snprintf(sql, sizeof(sql),
            "SELECT COUNT(*) FROM angajati WHERE idLibrarie = %d;", idLibrarie);
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) return nullptr;
        if (sqlite3_step(stmt) == SQLITE_ROW) nrAngajati = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);
        if (nrAngajati == 0) return nullptr;

        Angajat* angajati = new Angajat[nrAngajati];
        snprintf(sql, sizeof(sql),
            "SELECT id, nume, varsta, gen, salariu, anAngajare, departament"
            " FROM angajati WHERE idLibrarie = %d;", idLibrarie);
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            delete[] angajati; nrAngajati = 0; return nullptr;
        }

        int i = 0;
        while (sqlite3_step(stmt) == SQLITE_ROW && i < nrAngajati) {
            const char* nume   = (const char*)sqlite3_column_text(stmt, 1);
            int varsta         = sqlite3_column_int(stmt, 2);
            const char* genStr = (const char*)sqlite3_column_text(stmt, 3);
            float salariu      = (float)sqlite3_column_double(stmt, 4);
            int anAngajare     = sqlite3_column_int(stmt, 5);
            int departament    = sqlite3_column_int(stmt, 6);

            angajati[i].setNume(nume ? nume : "Anonim");
            angajati[i].setVarsta(varsta);
            angajati[i].setGen(genStr ? genStr[0] : 'N');
            angajati[i].setSalariu(salariu);
            angajati[i].setAnAngajare(anAngajare);
            angajati[i].setDepartament((Departament)departament);
            i++;
        }
        sqlite3_finalize(stmt);
        cout << "[DB] " << nrAngajati << " angajati incarcati." << endl;
        return angajati;
    }

    // ANGAJAT-CITITOR
    void salveazaAngajatCititor(const AngajatCititor& ac, int idLibrarie) {
        std::string nume = ac.getNume();
        char gen[2] = { ac.getGen(), '\0' };
        // getCartiCumparate() returneaza vector<int>
        std::vector<int> carti = ac.getCartiCumparate();

        char sql[1024];
        snprintf(sql, sizeof(sql),
            "INSERT INTO angajat_cititor"
            " (idLibrarie, nume, varsta, gen, salariu, anAngajare, departament,"
            "  buget, nrCartiCumparate, areCardFidelitate)"
            " VALUES (%d, '%s', %d, '%s', %.2f, %d, %d, %.2f, %d, %d);",
            idLibrarie, nume.empty() ? "Anonim" : nume.c_str(),
            ac.getVarsta(), gen,
            ac.getSalariu(), ac.getAnAngajare(), (int)ac.getDepartament(),
            ac.getBuget(), (int)carti.size(), (int)ac.getStatus()
        );
        if (!executeSQL(sql)) return;

        long long idAC = sqlite3_last_insert_rowid(db);
        for (int id : carti) {
            char sqlC[256];
            snprintf(sqlC, sizeof(sqlC),
                "INSERT INTO angajat_cititor_carti (idAngajatCititor, idCarte)"
                " VALUES (%lld, %d);", idAC, id);
            executeSQL(sqlC);
        }
    }

    AngajatCititor* incarcaAngajatCititorLibrarie(int idLibrarie, int& nrAC) {
        nrAC = 0;
        sqlite3_stmt* stmt;
        char sql[256];

        snprintf(sql, sizeof(sql),
            "SELECT COUNT(*) FROM angajat_cititor WHERE idLibrarie = %d;", idLibrarie);
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) return nullptr;
        if (sqlite3_step(stmt) == SQLITE_ROW) nrAC = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);
        if (nrAC == 0) return nullptr;

        AngajatCititor* ac = new AngajatCititor[nrAC];
        snprintf(sql, sizeof(sql),
            "SELECT id, nume, varsta, gen, salariu, anAngajare, departament,"
            " buget, nrCartiCumparate, areCardFidelitate"
            " FROM angajat_cititor WHERE idLibrarie = %d;", idLibrarie);
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            delete[] ac; nrAC = 0; return nullptr;
        }

        int i = 0;
        while (sqlite3_step(stmt) == SQLITE_ROW && i < nrAC) {
            int dbId           = sqlite3_column_int(stmt, 0);
            const char* nume   = (const char*)sqlite3_column_text(stmt, 1);
            int varsta         = sqlite3_column_int(stmt, 2);
            const char* genStr = (const char*)sqlite3_column_text(stmt, 3);
            float salariu      = (float)sqlite3_column_double(stmt, 4);
            int anAngajare     = sqlite3_column_int(stmt, 5);
            int departament    = sqlite3_column_int(stmt, 6);
            float buget        = (float)sqlite3_column_double(stmt, 7);
            bool card          = sqlite3_column_int(stmt, 9) != 0;

            ac[i].setNume(nume ? nume : "Anonim");
            ac[i].setVarsta(varsta);
            ac[i].setGen(genStr ? genStr[0] : 'N');
            ac[i].setSalariu(salariu);
            ac[i].setAnAngajare(anAngajare);
            ac[i].setDepartament((Departament)departament);
            ac[i].setBuget(buget);
            ac[i].setStatus(card);

            sqlite3_stmt* stmtC;
            char sqlC[256];
            snprintf(sqlC, sizeof(sqlC),
                "SELECT idCarte FROM angajat_cititor_carti WHERE idAngajatCititor = %d;", dbId);
            if (sqlite3_prepare_v2(db, sqlC, -1, &stmtC, nullptr) == SQLITE_OK) {
                std::vector<int> cp;
                while (sqlite3_step(stmtC) == SQLITE_ROW)
                    cp.push_back(sqlite3_column_int(stmtC, 0));
                sqlite3_finalize(stmtC);
                if (!cp.empty()) ac[i].setCartiCumparate(cp);
            }
            i++;
        }
        sqlite3_finalize(stmt);
        cout << "[DB] " << nrAC << " angajat-cititori incarcati." << endl;
        return ac;
    }

    // MANAGERI
    void salveazaManager(const Manager& m, int idLibrarie) {
        std::string nume = m.getNume();
        char gen[2] = { m.getGen(), '\0' };
        char sql[512];
        snprintf(sql, sizeof(sql),
            "INSERT INTO manageri (idLibrarie, nume, varsta, gen, salariu, anAngajare, departament, bonus, nrSubalterni)"
            " VALUES (%d, '%s', %d, '%s', %.2f, %d, %d, %.2f, %d);",
            idLibrarie, nume.empty() ? "Anonim" : nume.c_str(),
            m.getVarsta(), gen,
            m.getSalariu(), m.getAnAngajare(), (int)m.getDepartament(),
            m.getBonus(), m.getNrSubalterni()
        );
        executeSQL(sql);
    }

    Manager* incarcaManageriLibrarie(int idLibrarie, int& nrManageri) {
        nrManageri = 0;
        sqlite3_stmt* stmt;
        char sql[256];

        snprintf(sql, sizeof(sql),
            "SELECT COUNT(*) FROM manageri WHERE idLibrarie = %d;", idLibrarie);
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) return nullptr;
        if (sqlite3_step(stmt) == SQLITE_ROW) nrManageri = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);
        if (nrManageri == 0) return nullptr;

        Manager* manageri = new Manager[nrManageri];
        snprintf(sql, sizeof(sql),
            "SELECT nume, varsta, gen, salariu, anAngajare, departament, bonus, nrSubalterni"
            " FROM manageri WHERE idLibrarie = %d;", idLibrarie);
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            delete[] manageri; nrManageri = 0; return nullptr;
        }

        int i = 0;
        while (sqlite3_step(stmt) == SQLITE_ROW && i < nrManageri) {
            const char* nume   = (const char*)sqlite3_column_text(stmt, 0);
            int varsta         = sqlite3_column_int(stmt, 1);
            const char* genStr = (const char*)sqlite3_column_text(stmt, 2);
            float salariu      = (float)sqlite3_column_double(stmt, 3);
            int anAngajare     = sqlite3_column_int(stmt, 4);
            int departament    = sqlite3_column_int(stmt, 5);
            float bonus        = (float)sqlite3_column_double(stmt, 6);
            int nrSub          = sqlite3_column_int(stmt, 7);

            manageri[i].setNume(nume ? nume : "Anonim");
            manageri[i].setVarsta(varsta);
            manageri[i].setGen(genStr ? genStr[0] : 'N');
            manageri[i].setSalariu(salariu);
            manageri[i].setAnAngajare(anAngajare);
            manageri[i].setDepartament((Departament)departament);
            manageri[i].setBonus(bonus);
            manageri[i].setNrSubalterni(nrSub);
            i++;
        }
        sqlite3_finalize(stmt);
        cout << "[DB] " << nrManageri << " manageri incarcati." << endl;
        return manageri;
    }

public:
    Database(const char* numeFisier = "librarie.db") : db(nullptr) {
        int rc = sqlite3_open(numeFisier, &db);
        if (rc != SQLITE_OK) {
            cout << "[DB] Nu s-a putut deschide baza de date: " << sqlite3_errmsg(db) << endl;
            db = nullptr;
        } else {
            cout << "[DB] Conectat la baza de date: " << numeFisier << endl;
            creeazaTabelele();
        }
    }

    ~Database() {
        if (db != nullptr) { sqlite3_close(db); cout << "[DB] Conexiune inchisa." << endl; }
    }

    bool esteConectat() const { return db != nullptr; }

    void creeazaTabelele() {
        executeSQL("CREATE TABLE IF NOT EXISTS librarii (id INTEGER PRIMARY KEY AUTOINCREMENT, denumire TEXT, adresa TEXT);");
        executeSQL("CREATE TABLE IF NOT EXISTS carti (id INTEGER PRIMARY KEY AUTOINCREMENT, idLibrarie INTEGER, titlu TEXT, anPublicare INTEGER, nrPag INTEGER, categorie INTEGER, pret REAL, autorNume TEXT, autorAn INTEGER, autorNrPremii INTEGER, autorNrScrieri INTEGER, FOREIGN KEY(idLibrarie) REFERENCES librarii(id));");
        executeSQL("CREATE TABLE IF NOT EXISTS ratinguri (idCarte INTEGER, valoare REAL, FOREIGN KEY(idCarte) REFERENCES carti(id));");
        executeSQL("CREATE TABLE IF NOT EXISTS clienti (id INTEGER PRIMARY KEY AUTOINCREMENT, idLibrarie INTEGER, nume TEXT, buget REAL, gen TEXT, areCardFidelitate INTEGER, FOREIGN KEY(idLibrarie) REFERENCES librarii(id));");
        executeSQL("CREATE TABLE IF NOT EXISTS carti_cumparate (idClient INTEGER, idCarte INTEGER, FOREIGN KEY(idClient) REFERENCES clienti(id), FOREIGN KEY(idCarte) REFERENCES carti(id));");
        executeSQL("CREATE TABLE IF NOT EXISTS angajati (id INTEGER PRIMARY KEY AUTOINCREMENT, idLibrarie INTEGER, nume TEXT, varsta INTEGER, gen TEXT, salariu REAL, anAngajare INTEGER, departament INTEGER, FOREIGN KEY(idLibrarie) REFERENCES librarii(id));");
        executeSQL("CREATE TABLE IF NOT EXISTS angajat_cititor (id INTEGER PRIMARY KEY AUTOINCREMENT, idLibrarie INTEGER, nume TEXT, varsta INTEGER, gen TEXT, salariu REAL, anAngajare INTEGER, departament INTEGER, buget REAL, nrCartiCumparate INTEGER, areCardFidelitate INTEGER, FOREIGN KEY(idLibrarie) REFERENCES librarii(id));");
        executeSQL("CREATE TABLE IF NOT EXISTS angajat_cititor_carti (idAngajatCititor INTEGER, idCarte INTEGER, FOREIGN KEY(idAngajatCititor) REFERENCES angajat_cititor(id), FOREIGN KEY(idCarte) REFERENCES carti(id));");
        executeSQL("CREATE TABLE IF NOT EXISTS manageri (id INTEGER PRIMARY KEY AUTOINCREMENT, idLibrarie INTEGER, nume TEXT, varsta INTEGER, gen TEXT, salariu REAL, anAngajare INTEGER, departament INTEGER, bonus REAL, nrSubalterni INTEGER, FOREIGN KEY(idLibrarie) REFERENCES librarii(id));");
    }

    int alegeLibrarie() {
        if (!db) return -1;
        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(db, "SELECT id, denumire, adresa FROM librarii;", -1, &stmt, nullptr) != SQLITE_OK) return -1;

        cout << "\n========== LIBRARII DISPONIBILE ==========" << endl;
        int iduri[64]; int nrLibrarii = 0;
        while (sqlite3_step(stmt) == SQLITE_ROW && nrLibrarii < 64) {
            int id = sqlite3_column_int(stmt, 0);
            const char* den = (const char*)sqlite3_column_text(stmt, 1);
            const char* adr = (const char*)sqlite3_column_text(stmt, 2);
            cout << nrLibrarii+1 << ". " << (den?den:"?") << "  |  " << (adr?adr:"?") << endl;
            iduri[nrLibrarii++] = id;
        }
        sqlite3_finalize(stmt);
        cout << "===========================================" << endl;

        if (nrLibrarii == 0) { cout << "[DB] Nu exista librarii." << endl; return -1; }
        int opt = 0;
        do { cout << "Alege libraria (1-" << nrLibrarii << "): "; cin >> opt; }
        while (opt < 1 || opt > nrLibrarii);
        return iduri[opt-1];
    }

    void incarcaLibrarie(Librarie& l, int idLibrarie) {
        if (!db) return;

        char sql[256]; sqlite3_stmt* stmt;
        snprintf(sql, sizeof(sql), "SELECT denumire, adresa FROM librarii WHERE id = %d;", idLibrarie);
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                const char* den = (const char*)sqlite3_column_text(stmt, 0);
                const char* adr = (const char*)sqlite3_column_text(stmt, 1);
                l.setDenumire(den ? den : "Librarie");
                l.setAdresa(adr ? adr : "Adresa necunoscuta");
            }
        }
        sqlite3_finalize(stmt);

        int nrCarti = 0;
        Carte* carti = incarcaCartiLibrarie(idLibrarie, nrCarti);
        if (carti && nrCarti > 0) {
            l.setStoc(std::vector<Carte>(carti, carti + nrCarti));
            delete[] carti;
        }

        int nrClienti = 0;
        Cititor* clienti = incarcaClientiLibrarie(idLibrarie, nrClienti);
        if (clienti && nrClienti > 0) {
            l.setClienti(std::vector<Cititor>(clienti, clienti + nrClienti));
            delete[] clienti;
        }

        int nrAngajati = 0;
        Angajat* angajati = incarcaAngajatiLibrarie(idLibrarie, nrAngajati);
        if (angajati && nrAngajati > 0) {
            l.setAngajati(std::vector<Angajat>(angajati, angajati + nrAngajati));
            delete[] angajati;
        }

        int nrAC = 0;
        AngajatCititor* ac = incarcaAngajatCititorLibrarie(idLibrarie, nrAC);
        for (int i = 0; i < nrAC; i++)
            l.adaugaPersoana(new AngajatCititor(ac[i]));
        if (ac) delete[] ac;

        int nrManageri = 0;
        Manager* manageri = incarcaManageriLibrarie(idLibrarie, nrManageri);
        for (int i = 0; i < nrManageri; i++)
            l.adaugaPersoana(new Manager(manageri[i]));
        if (manageri) delete[] manageri;

        cout << "[DB] Libraria a fost incarcata complet." << endl;
    }

    void salveazaLibrarie(const Librarie& l, int idLibrarie) {
        if (!db) return;

        char sql[512];
        snprintf(sql, sizeof(sql),
            "UPDATE librarii SET denumire='%s', adresa='%s' WHERE id=%d;",
            l.getDenumire().c_str(), l.getAdresa().c_str(), idLibrarie);
        executeSQL(sql);

        snprintf(sql, sizeof(sql), "DELETE FROM ratinguri WHERE idCarte IN (SELECT id FROM carti WHERE idLibrarie = %d);", idLibrarie);
        executeSQL(sql);
        snprintf(sql, sizeof(sql), "DELETE FROM carti_cumparate WHERE idClient IN (SELECT id FROM clienti WHERE idLibrarie = %d);", idLibrarie);
        executeSQL(sql);
        snprintf(sql, sizeof(sql), "DELETE FROM angajat_cititor_carti WHERE idAngajatCititor IN (SELECT id FROM angajat_cititor WHERE idLibrarie = %d);", idLibrarie);
        executeSQL(sql);
        snprintf(sql, sizeof(sql), "DELETE FROM carti WHERE idLibrarie = %d;", idLibrarie);
        executeSQL(sql);
        snprintf(sql, sizeof(sql), "DELETE FROM clienti WHERE idLibrarie = %d;", idLibrarie);
        executeSQL(sql);
        snprintf(sql, sizeof(sql), "DELETE FROM angajati WHERE idLibrarie = %d;", idLibrarie);
        executeSQL(sql);
        snprintf(sql, sizeof(sql), "DELETE FROM angajat_cititor WHERE idLibrarie = %d;", idLibrarie);
        executeSQL(sql);
        snprintf(sql, sizeof(sql), "DELETE FROM manageri WHERE idLibrarie = %d;", idLibrarie);
        executeSQL(sql);

        for (const Carte& c : l.getStoc())    salveazaCarte(c, idLibrarie);
        for (const Cititor& c : l.getClienti()) salveazaClient(c, idLibrarie);
        for (const Angajat& a : l.getAngajati()) salveazaAngajat(a, idLibrarie);

        // getPersoane() returneaza copii prin clone() — facem delete dupa
        for (Persoana* p : l.getPersoane()) {
            if (AngajatCititor* ac = dynamic_cast<AngajatCititor*>(p))
                salveazaAngajatCititor(*ac, idLibrarie);
            else if (Manager* mg = dynamic_cast<Manager*>(p))
                salveazaManager(*mg, idLibrarie);
            delete p;
        }

        cout << "[DB] Libraria a fost salvata." << endl;
    }
};

#endif