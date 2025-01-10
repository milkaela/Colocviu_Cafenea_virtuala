#ifndef BARISTA_H
#define BARISTA_H
#include "Angajat.h"
#include <fstream>
#include <sstream>

class Barista : public Angajat { // Mostenire
public:
    // Constructor
    Barista(const string& nume, const string& prenume, const string& oraIntrare, const string& oraIesire)
            : Angajat(nume, prenume, oraIntrare, oraIesire) {} // apel al constructorului din clasa de baza
    // suprascrierea metodei afisare
    void afisare() const override {
        cout << "Barista: " << endl;
        Angajat::afisare(); // Apel la afisare() din clasa de baza
    }

    // modificarea scrierii în CSV pentru a include functia, oraIntrare, oraIesire si zi
    void scrieInCsv(const string& oras) const override {
        try { //exceptii
            string caleFisier = "./Testing/" + oras + "/angajati_" + oras + ".csv";

            ofstream fout(caleFisier, ios::app);
            if (!fout.is_open()) {
                throw runtime_error("Eroare la deschiderea fișierului pentru scriere.");
            }

            // scriu datele in fisier
            fout << "barista" << "," << nume << "," << prenume << "," << oraIntrare << "," << oraIesire << endl;
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
            while (getline(fin, linie)) { //citesc fiecare linie
                istringstream ss(linie); // separ datele din linie
                string functieCSV, numeCSV, prenumeCSV, oraIntrareCSV, oraIesireCSV, ziCSV;

                getline(ss, functieCSV, ',');
                if (functieCSV == "barista") { // verific daca functia este 'barista'
                    getline(ss, numeCSV, ',');
                    getline(ss, prenumeCSV, ',');
                    getline(ss, oraIntrareCSV, ',');
                    getline(ss, oraIesireCSV, ',');

                    // Afișarea datelor citite
                    cout << "Citit din CSV: " << numeCSV << " " << prenumeCSV << " Ora intrare: " << oraIntrareCSV << " Ora iesire: " << oraIesireCSV << endl;
                }
            }
            fin.close();
        } else {
            cerr << "Eroare la deschiderea fișierului pentru citire." << endl;
        }
    }
    string getFunctie() const override {
        return "barista";
    }

    double calculeazaSalariu() const override {
        return Angajat::calculeazaSalariu(); //apelez metoda din clasa de baza
    }

};
#endif
