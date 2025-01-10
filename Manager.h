#ifndef MANAGER_H //Manager.h ca sa nu fie inclus de mai multe ori, ifndef verifica daca nu a fost definit
#define MANAGER_H  //indic ca am definit Manager.h (guard pentru a nu include fisierul de mai multe ori si eventualel erori)
#include "Angajat.h"
#include <fstream>
#include <sstream>

class Manager : public Angajat { //mostenire( principiu OOP)
public:
    Manager( const string &nume, const string &prenume, const string &oraIntrare,
            const string &oraIesire) : Angajat(nume, prenume, oraIntrare, oraIesire) {} //mostenirea constructorului din clasa de baza

    void afisare() const override {
        cout << "Manager: " << endl;
        Angajat::afisare();
    } //suprascrierea metodei virtuale afisare

    void scrieInCsv(const string& oras) const override {
        try {
            string caleFisier = "./Testing/" + oras + "/angajati_" + oras + ".csv";

            ofstream fout(caleFisier, ios::app);
            if (!fout.is_open()) {
                throw runtime_error("Eroare la deschiderea fișierului pentru scriere.");
            }

            fout << "manager" << "," << nume << "," << prenume << "," << oraIntrare << "," << oraIesire << endl;
            fout.close();
        } catch (const runtime_error& e) {
            cerr << "Exceptie capturata: " << e.what() << endl;
        } catch (...) {
            cerr << "A avut loc o eroare necunoscută la scrierea în CSV." << endl;
        }
    }

    // modificarea citirii din CSV pentru a include functia, oraIntrare, oraIesire si zi
    void citesteDinCsv(const string& oras) const override {
        string caleFisier = "./Testing/" + oras + "/angajat_" + oras + ".csv";
        ifstream fin(caleFisier);
        if (fin.is_open()) {
            string linie;
            while (getline(fin, linie)) { // citirea linie cu linie
                istringstream ss(linie); // separ datele din linie
                string functieCSV, numeCSV, prenumeCSV, oraIntrareCSV, oraIesireCSV, ziCSV;

                getline(ss, functieCSV, ','); // citesc functia
                if (functieCSV == "manager") { // verific daca functia este 'manager'
                    getline(ss, numeCSV, ',');
                    getline(ss, prenumeCSV, ',');
                    getline(ss, oraIntrareCSV, ',');
                    getline(ss, oraIesireCSV, ',');
//                    getline(ss, ziCSV, ',');

                    // afișarea datelor citite
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
        return "manager";
    }
    double calculeazaSalariu() const override {
        return Angajat::calculeazaSalariu(); //apelez metoda din clasa de baza
    }
};

#endif
