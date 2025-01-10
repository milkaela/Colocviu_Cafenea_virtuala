#ifndef RAPORT_H
#define RAPORT_H

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include <memory>
#include <map>
#include <chrono>
#include <set>
#include "Angajat.h"
#include "Comenzi.h"
#include "Produse.h"
#include "Evenimente.h"

using namespace std;

//Folosesc Factory method aici pentru ca daca se va schimba modul de calcul al profitului, nu va trebui sa modificam in toate locurile
// (e un proces complex de creeare a raportului, deci utilizarea acestui design pattern ar simplifica codul) unde se calculeaza profitul
//asa asigur o incapsulare mai buna si o mai buna organizare a codului
// am citit despre acest pattern aici: https://www.geeksforgeeks.org/factory-method-for-designing-pattern/
class Raport {
private:
    string data;
    double profitComenzi;
    double costIngrediente;
    double salariiAngajati;
    double profitEvenimente;
    double profitTotal;

public:
    // Constructor
    Raport(const string& data, double profitComenzi, double costIngrediente, double salariiAngajati, double profitEvenimente, double profitTotal)
    {
        this->data = data;
        this->profitComenzi = profitComenzi;
        this->costIngrediente = costIngrediente;
        this->salariiAngajati = salariiAngajati;
        this->profitEvenimente = profitEvenimente;
        this->profitTotal = profitTotal;
    }

    // Getteri
//    string getData() const { return data; }
//    double getProfitComenzi() const { return profitComenzi; }
//    double getCostIngrediente() const { return costIngrediente; }
//    double getSalariiAngajati() const { return salariiAngajati; }
//    double getProfitEvenimente() const { return profitEvenimente; }
//    double getProfitTotal() const { return profitTotal; }

    // Factory Method
    static Raport createRaport(const string& oras, const vector<unique_ptr<Angajat>>& angajati) {
        Comanda comanda;

        // calculez salariile angajaților
        double salariiTotale = 0.0;
        for (const auto& angajat : angajati) {
            salariiTotale += angajat->calculeazaSalariu();  // apelez functia din clasa Angajat
        }

        // calculez profitul zilnic pe comenzi
        double profitZilnicValoare = comanda.profitZilnic(oras);
        double costIngrediente = comanda.calculeazaCostTotalIngrediente(oras);
        double pretTotalComenzi = comanda.calcularePretTotal(oras);

        // profitul evenimentelor
        vector<Eveniment> evenimente = Eveniment::citireDinFisier(oras); //apelez metoda statica pentru citirea evenimentelor
        double profitEvenimente = 0;
        for (const auto& eveniment : evenimente) {
            profitEvenimente += eveniment.getProfit();
        }

        // profitul total
        double profitTotal = profitZilnicValoare - costIngrediente - salariiTotale + profitEvenimente;

        // obținerea datei curente
        auto now = chrono::system_clock::now();
        time_t currentTime = chrono::system_clock::to_time_t(now);
        tm* localTime = localtime(&currentTime);
        ostringstream oss;
        oss << std::put_time(localTime, "%Y-%m-%d");
        string dataRaport = oss.str();

        return Raport(dataRaport, pretTotalComenzi, costIngrediente, salariiTotale, profitEvenimente, profitTotal);
    }

    // metoda de scriere a raportului în fișier
    void scrieRaportInFisier(const string& oras) const {
        string caleFolder = "./Testing/" + oras;
        string caleFisier = caleFolder + "/raport_" + oras + ".csv";

        ofstream fisier(caleFisier, ios::app);
        if (!fisier.is_open()) {
            cerr << "Eroare la deschiderea fisierului: " << caleFisier << endl;
            return;
        }

        time_t now = chrono::system_clock::to_time_t(chrono::system_clock::now());
        tm* localTime = localtime(&now);
        char buffer[80];
        strftime(buffer, sizeof(buffer), "%d.%m.%Y", localTime);
        string dataFormatata(buffer);

        fisier << dataFormatata << "," << fixed << setprecision(2) << profitComenzi << "," << costIngrediente << "," << salariiAngajati << "," << profitEvenimente << "," << profitTotal << "\n";

        fisier.close();
    }


    static void generateReport(const vector<unique_ptr<Angajat>>& angajati, const string& oras) {
        // cream raportul folosind Factory Method
        Raport raport = Raport::createRaport(oras, angajati);
        // Scriem raportul în fișier
        raport.scrieRaportInFisier(oras);
    }

};
void afiseazaRaportDinFisier(const string& oras) {
    string caleFolder = "./Testing/" + oras;
    string caleFisier = caleFolder + "/raport_" + oras + ".csv";

    ifstream fisier(caleFisier);
    if (!fisier.is_open()) {
        cerr << "Eroare la deschiderea fisierului: " << caleFisier << endl;
        return;
    }

    cout << "Rapoartele pentru orasul " << oras << " sunt:\n";
    string linie;
    while (getline(fisier, linie)) {
        cout << linie << endl;
    }

    fisier.close();
}

#endif // RAPORT_H
