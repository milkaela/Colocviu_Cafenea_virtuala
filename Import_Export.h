#ifndef IMPORT_EXPORT_H
#define IMPORT_EXPORT_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <iomanip>
#include <memory>
#include <stdexcept>
#include "Angajat.h"
#include "Comenzi.h"
#include "Evenimente.h"
#include "Produse.h"
#include "Raport.h"

using namespace std;

void afisareComenziInEngleza(const vector<Comanda>& comenzi) {
    //dictionar pentru traducerea produselor
    map<string, string> traduceriProduse = {
            {"Cacao", "Cocoa"},
            {"Cafea", "Coffee"},
            {"Cafea macinata", "Ground coffee"},
            {"Cappuccino", "Cappuccino"},
            {"Chec", "Cake"},
            {"Latte", "Latte"},
            {"Milkshake", "Milkshake"},
            {"Sirop", "Syrup"},
            {"Sanvis", "Sandwich"},
            {"Fursec", "Cookie"}
    };

    // afisez antetul
    cout << "name,product,quantity,total_price\n";

    for (const auto& comanda : comenzi) {
        cout << comanda.getNumeClient() << ",";

        // construiesc șirurile pentru produse și cantități
        string produseEngleza = "(";
        string cantitatiStr = "(";


        for (size_t i = 0; i < comanda.getProduseComandate().size(); ++i) {
            auto produs = comanda.getProduseComandate()[i].first;
            int cantitate = comanda.getProduseComandate()[i].second;

            string numeProdus = produs->getNume();
            string traducereProdus = numeProdus;

            // daca produsul e in dictionar, il traduc
            if (traduceriProduse.find(numeProdus) != traduceriProduse.end()) {
                traducereProdus = traduceriProduse[numeProdus];
            } else {
                cerr << "Avertisment: Numele produsului \"" << numeProdus << "\" nu are o traducere disponibilă.\n";
            }

            // adaug produsul si cantitatea la șiruri
            produseEngleza += traducereProdus;
            cantitatiStr += to_string(cantitate);

            if (i < comanda.getProduseComandate().size() - 1) {
                produseEngleza += ",";
                cantitatiStr += ",";
            }
        }

        produseEngleza += ")";
        cantitatiStr += ")";

        // afisez produsele si cantitatile
        cout << produseEngleza << "," << cantitatiStr << ",";
        cout << fixed << setprecision(2) << comanda.getPretTotalSalvat() << "\n";
    }
}

void afisareTraducereProduseCSV(const string& oras) {
    string caleFisier = "./Testing/" + oras + "/produse_" + oras + ".csv";

    ifstream fisierCitire(caleFisier);
    if (!fisierCitire) {
        cerr << "Eroare la deschiderea fisierului pentru citire: " << caleFisier << endl;
        return;
    }

    map<string, string> traduceriNume = {
            {"Cacao", "Cocoa"},
            {"Cafea", "Coffee"},
            {"Cafea macinata", "Ground coffee"},
            {"Cappuccino", "Cappuccino"},
            {"Chec", "Cake"},
            {"Latte", "Latte"},
            {"Milkshake", "Milkshake"},
            {"Sirop", "Syrup"},
            {"Sanvis", "Sandwich"},
            {"Fursec", "Cookie"}
    };

    string linie;
    if (getline(fisierCitire, linie)) {
        cout << "type,name,price,quantity\n";
    }

    while (getline(fisierCitire, linie)) {
        stringstream ss(linie);
        string tip, nume, pret, cantitate;

        getline(ss, tip, ',');
        getline(ss, nume, ',');
        getline(ss, pret, ',');
        getline(ss, cantitate, ',');

        if (tip == "ingredient") {
            tip = "ingredient";
        } else if (tip == "produs_finit") {
            tip = "finished_product";
        } else {
            cerr << "Eroare: Tip necunoscut la traducere pentru produsul: " << nume << "\n";
        }

        string numeTradus = nume;
        if (traduceriNume.find(nume) != traduceriNume.end()) {
            numeTradus = traduceriNume[nume];
        } else {
            cerr << "Avertisment: Numele produsului \"" << nume << "\" nu are o traducere disponibila.\n";
        }

        cout << tip << "," << numeTradus << "," << pret << "," << cantitate << "\n";
    }

    fisierCitire.close();
}

//  pentru raport
void afisareRaportTradus(const string& oras) {
    string caleFisier = "./Testing/" + oras + "/raport_" + oras + ".csv";

    ifstream fisierCitire(caleFisier);
    if (!fisierCitire) {
        cerr << "Eroare la deschiderea fisierului pentru citire: " << caleFisier << endl;
        return;
    }

    // Afișăm antetul tradus
    cout << "Date,Profit Orders,Cost Ingredients,Salaries Employees,Profit Events,Total Profit\n";

    string linie;
    getline(fisierCitire, linie); // ignor antetul
    while (getline(fisierCitire, linie)) {
        cout << linie << "\n"; // afisez restul liniei asa cum e pentru ca sunt doar date numerice
    }

    fisierCitire.close();
}


// pentru evenimente
void afisareEvenimenteTraduse(const string& oras) {
    string caleFisier = "./Testing/" + oras + "/evenimente_" + oras + ".csv";

    ifstream fisierCitire(caleFisier);
    if (!fisierCitire) {
        cerr << "Eroare la deschiderea fisierului pentru citire: " << caleFisier << endl;
        return;
    }

    cout << "event,date,total_cost,participants,price_per_participant,profit\n";

    string linie;
    getline(fisierCitire, linie); // ignor antetul
    getline(fisierCitire, linie); // ignor antetul
    while (getline(fisierCitire, linie)) {
        cout << linie << "\n";
    }

    fisierCitire.close();
}


//  pentru angajati
void afisareAngajatiTradusi(const string& oras) {
    string caleFisier = "./Testing/" + oras + "/angajati_" + oras + ".csv";

    ifstream fisierCitire(caleFisier);
    if (!fisierCitire) {
        cerr << "Eroare la deschiderea fisierului pentru citire: " << caleFisier << endl;
        return;
    }

    map<string, string> traduceriFunctii = {
            {"barista", "barista"},
            {"ospatar", "waiter"},
            {"manager", "manager"}
    };

    string linie;
    if (getline(fisierCitire, linie)) {
        cout << "position,name,surname,start_time,end_time\n";
    }

    while (getline(fisierCitire, linie)) {
        stringstream ss(linie);
        string functie, nume, prenume, oraStart, oraSfarsit;

        getline(ss, functie, ',');
        getline(ss, nume, ',');
        getline(ss, prenume, ',');
        getline(ss, oraStart, ',');
        getline(ss, oraSfarsit, ',');

        if (traduceriFunctii.find(functie) != traduceriFunctii.end()) {
            functie = traduceriFunctii[functie];
        } else {
            cerr << "Avertisment: Functia \"" << functie << "\" nu are o traducere disponibila.\n";
        }

        cout << functie << "," << nume << "," << prenume << "," << oraStart << "," << oraSfarsit << "\n";
    }

    fisierCitire.close();
}

#endif
