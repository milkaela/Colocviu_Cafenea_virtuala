#ifndef PRODUSE_H
#define PRODUSE_H

#include <iostream>
#include <regex>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <memory>
#include <vector>
#include <limits>
#include <algorithm>
#include <iomanip>
using namespace std;
// Clasa abstracta Produs
class Produs {
protected:
    string nume;
    double cantitate;
 //incapsulare pentru a nu putea fi accesate direct de la exterior
public:
    Produs(const string& nume, double cantitate) {
        this->nume = nume;
        this->cantitate = cantitate;
    }

    // metode virtuale pure (cu rol in polimorfism), trebuie implementate in clasele derivate
    virtual void afisare() const = 0;
    virtual string getTip() const = 0;
    virtual double getPret() const = 0; // metoda virtuala pentru pret care va fi diferita in functie de tipul produsului

    string getNume() const { return nume; }
    double getCantitate() const { return cantitate; }
    void setCantitate(double cantitateNoua) { cantitate = cantitateNoua; }

    virtual ~Produs() = default; //destructor virtual
};

//mostenire
class Ingredient : public Produs {
private:
    double pretAchizitie; // Prețul de achiziție pe unitate ca sa pot sa calculez profitul mai tarziu

public:
    Ingredient(const string& nume, double cantitate, double pretAchizitie): Produs(nume, cantitate) {
        this->nume = nume;
        this->cantitate = cantitate;
        this->pretAchizitie = pretAchizitie;
    }

    double getPret() const override { return pretAchizitie; }

    void afisare() const override {
        cout << "Ingredient: " << nume << ", Cantitate: " << cantitate << ", Pret achizitie: " << pretAchizitie << "/unitate\n";
    }

    string getTip() const override { //suprascriere pentru a returna tipul produsului
        return "ingredient";
    }
};

// clasa pentru ProdusFinit (derivata din Produs)
class ProdusFinit : public Produs {
private:
    double pretVanzare; // pret de vanzare ca sa pot sa calculez profitul mai tarziu

public:
    ProdusFinit(const string& nume, double cantitate, double pretVanzare) : Produs(nume, cantitate)
    {   this->nume = nume;
        this->cantitate = cantitate;
        this->pretVanzare = pretVanzare;
    }

    double getPret() const override { return pretVanzare; } //suprascriere pentru a returna pretul produsului
    void afisare() const override {
        cout << "Produs finit: " << nume << ", Cantitate: " << cantitate << ", Pret de vanzare: " << pretVanzare << "/unitate\n";
    }
    string getTip() const override { //suprascriere pentru a returna tipul produsului
        return "produs_finit";
    }
};

void citireDinFisier(vector<shared_ptr<Produs>>& produse, const string& oras) { //shared ptr e un smart pointer care poate stoca elemente intr o lista dinamica,
    // -,practic permite partajarea unor prooprietati ale unui obiect in mai multe locuri din cod, si se ocupa de dealocarea memoriei automat
    //& se foloseste pentru a evita copierea inutila a vectorului, si il modifica doar daca e necesar
    string caleFisier = "./Testing/" + oras + "/produse_" + oras + ".csv";

    ifstream fin(caleFisier);
    if (!fin.is_open()) {
        cerr << "Eroare: Nu s-a putut deschide fisierul pentru citire.\n";
        return;
    }

    string linie;
    // ca sa ignot antetul
    if (!getline(fin, linie)) {
        cerr << "Eroare: Fisierul este gol sau nu a putut fi citit.\n";
        return;
    }

    int linieIndex = 0; // index pentru identificarea erorilor

    while (getline(fin, linie)) {
        linieIndex++;
        stringstream ss(linie);
        string tip, nume, pretStr, cantitateStr;

        if (!getline(ss, tip, ',') || !getline(ss, nume, ',') || !getline(ss, pretStr, ',') || !getline(ss, cantitateStr, ',')) {
            cerr << "Eroare: Linia " << linieIndex << " nu are formatul corect.\n";
            continue;
        }

        // inlocuiesc virgula cu punctul in valorile numerice pentru ca e posibil sa se foloseasca virgula in loc d epunct pentru pret(fiind un tip de data double)
        pretStr = regex_replace(pretStr, regex(","), ".");
        cantitateStr = regex_replace(cantitateStr, regex(","), ".");

        double pret = 0, cantitate = 0;
        //exceptie pentru conversia pretului si cantitatii
        try {
            pret = stod(pretStr); //conversie string to double
            cantitate = stod(cantitateStr);
        } catch (const exception& e) {
            cerr << "Eroare: Conversia pretului sau cantitatii a esuat la linia " << linieIndex << ": " << e.what() << "\n";
            cerr << "Valori problematice - Pret: " << pretStr << ", Cantitate: " << cantitateStr << "\n";
            continue;
        }

        if (tip == "ingredient") {
            produse.push_back(make_shared<Ingredient>(nume, cantitate, pret)); //folosesc make_shared pentru ca asa isi da seama automat de tipul de data si
            // pentru a nu mai fi nevoie de delete mai tarziu, am citit despre asta aici: https://www.geeksforgeeks.org/std-make-shared-in-cpp/
        } else if (tip == "produs_finit") {
            produse.push_back(make_shared<ProdusFinit>(nume, cantitate, pret));
        } else {
            cerr << "Eroare: Tip necunoscut de produs la linia " << linieIndex << ": " << tip << "\n";
        }
    }

    fin.close();
}

void adaugaSauActualizeazaProdus(const string& oras, const Produs& produs) {
    string caleFisier = "./Testing/" + oras + "/produse_" + oras + ".csv";

    map<string, vector<string>> produse; // prctic pentru accesarea in functie de ingredient/produs finit(chei)

    ifstream fisierCitire(caleFisier);
    if (!fisierCitire) {
        cerr << "Eroare la deschiderea fisierului pentru citire: " << caleFisier << endl;
        return;
    }

    string linie;
    getline(fisierCitire, linie); // pt ignorarea antetului
    while (getline(fisierCitire, linie)) {
        stringstream ss(linie);
        string tip, nume, pret, cantitate;
        getline(ss, tip, ',');
        getline(ss, nume, ',');
        getline(ss, pret, ',');
        getline(ss, cantitate, ',');
        produse[nume] = {tip, pret, cantitate};
    }
    fisierCitire.close();

    string numeProdus = produs.getNume();
    if (produse.find(numeProdus) != produse.end()) {
        cout << "Produsul '" << numeProdus << "' exista deja. Doriti sa actualizati cantitatea sau pretul? (da/nu): ";
        string raspuns;
        cin >> raspuns;
        if (raspuns == "da") {
            double pretNou = produs.getPret();//preluam pretul si cantitatea din obiectul primit ca parametru
            double cantitateNoua = produs.getCantitate();
            produse[numeProdus][1] = to_string(pretNou); //actualizam pretul si cantitatea in map
            if (cantitateNoua < 0) {
                cerr << "Eroare: Cantitatea nu poate fi negativa.\n";
                return;
            } else{
                produse[numeProdus][2] = to_string(cantitateNoua);
            }

            cout << "Produsul a fost actualizat cu succes.\n";
        } else {
            cout << "Produsul nu a fost modificat.\n";
            return;
        }
    } else {
        string tip = produs.getTip();
        double pret = produs.getPret();//alocam pretul si cantitatea din obiectul primit ca parametru
        double cantitate = produs.getCantitate();
        produse[numeProdus] = {tip, to_string(pret), to_string(cantitate)}; //adaugam produsul in map

        cout << "Produsul a fost adaugat cu succes.\n";
    }

    ofstream fisierScriere(caleFisier);
    if (!fisierScriere) {
        cerr << "Eroare la deschiderea fisierului pentru scriere: " << caleFisier << endl;
        return;
    }

    fisierScriere << "tip,nume,pret,cantitate\n";
    for (const auto& [nume, detalii] : produse) {
        fisierScriere << detalii[0] << "," << nume << ","<< fixed << setprecision(2) << stod(detalii[1]) << ","<< fixed << setprecision(2) << stod(detalii[2]) << "\n"; // am folosit fixed ca sa pastrez un numar fix de zecimale dupa virgula si setprecision pentru a seta numarul de zecimale
    }

    fisierScriere.close();
}

void stergereProdus(const string& oras, const string& numeProdus) {
    string caleFisier = "./Testing/" + oras + "/produse_" + oras + ".csv";

    map<string, vector<string>> produse; // pentru a stoca produsele citite
    ifstream fisierCitire(caleFisier);

    if (!fisierCitire) {
        cerr << "Eroare la deschiderea fișierului pentru citire: " << caleFisier << endl;
        return;
    }

    string linie;
    getline(fisierCitire, linie);

    while (getline(fisierCitire, linie)) {
        stringstream ss(linie);
        string tip, nume, pret, cantitate;
        getline(ss, tip, ',');
        getline(ss, nume, ',');
        getline(ss, pret, ',');
        getline(ss, cantitate, ',');

        // adaug produsul in map
        produse[nume] = {tip, pret, cantitate};
    }

    fisierCitire.close();

    // verific daca produsul exista
    if (produse.find(numeProdus) != produse.end()) {
        //daca exista il sterg
        produse.erase(numeProdus); //erase e o metoda a mapului care sterge elementul cu cheia data
        cout << "Produsul/ingredientul '" << numeProdus << "' a fost sters.\n";
    } else {
        cout << "Produsul/ingredientul '" << numeProdus << "' nu a fost gasit.\n";
        return;
    }

    // scriu inapoi in fisier fara produsul sters
    ofstream fisierScriere(caleFisier);
    if (!fisierScriere) {
        cerr << "Eroare la deschiderea fisierului pentru scriere: " << caleFisier << endl;
        return;
    }

    // scriu antetul
    fisierScriere << "tip,nume,pret,cantitate\n";

    //pt restul produselor din map scriem in fisier
    for (const auto& [nume, detalii] : produse) {
        fisierScriere << detalii[0] << "," << nume << "," << detalii[1] << "," << detalii[2] << "\n"; // am folosit const auto& pentru a nu face copii inutile
    }

    fisierScriere.close();
}
#endif
