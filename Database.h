#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <iostream>
#include <cstring>
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

    void salveazaCarte(const Carte& c, int idLibrarie) {
        char* titlu     = c.getTitlu();
        Autor sc        = c.getScriitor();
        char* autorNume = sc.getNume();

        char sql[1024];
        snprintf(sql, sizeof(sql),
            "INSERT INTO carti (idLibrarie, titlu, anPublicare, nrPag, categorie, pret,"
            " autorNume, autorAn, autorNrPremii, autorNrScrieri)"
            " VALUES (%d, '%s', %d, %d, %d, %.2f, '%s', %d, %d, %d);",
            idLibrarie,
            titlu     ? titlu     : "Necunoscut",
            c.getAnPublicare(), c.getNrPag(), (int)c.getCategorie(), c.getPret(),
            autorNume ? autorNume : "Anonim",
            sc.getAnNastere(), sc.getNrPremii(), sc.getNrScrieri()
        );
        delete[] titlu;
        delete[] autorNume;

        if (!executeSQL(sql)) return;

        long long idCarte = sqlite3_last_insert_rowid(db);
        int nr            = c.getNrRatinguri();
        float* ratinguri  = c.getRating();
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
            a.setAn(aAn);
            a.setPremii(aNrPremii);
            a.setScrieri(aNrScrieri);
            carti[i].setScriitor(a);

            sqlite3_stmt* stmtR;
            char sqlR[256];
            snprintf(sqlR, sizeof(sqlR),
                "SELECT valoare FROM ratinguri WHERE idCarte = %d;", dbId);
            int nrR = 0;
            if (sqlite3_prepare_v2(db, sqlR, -1, &stmtR, nullptr) == SQLITE_OK) {
                float temp[256];
                while (sqlite3_step(stmtR) == SQLITE_ROW && nrR < 256)
                    temp[nrR++] = (float)sqlite3_column_double(stmtR, 0);
                sqlite3_finalize(stmtR);
                if (nrR > 0) {
                    float* rc = new float[nrR];
                    for (int r = 0; r < nrR; r++) rc[r] = temp[r];
                    carti[i].setRatinguri(nrR, rc);
                    delete[] rc;
                }
            }
            i++;
        }
        sqlite3_finalize(stmt);
        cout << "[DB] " << nrCarti << " carti incarcate." << endl;
        return carti;
    }

    void salveazaClient(const Cititor& c, int idLibrarie) {
        char* nume  = c.getNume();
        char gen[2] = { c.getGen(), '\0' };

        char sql[512];
        snprintf(sql, sizeof(sql),
            "INSERT INTO clienti (idLibrarie, nume, buget, gen, areCardFidelitate)"
            " VALUES (%d, '%s', %.2f, '%s', %d);",
            idLibrarie, nume ? nume : "Anonim",
            c.getBuget(), gen, (int)c.getStatus()
        );
        delete[] nume;

        if (!executeSQL(sql)) return;

        long long idClient = sqlite3_last_insert_rowid(db);
        int nr             = c.getNrCartiCumparate();
        int* carti         = c.getCartiCumparate();
        for (int i = 0; i < nr; i++) {
            char sqlC[256];
            snprintf(sqlC, sizeof(sqlC),
                "INSERT INTO carti_cumparate (idClient, idCarte) VALUES (%lld, %d);",
                idClient, carti[i]);
            executeSQL(sqlC);
        }
        if (carti) delete[] carti;
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
            int nrC = 0;
            if (sqlite3_prepare_v2(db, sqlC, -1, &stmtC, nullptr) == SQLITE_OK) {
                int temp[256];
                while (sqlite3_step(stmtC) == SQLITE_ROW && nrC < 256)
                    temp[nrC++] = sqlite3_column_int(stmtC, 0);
                sqlite3_finalize(stmtC);
                if (nrC > 0) {
                    int* cp = new int[nrC];
                    for (int c = 0; c < nrC; c++) cp[c] = temp[c];
                    clienti[i].setCartiCumparate(cp, nrC);
                    delete[] cp;
                }
            }
            i++;
        }
        sqlite3_finalize(stmt);
        cout << "[DB] " << nrClienti << " clienti incarcati." << endl;
        return clienti;
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
        if (db != nullptr) {
            sqlite3_close(db);
            cout << "[DB] Conexiune inchisa." << endl;
        }
    }

    bool esteConectat() const { return db != nullptr; }

    void creeazaTabelele() {
        executeSQL(
            "CREATE TABLE IF NOT EXISTS librarii ("
            "  id       INTEGER PRIMARY KEY AUTOINCREMENT,"
            "  denumire TEXT,"
            "  adresa   TEXT"
            ");"
        );
        executeSQL(
            "CREATE TABLE IF NOT EXISTS carti ("
            "  id             INTEGER PRIMARY KEY AUTOINCREMENT,"
            "  idLibrarie     INTEGER,"
            "  titlu          TEXT,"
            "  anPublicare    INTEGER,"
            "  nrPag          INTEGER,"
            "  categorie      INTEGER,"
            "  pret           REAL,"
            "  autorNume      TEXT,"
            "  autorAn        INTEGER,"
            "  autorNrPremii  INTEGER,"
            "  autorNrScrieri INTEGER,"
            "  FOREIGN KEY(idLibrarie) REFERENCES librarii(id)"
            ");"
        );
        executeSQL(
            "CREATE TABLE IF NOT EXISTS ratinguri ("
            "  idCarte INTEGER,"
            "  valoare REAL,"
            "  FOREIGN KEY(idCarte) REFERENCES carti(id)"
            ");"
        );
        executeSQL(
            "CREATE TABLE IF NOT EXISTS clienti ("
            "  id                INTEGER PRIMARY KEY AUTOINCREMENT,"
            "  idLibrarie        INTEGER,"
            "  nume              TEXT,"
            "  buget             REAL,"
            "  gen               TEXT,"
            "  areCardFidelitate INTEGER,"
            "  FOREIGN KEY(idLibrarie) REFERENCES librarii(id)"
            ");"
        );
        executeSQL(
            "CREATE TABLE IF NOT EXISTS carti_cumparate ("
            "  idClient INTEGER,"
            "  idCarte  INTEGER,"
            "  FOREIGN KEY(idClient) REFERENCES clienti(id),"
            "  FOREIGN KEY(idCarte)  REFERENCES carti(id)"
            ");"
        );
    }

    // Afiseaza lista librariilor si returneaza id-ul din DB al celei alese
    int alegeLibrarie() {
        if (!db) return -1;

        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(db, "SELECT id, denumire, adresa FROM librarii;",
                               -1, &stmt, nullptr) != SQLITE_OK)
            return -1;

        cout << "\n========== LIBRARII DISPONIBILE ==========" << endl;
        int iduri[64];
        int nrLibrarii = 0;
        while (sqlite3_step(stmt) == SQLITE_ROW && nrLibrarii < 64) {
            int id          = sqlite3_column_int(stmt, 0);
            const char* den = (const char*)sqlite3_column_text(stmt, 1);
            const char* adr = (const char*)sqlite3_column_text(stmt, 2);
            cout << nrLibrarii + 1 << ". "
                 << (den ? den : "?") << "  |  " << (adr ? adr : "?") << endl;
            iduri[nrLibrarii++] = id;
        }
        sqlite3_finalize(stmt);
        cout << "===========================================" << endl;

        if (nrLibrarii == 0) {
            cout << "[DB] Nu exista librarii in baza de date." << endl;
            return -1;
        }

        int optiune = 0;
        do {
            cout << "Alege libraria (1-" << nrLibrarii << "): ";
            cin >> optiune;
        } while (optiune < 1 || optiune > nrLibrarii);

        return iduri[optiune - 1];
    }

    void incarcaLibrarie(Librarie& l, int idLibrarie) {
        if (!db) return;

        char sql[256];
        sqlite3_stmt* stmt;
        snprintf(sql, sizeof(sql),
            "SELECT denumire, adresa FROM librarii WHERE id = %d;", idLibrarie);
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
        if (carti && nrCarti > 0) { l.setStoc(carti, nrCarti); delete[] carti; }

        int nrClienti = 0;
        Cititor* clienti = incarcaClientiLibrarie(idLibrarie, nrClienti);
        if (clienti && nrClienti > 0) { l.setClienti(clienti, nrClienti); delete[] clienti; }

        cout << "[DB] Libraria a fost incarcata complet." << endl;
    }

    void salveazaLibrarie(const Librarie& l, int idLibrarie) {
        if (!db) return;

        char* den = l.getDenumire();
        char* adr = l.getAdresa();
        char sql[512];
        snprintf(sql, sizeof(sql),
            "UPDATE librarii SET denumire='%s', adresa='%s' WHERE id=%d;",
            den ? den : "Necunoscuta", adr ? adr : "Necunoscuta", idLibrarie);
        delete[] den; delete[] adr;
        executeSQL(sql);

        snprintf(sql, sizeof(sql),
            "DELETE FROM ratinguri WHERE idCarte IN"
            " (SELECT id FROM carti WHERE idLibrarie = %d);", idLibrarie);
        executeSQL(sql);
        snprintf(sql, sizeof(sql),
            "DELETE FROM carti_cumparate WHERE idClient IN"
            " (SELECT id FROM clienti WHERE idLibrarie = %d);", idLibrarie);
        executeSQL(sql);
        snprintf(sql, sizeof(sql),
            "DELETE FROM carti WHERE idLibrarie = %d;", idLibrarie);
        executeSQL(sql);
        snprintf(sql, sizeof(sql),
            "DELETE FROM clienti WHERE idLibrarie = %d;", idLibrarie);
        executeSQL(sql);

        Carte* stoc  = l.getStoc();
        int nrCarti  = l.getNrCarti();
        for (int i = 0; i < nrCarti; i++) salveazaCarte(stoc[i], idLibrarie);
        if (stoc) delete[] stoc;

        Cititor* clt  = l.getClienti();
        int nrClienti = l.getNrClienti();
        for (int i = 0; i < nrClienti; i++) salveazaClient(clt[i], idLibrarie);
        if (clt) delete[] clt;

        cout << "[DB] Libraria a fost salvata." << endl;
    }
};

#endif
