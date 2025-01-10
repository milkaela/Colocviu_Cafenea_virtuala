#ifndef COMENZI_H
#define COMENZI_H

#include <iostream>
#include <string>
#include <vector> //template pentru a putea folosi vectori
#include <fstream>
#include <sstream>
#include <map>
#include <iomanip>
#include "Produse.h"

using namespace std;

class Comanda {
private:
    string numeClient; // Incapsulare: membru privat pentru a stoca numele clientului
    vector<pair<shared_ptr<Produs>, int>> produseComandate; // Incapsulare: lista dinamica de produse comandate si cantitatile acestora(retine mai multe proprietati ale obiectelor)
    double pretTotal; // Incapsulare: pretul total al comenzii
public:
    // Constructor doar pentru numele clientului, restul atributelor sunt completate ulterior
    Comanda() {
        numeClient = "Anonim"; // pentru cazul in care clientul nu isi introduce numele
    }

    //ca sa pot accesa produsele( am expicat in produse de ce folosesc acest tip de vector)
    const vector<pair<shared_ptr<Produs>, int>> &getProduseComandate() const { //getter pentru produsele comandate
        return produseComandate;
    }

    //getter
    string getNumeClient() const {
        return numeClient;
    }

//    // calculez pretul total al comenzii
//    double getPretTotal() const {
//        double total = 0;
//        for (const auto &[produs, cantitate]: produseComandate) {
//            total += produs->getPret() * cantitate;
//        }
//        return total;
//    }

    void setPretTotal(double pret) { // setter pentru preTul total
        pretTotal = pret;
    }

    double getPretTotalSalvat() const { // Ggtter pentru pretul total
        return pretTotal;
    }


    // aplic o reducere de 20% daca clientul este fidel (a facut mai mult de 5 comenzi)
    double aplicareReducereClientFidel(const string &numeClient, double pretTotal, const string &caleFisier) {
        int numarComenzi = numaraComenziClient(numeClient, caleFisier);
        if (numarComenzi >= 5) {
            cout << "Reducere de 10% aplicata pentru clientul fidel.\n";
            return pretTotal * 0.80;
        }
        return pretTotal;
    }

    // scriu comanda in fisier
    void scriereInFisier(const string &caleFisier, const string &numeClient,
                         const vector<pair<shared_ptr<Produs>, int>> &produseComandate, double pretTotal) {

        if (pretTotal <= 0.0) {
            cout << "Comanda nu poate fi procesata deoarece pretul total este 0.\n";
            return;
        } //pentru cazul in care am epuizat un stoc si nu mai am cum sa efectuez comanda, sa nu o mai scriu degeaba in fisier

        ofstream fout(caleFisier, ios::app); // deschide fisierul in modul de adaugare
        if (!fout) {
            cerr << "Eroare la deschiderea fisierului pentru scriere: " << caleFisier << endl;
            return;
        }

        fout << numeClient << ",("; // scriu numele clientului, dupa care pentru comanda de produse folosesc ()
        // ca sa mi fie mai usor sa delimitez campurile in cazul in care am o comanda mai mare

        // adaug numele produselor
        for (size_t i = 0;
             i < produseComandate.size(); ++i) { //cu prduseComandate.size() accesez numarul de produse comandate
            fout << produseComandate[i].first->getNume(); // cu asta accesez numele fiecarui produs din comanda
            if (i < produseComandate.size() - 1) { //daca nu sunt la ultimul produs, adaug o virgula
                fout << ",";
            }
        }

        fout << "),(";

        // adaug cantitatile produselor
        for (size_t i = 0; i < produseComandate.size(); ++i) {
            fout << produseComandate[i].second; // cu asta accesez cantitatea fiecarui produs din comanda ()
            if (i < produseComandate.size() - 1) {
                fout << ",";
            }
        }

        fout << ")," << fixed << setprecision(2) << pretTotal
             << "\n"; //aici setez numarul de zecimale dupa virgula, voiam sa fac cu %.2d dar asa e mai sigur

        fout.close();
    }

    static vector<Comanda> citireComenzi(
            const string &oras) { //am facut metoda sa fie statica pentru a nu fi nevoie de un obiect pentru a apela metoda
        vector<Comanda> comenzi; // lista de comenzi
        string caleFisier = "./Testing/" + oras + "/comenzi_" + oras + ".csv";
        ifstream fisier(caleFisier);

        if (!fisier.is_open()) {
            cerr << "Eroare la deschiderea fisierului: " << caleFisier << endl;
            return comenzi;
        }

        string linie;
        getline(fisier, linie); // ignor antetul

        // parcurgem restul liniilor
        while (getline(fisier, linie)) {
            string numeClient;
            vector<string> produse;
            vector<double> cantitati;
            string pretTotal;

            // extrag numele clientului
            size_t start = 0;
            size_t end = linie.find(','); // merg pana la prima virgula
            numeClient = linie.substr(start, end - start);

            // pentru produsr
            start = linie.find("(", end) + 1; // merg de la (
            end = linie.find(")", start);    // merg pana la )
            string produseStr = linie.substr(start, end - start); //ce e intre va fi lista de produse

            // acum sparg lista de produse dupa virgula
            stringstream produseStream(produseStr);
            string produs;
            while (getline(produseStream, produs, ',')) {
                produse.push_back(produs);
            }

            // procedez la fel si pentru cantitati
            start = linie.find("(", end) + 1;
            end = linie.find(")", start);
            string cantitatiStr = linie.substr(start, end - start);

            stringstream cantitatiStream(cantitatiStr);
            string cantitate;
            while (getline(cantitatiStream, cantitate, ',')) {
                try {
                    cantitati.push_back(stod(cantitate)); // convertesc  cantitatea la double
                } catch (const invalid_argument &e) {
                    cerr << "Eroare: Nu se poate converti '" << cantitate << "' in numar real.\n";
                    cantitati.push_back(0.0); //in caz de eroare o pun sa fie 0
                } catch (const out_of_range &e) {
                    cerr << "Eroare: Valoarea '" << cantitate << "' este in afara limitelor numarului real.\n";
                    cantitati.push_back(0.0);
                }
            }

            // extrag pretul total
            start = linie.find(",", end) + 1; //stim ca e dupa ultima virgula
            pretTotal = linie.substr(start); // Prețul total ramane un string

            // verfic daca dimensiunile listelor de produse si cantitati coincid
            if (produse.size() != cantitati.size()) {
                cerr << "Eroare: Dimensiunile listelor de produse și cantități nu coincid pentru clientul "
                     << numeClient << endl;
                continue;
            }

            Comanda comanda;
            //incep creearea comenzii
            comanda.numeClient = numeClient; //introduc numele clientului in comanda

            // acum introduc produsele in comanda
            for (size_t i = 0; i < produse.size(); ++i) {
                shared_ptr<Produs> produs = make_shared<ProdusFinit>(produse[i], cantitati[i],
                                                                     0.0); // Prețul unitar nu contează
                comanda.produseComandate.push_back({produs, cantitati[i]});
            }

            double pret = 0;
            try {
                pret = stod(pretTotal); // convertesc string-ul la double
                comanda.setPretTotal(pret); //  prețul total al comenzii
            } catch (const invalid_argument &e) {
                cerr << "Eroare: Pretul total nu este valid: " << pretTotal << endl;
            }


            // adaug comanda in lista
            comenzi.push_back(comanda);
        }

        fisier.close();
        return comenzi; // si acum o returnez
    }

    // metoda pentru asigurarea calitatii de client fidel
    int numaraComenziClient(const string &numeClient, const string &caleFisier) {
        ifstream fisier(caleFisier);
        if (!fisier.is_open()) {
            cerr << "Eroare la deschiderea fisierului de comenzi.\n";
            return 0;
        }
        string linie;
        int numarComenzi = 0;

        // cu asta ignor antetul pentru ca primeam eroare la conversia lui
        getline(fisier, linie);

        // parcurg restul liniilor
        while (getline(fisier, linie)) {
            stringstream ss(linie);
            string numeClientFisier;
            getline(ss, numeClientFisier, ','); //iau fiecare nume de client din fisier si verific de cate ori apare
            if (numeClientFisier == numeClient) {
                numarComenzi++;
            }
        }

        fisier.close();
        return numarComenzi;
    }

    // metdoa ppentru gestionarea procesului de plasare a unei comenzi
    void creareComanda(vector<shared_ptr<Produs>> &produse, const string &oras) {
        string caleFisier = "./Testing/" + oras + "/comenzi_" + oras + ".csv";

        vector<pair<shared_ptr<Produs>, int>> produseComandate; // Lista de produse comandate
        double pretTotal = 0;

        // Citesc numele clientului
        string numeClient;
        cout << "Introduceti numele clientului: ";
        getline(cin, numeClient);

        while (true) {
            cout << "\n--- Produse disponibile ---\n";
            int index = 1;
            vector<shared_ptr<Produs>> produseFinite;
            for (const auto &produs: produse) {
                // verificam daca produsul este finit
                if (dynamic_cast<ProdusFinit *>(produs.get())) { //cu get accesez pointerul si cu dynamic cast verific daca unde indica pointerul e produs finit
                    produseFinite.push_back(produs);
                    cout << index++ << ". ";
                    produs->afisare(); // afisam produsul
                }
            }

            int alegereProdus;
            cout << "Introduceti numarul produsului pe care doriti sa-l comandati (0 pentru a termina): ";
            cin >> alegereProdus;

            if (alegereProdus == 0) {
                break; // iesc din bucla daca clientul nu mai doreste sa comande altceva
            }

            if (alegereProdus < 1 || alegereProdus > produseFinite.size()) {
                cout << "Alegere invalida. Incercati din nou.\n";
                continue;
            }

            int cantitate;
            cout << "Introduceti cantitatea pentru produsul selectat: ";
            cin >> cantitate;

            if (cantitate <= 0) {
                cout << "Cantitate invalida. Incercati din nou.\n";
                continue;
            }

            shared_ptr<Produs> produs = produseFinite[alegereProdus - 1];
            double cantitateNoua = produs->getCantitate() - cantitate; // Calculăm noul stoc

            // verific daca stocul este suficient
            if (cantitateNoua < 0) {
                cout << "Stoc insuficient. Cantitate disponibila: " << produs->getCantitate() << "\n";
                continue; // trebuie sa aleg alt produs
            }

            // reduc stocul produsul si il adaug la comanda
            produs->setCantitate(cantitateNoua);
            adaugaSauActualizeazaProdus(oras, *produs); // actualizez stocul in fisier

            produseComandate.push_back({produs, cantitate}); // adaugarea prdosului in fisier
            pretTotal += produs->getPret() * cantitate; // adaug pretul produsului la pretul total

            cout << "Produsul a fost adaugat in comanda. Mai doriti sa adaugati alte produse? (da/nu): ";
            string raspuns;
            cin >> raspuns;
            if (raspuns == "nu") {
                break;
            }
        }

        // aplic reducerea pentru clientii fideli daca e cazul
        pretTotal = aplicareReducereClientFidel(numeClient, pretTotal, caleFisier);
        setPretTotal(pretTotal); //setez pretul total ca sa il pot folosi ulterior in alte functii
        scriereInFisier(caleFisier, numeClient, produseComandate, pretTotal);

        cout << "Pret total pentru comanda: " << fixed << setprecision(2) << pretTotal << " RON\n";
    }

    double calculeazaCostTotalIngrediente(const string &oras) {
        string caleFisier = "./Testing/" + oras + "/produse_" + oras + ".csv";
        ifstream fisier(caleFisier);
        if (!fisier.is_open()) {
            cerr << "Eroare la deschiderea fisierului: " << caleFisier << endl;
            return 0;
        }

        string linie;
        double costTotal = 0;

        while (getline(fisier, linie)) {
            stringstream ss(linie);
            string tip, nume, pretStr, cantitateStr;

            getline(ss, tip, ',');
            getline(ss, nume, ',');
            getline(ss, pretStr, ',');
            getline(ss, cantitateStr, ',');

            // verific dacă produsul este ingredient
            if (tip == "ingredient") {
                double pret = stod(pretStr);       // convertesc prețul la double
                double cantitate = stod(cantitateStr); // convertesc cantitatea la double

                costTotal += pret * cantitate; // adaug la costul total
            }
        }

        fisier.close();
        return costTotal;
    }

    double calcularePretTotal(const string &oras) { //calculeaza valoarea totala a comenzilor
        string caleFisier = "./Testing/" + oras + "/comenzi_" + oras + ".csv";
        double pretTotal = 0;
        ifstream fisier(caleFisier);

        if (!fisier.is_open()) {
            cerr << "Nu se poate deschide fisierul: " << caleFisier << endl;
            return -1;
        }

        string linie;

        // ignor antetul
        getline(fisier, linie);

        while (getline(fisier, linie)) {
            stringstream ss(linie);
            vector<string> valori;
            string element;

            // despartim liniile dupa virgula
            while (getline(ss, element, ',')) {
                valori.push_back(element);
            }

            // stiu ca ultimul element este pretul total
            if (!valori.empty()) {
                string pretStr = valori.back(); // ultimul element, o iau de la coada ca sa mi iau pretul direct
                try {
                    pretTotal += stod(pretStr); // convertesc inapoi la double si adaug la pretul total
                } catch (const invalid_argument &e) {
                    cerr << "Eroare la conversia pretului: " << pretStr << endl;
                }
            }
        }

        fisier.close();
        return pretTotal;
    }

    double profitZilnic(const string &oras) {
        double pret_total = calcularePretTotal(oras);
        double cost_total = calculeazaCostTotalIngrediente(oras);
        return pret_total - cost_total;
    }

    void afisareComenzi(const vector<Comanda> &comenzi) {
        for (const auto &comanda: comenzi) {
            cout << "Nume: " << comanda.numeClient << endl;

            cout << "Produse:" << endl;

            for (const auto &produsCantitate: comanda.produseComandate) {
                shared_ptr<Produs> produs = produsCantitate.first;
                int cantitate = produsCantitate.second;

                // afisez doar numele produsului si cantitatea
                cout << "  - " << produs->getNume() << endl;
                cout << "Cantitate: " << cantitate << endl;
            }

            // afisez pretul total
            cout << "Pret total: " << fixed << setprecision(2) << comanda.getPretTotalSalvat() << " RON" << endl;


            cout << "-------------------------------------" << endl;
        }
    }
};

#endif //COMENZI_H
