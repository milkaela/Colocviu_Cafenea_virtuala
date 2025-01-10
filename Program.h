#ifndef PROGRAM_H
#define PROGRAM_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "Tura.h"
using namespace std;

class ProgramTure {
private:
    vector<Tura> ture;  // Stochează turele pentru fiecare zi

public:

    // Adaugă o tură pentru o zi specifică
    void adaugaTura(const Tura& tura) {
        ture.push_back(tura);
    }

    // Afișează toate turele (pentru debug)
    void afisareProgram() const {
        for (const auto& tura : ture) {
            tura.afisareTura();
        }
    }

    void genereazaTure(const vector<string>& angajati, const vector<string>& zile, int angajatiPerTura) {
        int index = 0;
        int n = angajati.size();

        for (const auto& zi : zile) {
            Tura tura(zi, "08:00", "16:00");  // Presupunem că programul este de 8 ore (de la 8 la 16)

            for (int i = 0; i < angajatiPerTura; ++i) {
                tura.adaugaAngajat(angajati[index]);
                index = (index + 1) % n;  // Rotește angajații pentru ture
            }

            adaugaTura(tura);  // Adaugă tura generată
        }
    }

    // Funcție pentru scrierea turelor într-un fișier CSV
    void scrieTureInFisier(const string& folderOras, const string& numeFisier) const {
        for (const auto& tura : ture) {
            // Aici trebuie să adăugăm informațiile în fișier pentru fiecare angajat
            // Presupunem că pentru fiecare tură scriem o linie cu angajatul, ora de intrare, ora de ieșire și ziua
            ofstream fout(folderOras + "/" + numeFisier, ios::app);

            if (!fout.is_open()) {
                cerr << "Eroare la deschiderea fișierului!" << endl;
                return;
            }

            // Scrierea datelor despre tură pentru fiecare angajat
            for (const auto& angajat : tura.getAngajati()) {
                fout << angajat << "," << tura.getOraStart() << "," << tura.getOraSfarsit() << "," << tura.getZi() << endl;
            }

            fout.close();
        }
    }

    // Getter pentru ture (pentru a accesa din afacere dacă e necesar)
    const vector<Tura>& getTure() const {
        return ture;
    }
};

#endif //PROGRAM_H
