#ifndef TURA_H
#define TURA_H
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

 class Tura {
    private:
        string zi;
        string oraIntrare;
        string oraIesire;
        vector<string> angajati;
    public:
     Tura(const string &zi, const string &oraIntrare, const string &oraIesire) :
     zi(zi),oraIntrare(oraIntrare),oraIesire(oraIesire) {}

     void adaugaAngajat(const string &numeAngajat) {
         angajati.push_back(numeAngajat);
     }
     void afisareTura() const {
            cout << "Zi: " << zi << endl;
            cout << "Ora intrare: " << oraIntrare << endl;
            cout << "Ora iesire: " << oraIesire << endl;
            cout << "Angajati: ";
            for (const auto &angajat : angajati) {
                cout << angajat << " ";
            }
            cout << endl;
     };
     void adaugaTuraInFisier(const string& folderOras, const string& numeFisier, const string& oraIntrare, const string& oraIesire, const string& zi) const {
         try {
             string caleFisier = folderOras + "/" + numeFisier;

             // Deschidem fișierul pentru a adăuga informațiile despre tură
             ofstream fout(caleFisier, ios::app);
             if (!fout.is_open()) {
                 throw runtime_error("Eroare la deschiderea fișierului pentru adăugarea turei.");
             }

             // Adăugăm informațiile despre tura respectivă
             fout << "Ora Intrare: " << oraIntrare << ", Ora Iesire: " << oraIesire << ", Zi: " << zi << "\n";

             fout.close(); // Închidem fișierul
         } catch (const runtime_error& e) {
             cerr << "Exceptie capturata: " << e.what() << endl;
         } catch (...) {
             cerr << "A avut loc o eroare necunoscută la adăugarea turei în fișier." << endl;
         }
     }


     // Getters pentru a accesa informațiile turei
     const vector<string>& getAngajati() const { return angajati; }
     string getOraStart() const { return oraIntrare; }
     string getOraSfarsit() const { return oraIesire; }
     string getZi() const { return zi; }
 };
#endif
