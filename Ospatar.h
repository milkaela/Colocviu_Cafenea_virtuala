#ifndef OSPATAR_H
#define OSPATAR_H
#include "Angajat.h"
#include <fstream>
#include <sstream>

class Ospatar : public Angajat { // Mostenire
public:
    // constructor actualizat pentru a include functia
    Ospatar( const string &nume, const string &prenume, const string &oraIntrare, const string &oraIesire): Angajat( nume, prenume, oraIntrare, oraIesire) {} // Pasarea parametrilor constructorului clasei de baza

    // Suprascrierea metodei afisare
    void afisare() const override {
        cout << "Ospatar: " << endl;
        Angajat::afisare(); // apel la afisare() din clasa de baza
    }

    // modificarea scrierii în CSV pentru a include functia, oraIntrare, oraIesire si zi
    void scrieInCsv(const string& oras) const override {
        try {
            string caleFisier = "./Testing/" + oras + "/angajati_" + oras + ".csv";

            ofstream fout(caleFisier, ios::app);
            if (!fout.is_open()) {
                throw runtime_error("Eroare la deschiderea fișierului pentru scriere.");
            }

            fout << "ospatar" << "," << nume << "," << prenume << "," << oraIntrare << "," << oraIesire << endl;
            fout.close();
        } catch (const runtime_error& e) {
            cerr << "Exceptie capturata: " << e.what() << endl;
        } catch (...) {
            cerr << "A avut loc o eroare necunoscută la scrierea în CSV." << endl;
        }
    }

    // modificarea citirii din CSV pentru a include functia, oraIntrare, oraIesire si zi
    void citesteDinCsv(const string& oras) const override {
        string caleFisier = "./Testing/" + oras + "/angajati_" + oras + ".csv";
        ifstream fin(caleFisier);
        if (fin.is_open()) {
            string linie;
            while (getline(fin, linie)) { // citesc linie cu linie
                istringstream ss(linie); // separ datele din linia citita
                string functieCSV, numeCSV, prenumeCSV, oraIntrareCSV, oraIesireCSV, ziCSV;

                getline(ss, functieCSV, ','); // Citirea functiei
                if (functieCSV == "ospatar") { // verific daca functia coincide
                    getline(ss, numeCSV, ',');
                    getline(ss, prenumeCSV, ',');
                    getline(ss, oraIntrareCSV, ',');
                    getline(ss, oraIesireCSV, ',');
//                    getline(ss, ziCSV, ',');

                    // afisare
                    cout << "Citit din CSV: " << numeCSV << " " << prenumeCSV
                         << " Ora intrare: " << oraIntrareCSV
                         << " Ora iesire: " << oraIesireCSV << endl;
//                         << " Ziua: " << ziCSV << endl;
                }
            }
            fin.close();
        } else {
            cerr << "Eroare la deschiderea fișierului pentru citire." << endl;
        }
    }
    string getFunctie() const override {
        return "ospatar";
    }

    double calculeazaSalariu() const override {
        return Angajat::calculeazaSalariu(); //apelez metoda din clasa de baza
    }
};
#endif
