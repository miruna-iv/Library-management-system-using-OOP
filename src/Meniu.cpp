#include "Meniu.h"
#include "Exceptii.h"
#include "Colectie.h"
#include "ConfigurareLibrarie.h"

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

#define CATCH_EXCEPTII \
    catch(const ValidationError& e){ cout << e.what() << endl; } \
    catch(const NotFoundError& e){ cout << e.what() << endl; } \
    catch(const InsufficientBudgetError& e){ cout << e.what() << endl; } \
    catch(const LibrarieError& e){ cout << "[Eroare] " << e.what() << endl; } \
    catch(...){ cout << "[Eroare necunoscuta]" << endl; }


Meniu::Meniu(Librarie& lib, IAbonamentFactory* f) : librarie(lib), factory(f) {}

void Meniu::curatareBuffer(){
    cin.ignore(1000, '\n');
}

void Meniu::afiseazaMeniuPrincipal(){
    cout<<"\n----------- " << librarie.getDenumire() << " -----------"<<endl;
    cout<<"0. Iesire"<<endl;
    cout<<"1. Meniu clienti"<<endl;
    cout<<"2. Meniu carti"<<endl;
    cout<<"3. Meniu angajati"<<endl;
    cout<<"4. Simulare vanzare"<<endl;
    cout<<"5. Afisare configuratie librarie"<<endl;
    cout<<"--------------------------------------"<<endl;
}

void Meniu::porneste(){
    int opt= -1;

    while(opt != 0){
        afiseazaMeniuPrincipal();
        cout<<"Optiune: ";
        cin>>opt;

        switch(opt){
            case 0: cout<<"Va multumin si va mai asteptam!"<<endl; break;
            case 1: meniuClienti(); break;
            case 2: meniuCarti(); break;
            case 3: meniuAngajati(); break;
            case 4: executaVanzare(); break;
            case 5: {
                int optCfg;
                CONFIG.afiseaza();
                cout<<"1. Modifica TVA  2. Modifica reducere maxima  3. Modifica salariu minim  0. Inapoi\n";
                cout<<"Optiune: "; cin>>optCfg;
                try {
                    if(optCfg == 1){
                        float t; 
                        cout<<"TVA nou (%): "; 
                        cin>>t;
                        CONFIG.setTVA(t);
                        cout<<"TVA actualizat.\n";
                    } else if(optCfg == 2){
                        float r; 
                        cout<<"Reducere maxima noua (%): "; 
                        cin>>r;
                        CONFIG.setReducereMaxima(r);
                        cout<<"Reducere maxima actualizata.\n";
                    } else if(optCfg == 3){
                        float s; 
                        cout<<"Salariu minim nou (RON): "; 
                        cin>>s;
                        CONFIG.setSalariuMinim(s);
                        cout<<"Salariu minim actualizat.\n";
                    }
                } catch(const ValidationError& e){
                    cout << e.what() << endl;
                }
                break;
            }
            default: cout<<"Optiune invalida"<<endl;
        }
    }
}

void Meniu::executaVanzare(){
    try{
        vector<Cititor> listaClienti=librarie.getClienti();
        vector<Carte> listaCarti=librarie.getStoc();

        int nrCarti=(int)listaCarti.size();
        int nrClienti=(int)listaClienti.size();
        

        if(nrCarti==0) throw NotFoundError(0, "stoc");
        if(nrClienti==0) throw NotFoundError(0, "clienti");

        cout<<"Selectiati clientul: "<<endl;
        for(int i=0; i<nrClienti; i++)
            cout<<i<<". "<<listaClienti[i].getNume()<<" (Buget: "<<listaClienti[i].getBuget()<<" RON)"<<endl;
        
        int indexClient;
        cout<<"Index-ul clientului: ";
        cin>>indexClient;
        if(indexClient<0 || indexClient>=nrClienti) throw NotFoundError(indexClient, "clienti");


        cout<<"Selectati cartea: "<<endl;
        for(int i=0; i<nrCarti; i++)
            cout<<i<<". "<<listaCarti[i].getTitlu()<<" - "<<listaCarti[i].getPret()<<" RON"<<endl;
        
        int indexCarte;
        cout<<"Index-ul cartii: ";
        cin>>indexCarte;
        if(indexCarte<0 || indexCarte>=nrCarti) throw NotFoundError(indexCarte, "stoc");


        float pretCarte=listaCarti[indexCarte].getPret();
        float bugetClient=listaClienti[indexClient].getBuget();

        //demonstreaza stack unwinding:
        // estructorul sau e apelat automat daca InsufficientBudgetError e aruncata
            ResourceGuard guard("tranzactie_vanzare");

        if(bugetClient < pretCarte)
                throw InsufficientBudgetError(bugetClient, pretCarte);

        //bugetul este actualizat folosindu-ne de supraincarcarea operatorului - din clasa Cititor;
        listaClienti[indexClient]=listaClienti[indexClient]-pretCarte;

        listaClienti[indexClient] = listaClienti[indexClient] + listaCarti[indexCarte].getId();
        librarie.setClienti(listaClienti);
        cout<<"Vanzare efectuata cu succes!"<<endl;
    
    } CATCH_EXCEPTII
}

void Meniu::meniuClienti(){
    int opt=-1;
    while(opt){
        cout<<"----------------MENIU CLIENTI----------------"<<endl;
        cout<<"0. Iesire"<<endl;
        cout<<"1. Vizualizare lista de clienti"<<endl;
        cout<<"2. Inregistrare client nou"<<endl;
        cout<<"3. Adauga suma la buget"<<endl;
        cout<<"4. Vizualizare istoric client"<<endl;
        cout<<"5. Vizualizare persoane"<<endl;
        cout<<"6. Vizualizare clienti cu card fidelitate"<<endl;
        cout<<"7. Nr clienti cu buget peste o limita"<<endl;
        cout<<"8. Schimba tipul de abonament al unui client"<<endl;
        cout<<"---------------------------------------------"<<endl;

        cin>>opt;
        switch(opt){
            case 0:{
                return;
            }
            case 1: try{
                vector<Cititor> aux=librarie.getClienti();
                if(aux.empty()) throw NotFoundError(0, "clienti");
                for(const auto& c : aux) cout<<c<<endl;
            } CATCH_EXCEPTII break;

            case 2: try{
                Cititor c;
                cin >> c;  // citeste toate datele

                cout<<"Tip abonament: 1. Basic  2. VIP\n";
                cout<<"Optiune: "; int optAb; cin>>optAb;

                if(optAb == 2){
                    c.setStatus(true);
                    c.setBuget(c.getBuget() + 100.0f);
                }
                librarie.adaugaClient(c);
                cout<<"Clientul a fost adaugat cu succes!"<<endl;
            } CATCH_EXCEPTII break;

            case 3: try {
                int nrClienti= librarie.getClienti().size();
                if(nrClienti==0) throw NotFoundError(0, "clienti");

                int indexClient; 
                cout<<"Introduceti indexul clientului (0-" << librarie.getClienti().size() - 1 << "): "; 
                cin>>indexClient;
                if(indexClient<0 || indexClient>=nrClienti) throw NotFoundError(indexClient, "clienti");

                float suma; 
                cout<<"Introduceti suma de adaugat: "; 
                cin>>suma;
                if(suma < 0) throw ValidationError("suma", suma);

                librarie.getClient(indexClient) = librarie.getClient(indexClient) + suma;  
                cout<<"Suma a fost adaugata cu succes!"<<endl; 

            } CATCH_EXCEPTII break;

            case 4: try{
                int indexClient; 
                cout<<"Introduceti indexul clientului (0-" << librarie.getClienti().size() - 1 << "): "; 
                cin>>indexClient; 
                if(indexClient<0 || indexClient>=librarie.getClienti().size()) throw NotFoundError(indexClient, "clienti");

                vector<int>carti = librarie.getClient(indexClient).getCartiCumparate();
                if(carti.empty()) cout<<"Niciun istoric de cumparaturi."<<endl;
                for(int id : carti)
                    cout<<"ID: "<<id<<endl;
                
            }CATCH_EXCEPTII break;


            case 5: try{
                librarie.afiseazaPersoane();
                
            } CATCH_EXCEPTII break;

            case 6: try {
                vector<Cititor> cuCard = librarie.clientiCuCard();
                if(cuCard.empty()) throw NotFoundError(0, "clienti cu card");
                
                for(const auto& c : cuCard) 
                    cout<<c.getNume()<<endl;
            } CATCH_EXCEPTII break;
 
            case 7: try {
                float limita; 
                cout<<"Limita buget: "; 
                cin>>limita;
                if(limita < 0) throw ValidationError("limita", limita);

                cout<<"Clienti cu buget peste "<<limita<<" RON: "<<librarie.nrClientiCuBugetPeste(limita)<<endl;
            } CATCH_EXCEPTII break;

            case 8: try {
                vector<Cititor> cl = librarie.getClienti();
                int nr = (int)cl.size();
                if(nr == 0) throw NotFoundError(0, "clienti");
 
                for(int i = 0; i < nr; i++)
                    cout << i << ". " << cl[i].getNume()
                         << " | Card: " << (cl[i].getStatus() ? "Da" : "Nu")
                         << " | Buget: " << cl[i].getBuget() << " RON\n";
 
                int idx; 
                cout<<"Index client: ";
                cin>>idx;
                if(idx < 0 || idx >= nr) throw NotFoundError(idx, "clienti");
 
                cout<<"1. Basic (fara card, fara reduceri)\n";
                cout<<"2. VIP   (card fidelitate + 100 RON bonus)\n";
                cout<<"Optiune: "; 
                int optAb; 
                cin>>optAb;
 
                if(optAb == 1){
                    // schimba la Basic — elimina cardul si bugetul bonus
                    cl[idx].setStatus(false);
                    cout<<"Abonament schimbat la Basic.\n";
                } else if(optAb == 2){
                    // schimba la VIP — adauga card si 100 RON bonus
                    // folosim fabrica VIP prin interfata abstracta
                    IAbonamentFactory* vip = new AbonamentVIPFactory();
                    Cititor nou = vip->getCititorFactory()->creeaza(
                        cl[idx].getNume(), cl[idx].getGen(), cl[idx].getBuget());
                    // pastram istoricul de cumparaturi
                    nou.setCartiCumparate(cl[idx].getCartiCumparate());
                    cl[idx] = nou;
                    delete vip;
                    cout<<"Abonament schimbat la VIP.\n";
                } else {
                    cout<<"Optiune invalida!\n";
                    break;
                }
                librarie.setClienti(cl);
            } CATCH_EXCEPTII break;

            default: cout<<"Optiune invalida!"<<endl;
        }
    }

}

void Meniu::meniuCarti(){
    int opt=-1;
    while(opt!=0){
        cout<<"----------------MENIU CARTI----------------"<<endl;
        cout<<"0. Iesire"<<endl;
        cout<<"1. Vizualizare stoc in ordine crescatoare / descrescatoare"<<endl;
        cout<<"2. Adaugare carte in stoc"<<endl;
        cout<<"3. Aplica reducere intregului stoc"<<endl;
        cout<<"4. Modificare pret carte"<<endl;
        cout<<"5. Cauta carte dupa titlu"<<endl;
        cout<<"6. Sorteaza carti dupa pret (STL)"<<endl;
        cout<<"7. Filtreaza carti dupa pret maxim"<<endl;
        cout<<"8. Sterge carti ieftine"<<endl;
        cout<<"9. Top 5 carti dupa pret"<<endl;
        cout<<"-------------------------------------------"<<endl;

        cin>>opt;

        switch(opt){
            case 0:{
                return;
            }
            case 1: try{
                vector<Carte> stoc= librarie.getStoc();
                if(stoc.empty()) throw NotFoundError(0, "stoc");

                int opt1; 
                cout<<"In ce ordine doriti sa fie afisate cartile? (1-Crescator, 2-Descrescator): "; 
                cin>>opt1; 
                sort(stoc.begin(), stoc.end(), [opt1](const Carte& a, const Carte& b){
                    return opt1==1 ? a<b : a>b ;
                });
                
                for(const auto& c : stoc){
                    cout<<c<<endl;
                }
                
            } CATCH_EXCEPTII break;

            case 2: try{
                Carte c;
                cin >> c;

                cout<<"Tip abonament: 1. Basic  2. VIP\n";
                cout<<"Optiune: "; int optAb; cin>>optAb;

                if(optAb == 2)
                    c.setPret(c.getPret() * 0.8f);  // reducere VIP 20%

                librarie.adaugaCarte(c);
                cout<<"Cartea a fost adaugata cu succes!"<<endl;
            } CATCH_EXCEPTII break;

            case 3: try{ 
                float p; 
                cout<<"Procent reducere (0-100): "; 
                cin>>p; 
                if(p<0 || p>100) throw ValidationError("procent reducere", p);

                librarie=librarie-p; 
                cout<<"Reducere aplicata"<<endl;

            } CATCH_EXCEPTII break;

            case 4: try{ 
                vector<Carte> stoc= librarie.getStoc();
                if(stoc.empty()) throw NotFoundError(0, "stoc");
                
                int nrCarti=(int)stoc.size();
                cout<<"Cartile disponibile:"<<endl; 
                for(int i=0; i<nrCarti; i++){ 
                    string t=stoc[i].getTitlu(); 
                    cout<<i<<". "<<(t.empty()?"?":t)<<" - "<<stoc[i].getPret()<<" RON"<<endl;
                } 

                int idx; 
                cout<<"Alegeti cartea al carei pret vreti sa il modificati (0-"<<nrCarti-1<<"): "; 
                cin>>idx; 
                if(idx<0 || idx>=nrCarti) throw NotFoundError(idx, "stoc");

                int optPret; 
                cout<<"1. Creste pretul cu 1 RON"<<endl; 
                cout<<"2. Scade pretul cu 1 RON"<<endl; 
                cout<<"Alegeti optiunea: "; 
                cin>>optPret; 
                float pretVechi=stoc[idx].getPret(); 
                string titlu=stoc[idx].getTitlu(); 
                if(optPret==1){ 
                    ++stoc[idx]; 
                    cout<<"Pretul cartii \""<<(titlu.empty()?"?":titlu)<<"\" a crescut de la "<<pretVechi<<" la "<<stoc[idx].getPret()<<" RON."<<endl; 
                } 
                else{ 
                    --stoc[idx]; 
                    cout<<"Pretul cartii \""<<(titlu.empty()?"?":titlu)<<"\" a scazut de la "<<pretVechi<<" la "<<stoc[idx].getPret()<<" RON."<<endl; 
                }
                librarie.setStoc(stoc); 
                
            } CATCH_EXCEPTII break;

            case 5: try{
                string titlu;
                curatareBuffer();
                cout<<"Titlu: ";
                getline(cin, titlu);
                Carte* gasita = librarie.cautaCarte(titlu);
                if(!gasita) throw NotFoundError(0, "stoc (titlu: " + titlu + ")");

                cout<<*gasita<<endl;
                
            }CATCH_EXCEPTII break;


            case 6: try{
                if(librarie.getStoc().empty()) throw NotFoundError(0, "stoc");

                librarie.sorteazaCartiDupaPret();
                cout<<"Carti sortate dupa pret:"<<endl;
                for(const auto& c : librarie.getStoc())
                    cout<<c.getTitlu()<<" - "<<c.getPret()<<" RON"<<endl;
                
            }CATCH_EXCEPTII break;

            case 7: try{
                float pretMax;
                cout<<"Pret maxim: ";
                cin>>pretMax;
                if(pretMax<0) throw ValidationError("pret maxim", pretMax);

                vector<Carte> filtrate = librarie.filtreazaCartiDupaPret(pretMax);
                if(filtrate.empty()) throw NotFoundError(0, "stoc (sub pretul maxim)");
                for(const auto& c : filtrate)
                    cout<<c.getTitlu()<<" - "<<c.getPret()<<" RON"<<endl;
                
            } CATCH_EXCEPTII break;

            case 8: try{
                float pretMin; 
                cout<<"Sterge cartile cu pretul sub: "; 
                cin>>pretMin;
                if(pretMin < 0) throw ValidationError("pret minim", pretMin);

                librarie.stergeCartiIeftine(pretMin);
                cout<<"Carti sterse."<<endl;
            }CATCH_EXCEPTII break;

            case 9: try {
                vector<Carte> stoc = librarie.getStoc();
                if(stoc.empty()) throw NotFoundError(0, "stoc");
 
                Colectie<Carte, 5> top5;
                sort(stoc.begin(), stoc.end(),
                    [](const Carte& a, const Carte& b){ return a.getPret() > b.getPret(); });
 
                // adaugam primele min(5, nrCarti) carti in colectia cu dimensiune fixa
                for(int i = 0; i < (int)stoc.size() && !top5.full(); i++)
                    top5.adauga(stoc[i]);
 
                cout << "Top " << top5.size() << " carti dupa pret:\n";
                for(int i = 0; i < top5.size(); i++)
                    cout << i+1 << ". " << top5.get(i).getTitlu()
                         << " - " << top5.get(i).getPret() << " RON\n";
 
                Colectie<std::string, 5> titluri = top5.transforma<std::string>(
                    [](const Carte& c) { return c.getTitlu(); });
 
            } CATCH_EXCEPTII break;

            default: cout<<"Optiune invalida!"<<endl;
        }
    }
}

void Meniu::meniuAngajati(){
    int opt=-1;
    while(opt){
        cout<<"----------------MENIU ANGAJATI----------------"<<endl;
        cout<<"0. Iesire"<<endl;
        cout<<"1. Vizualizare angajati"<<endl;
        cout<<"2. Modificare salariu"<<endl;
        cout<<"3. Vizualizare angajati - clienti"<<endl;
        cout<<"4. Aplicare reducere angajat in cazul angajatilor clienti"<<endl;
        cout<<"5. Adaugare angajat"<<endl;
        cout<<"6. Adauga angajat-cititor"<<endl;
        cout<<"7. Vizualizare istoric angajat-cititor"<<endl;
        cout<<"8. Adaugare manager"<<endl;
        cout<<"9. Vizualizare manageri"<<endl;
        cout<<"10. Sorteaza angajati dupa salariu"<<endl;
        cout<<"----------------------------------------------"<<endl;
        cin>>opt;

        switch(opt){
            case 0:{
                return;
            }
            case 1: try{
                //Persoana p; eroare de compilare
                vector<Angajat> aux=librarie.getAngajati();
                vector<Persoana*> pers=librarie.getPersoane();

                if(aux.empty() && pers.empty()) throw NotFoundError(0, "angajati");

                for(const auto& a: aux)
                    cout<<a<<endl;
                for(Persoana* p : pers)
                    if(dynamic_cast<AngajatCititor*>(p))
                            p->afisare();

                for(Persoana* p : pers) 
                    delete p;
                
            }CATCH_EXCEPTII break;

            case 2: try{
                vector<Angajat> staff=librarie.getAngajati();
                vector<Persoana*> pers=librarie.getPersoane();
                int nrAng=(int)staff.size();
                int nrPers=(int)(int)pers.size();


                int nrAC = 0;
                for(Persoana* p : pers)
                    if(dynamic_cast<AngajatCititor*>(p)) nrAC++;

                int total = nrAng + nrAC;
                if(total == 0){
                    cout << "Niciun angajat." << endl;
                    for(Persoana* p : pers) delete p;
                    throw NotFoundError(0, "angajati");
                }

                int idx_in_pers[64]; int cntAC=0;
                for(int i=0;i<nrAng;i++){
                    string n = staff[i].getNume();
                    cout << i << ". " << (!n.empty()?n:"?") << " - " << staff[i].getSalariu() << " RON (Angajat)" << endl;
                }
                for(int i=0;i<nrPers;i++){
                    if(AngajatCititor* ac=dynamic_cast<AngajatCititor*>(pers[i])){
                        string n = ac->getNume();
                        cout << nrAng+cntAC << ". " << (!n.empty()?n:"?") << " - " << ac->getSalariu() << " RON (Angajat-Cititor)" << endl;
                        idx_in_pers[cntAC++]=i;
                    }
                }

                int idx;
                cout << "Alegeti angajatul (0-" << total-1 << "): ";
                cin >> idx;
                if(idx < 0 || idx >= total){
                    cout << "Index invalid!" << endl;
                    for(Persoana* p : pers) delete p;
                    throw NotFoundError(idx, "angajati");
                }
                int op;
                cout << "1. Creste cu 100 RON  2. Scade cu 100 RON: ";
                cin >> op;

                if(idx < nrAng){
                    float vechi = staff[idx].getSalariu();
                    string n = staff[idx].getNume();
                    if(op==1) ++staff[idx]; else --staff[idx];
                    cout << "\"" << (!n.empty()?n:"?") << "\": " << vechi << " -> " << staff[idx].getSalariu() << " RON" << endl;
                    librarie.setAngajati(staff);
                } else {
                    int persIdx = idx_in_pers[idx - nrAng];
                    AngajatCititor* ac = dynamic_cast<AngajatCititor*>(pers[persIdx]);
                    float vechi = ac->getSalariu();
                    string n = ac->getNume();
                    if(op==1) ac->setSalariu(vechi+100.0f); else ac->setSalariu(vechi>=100?vechi-100:vechi);
                    cout << "\"" << (!n.empty()?n:"?") << "\": " << vechi << " -> " << ac->getSalariu() << " RON" << endl;
                    librarie.setPersoane(pers);
                }
                for(Persoana* p : pers) delete p;
            } CATCH_EXCEPTII break;

            case 3: try{
                vector<Persoana*> pers = librarie.getPersoane();
                int nrPers=(int)pers.size();
                bool gasit = false;
                for(int i = 0; i < nrPers; i++){
                    if(AngajatCititor* ac = dynamic_cast<AngajatCititor*>(pers[i])){
                        cout << *ac << endl;
                        gasit = true;
                    }
                }
                if(!gasit) throw NotFoundError(0, "angajati-clienti");
                for(Persoana* p: pers) delete p;
                
            }CATCH_EXCEPTII break;

            case 4: try{
                vector<Persoana*> pers = librarie.getPersoane();
                int nrPers=(int)pers.size();

                bool gasit = false;
                for(int i = 0; i < nrPers; i++){
                    if(dynamic_cast<AngajatCititor*>(pers[i])){
                        string n = pers[i]->getNume();
                        cout << i << ". " << (!n.empty()?n:"?") << endl;
                        gasit = true;
                    }
                }
                if(!gasit){
                    for(Persoana* p: pers) delete p;
                    throw NotFoundError(0, "angajati-clienti");
                }
 
                int idxA; 
                cout << "Index-ul persoanei: "; 
                cin >> idxA;
                if(idxA < 0 || idxA >= nrPers){
                    for(Persoana* p: pers) delete p;
                    throw NotFoundError(idxA, "persoane"); 
                }
 
                AngajatCititor* ac = dynamic_cast<AngajatCititor*>(pers[idxA]);
                if(!ac){
                    for(Persoana* p: pers) delete p;
                    throw NotFoundError(idxA, "angajati-clienti");
                    break;
                }
 
                int nrCarti = librarie.getStoc().size();
                if(nrCarti == 0){
                    for(Persoana* p: pers) delete p;
                    throw NotFoundError(0, "stoc");
                }
 
                vector<Carte> stoc = librarie.getStoc();
                for(int i = 0; i < nrCarti; i++){
                    string t = stoc[i].getTitlu();
                    cout << i << ". " << (!t.empty()?t:"?") << " - " << stoc[i].getPret() << " RON" << endl;
                }
                int idxC; 
                cout << "Index-ul cartii: "; 
                cin >> idxC;
                if(idxC < 0 || idxC >= nrCarti){
                    for(Persoana* p: pers) delete p;
                    throw NotFoundError(idxC, "stoc");
                }
 
                float pretVechi = stoc[idxC].getPret();
                float pretNou   = ac->aplicaReducere(pretVechi);
                string t = stoc[idxC].getTitlu();
                cout << "Pret original: " << pretVechi << " RON" << endl;
                cout << "Pret redus pentru \"" << (!t.empty()?t:"?") << "\": " << pretNou << " RON" << endl;
                int idCarte = stoc[idxC].getId();
                // Adaugam cartea in istoricul angajatului cititor (prin upcasting Cititor)
                Cititor copieC = static_cast<Cititor&>(*ac);
                copieC = copieC + idCarte;
                ac->setCartiCumparate(copieC.getCartiCumparate());
                // Salvam array-ul actualizat inapoi in librarie
                librarie.setPersoane(pers);
                for(Persoana* p: pers) delete p;
                
            } CATCH_EXCEPTII break;
            
            case 5: try{
                Angajat a; 
                cin >> a;
                librarie.adaugaAngajat(a);
                cout << "Angajatul a fost adaugat!" << endl;
            }CATCH_EXCEPTII break;

            case 6: try{
                AngajatCititor ac; 
                cin >> ac;
                librarie.adaugaPersoana(new AngajatCititor(ac));
                cout << "Angajatul-client a fost adaugat!" << endl;
            } CATCH_EXCEPTII break;

            case 7: try{
                vector<Persoana*> pers = librarie.getPersoane();
                int nrPers=(int)pers.size();
                bool gasit = false;
                for(int i=0;i<nrPers;i++)
                    if(dynamic_cast<AngajatCititor*>(pers[i])){
                        string n = pers[i]->getNume();
                        cout << i << ". " << (!n.empty()?n:"?") << endl;
                        gasit = true;
                    }
                if(!gasit){
                    for(Persoana* p:pers) delete p; 
                    throw NotFoundError(0, "angajati-clienti"); 
                }
                int idxA;
                cout << "Index-ul persoanei: ";
                cin >> idxA;
                if(idxA < 0 || idxA >= nrPers || !dynamic_cast<AngajatCititor*>(pers[idxA])){
                    for(Persoana* p:pers) delete p;
                    throw NotFoundError(idxA, "angajati-clienti");
                }
                AngajatCititor* ac = dynamic_cast<AngajatCititor*>(pers[idxA]);
                int nr = ac->getCartiCumparate().size();
                if(nr == 0) cout << "Niciun istoric de cumparaturi." << endl;
                else {
                    cout << "Istoric cumparaturi (ID-uri carti): ";
                    vector<int> carti = ac->getCartiCumparate();
                    for(int i = 0; i < nr; i++) cout << carti[i] << " ";
                    cout << endl;
                }
                for(Persoana* p:pers) delete p;
            }CATCH_EXCEPTII break;
            
            case 8: try{
                Manager m;
                cin >> m;
                // Managerii sunt stocati polimorfic in persoane[] ca Persoana*
                // dynamic_cast ii va putea identifica ulterior ca Manager*
                librarie.adaugaPersoana(new Manager(m));
                cout << "Managerul a fost adaugat!" << endl;
            }CATCH_EXCEPTII break;

            case 9: try{
                vector<Persoana*> pers = librarie.getPersoane();
                int nrPers=(int)pers.size();
                bool gasit = false;
                for(int i = 0; i < nrPers; i++){
                    // dynamic_cast: preferat fata de static_cast deoarece verifica
                    // tipul real al obiectului la runtime
                    // static_cast ar putea casta gresit un AngajatCititor la Manager.
                    Manager* mg = dynamic_cast<Manager*>(pers[i]);
                    if(mg){
                        cout << *mg << endl;
                        gasit = true;
                    }
                }
                if(!gasit) throw NotFoundError(0, "manageri");
                for(Persoana* p:pers) delete p;
            }CATCH_EXCEPTII break;

            case 10: try{
                if(librarie.getAngajati().empty()) throw NotFoundError(0, "angajati");
                librarie.sorteazaAngajatiDupaSalariu();
                cout<<"Angajati sortati dupa salariu:"<<endl;
                for(const auto& a : librarie.getAngajati())
                    cout<<a.getNume()<<" - "<<a.getSalariu()<<" RON"<<endl;
            }CATCH_EXCEPTII break;
            
            default: cout<<"Optiune invalida!"<<endl;
        } 
    }
}