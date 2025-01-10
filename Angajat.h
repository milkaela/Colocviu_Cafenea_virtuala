#ifndef ANGAJAT_H
#define ANGAJAT_H
#include <string>
#include <iostream>
#include <ctime> //pentru a putea folosi functiile de timp
#include <iomanip>
#include <sstream>

using namespace std;

class Angajat {
protected: // protected pentru a putea fi accesate de clasele derivate si sa nu fie accesate de alte clase
    string nume;
    string prenume;
    string oraIntrare;
    string oraIesire;

public:
    //constructor
    Angajat(const string &nume, const string &prenume, const string &oraIntrare, const string &oraIesire) { //folosesc const string& pentru a nu face copii inutile si a nu modifica obiectul
        this->nume = nume;
        this->prenume = prenume;
        this->oraIntrare = oraIntrare;
        this->oraIesire = oraIesire;
    }

    virtual ~Angajat() = default; //destructor virtual pentru a putea apela destructorul clasei derivate
    virtual string getFunctie() const = 0; // Metoda virtuala pura pentru a putea fi apelata de clasele derivate


    virtual void afisare() const { //metoda virtuala pentru a putea fi apelata de clasele derivate
        cout << "Nume: " << nume << endl;
        cout << "Prenume: " << prenume << endl;
        cout << "Ora intrare: " << oraIntrare << endl;
        cout << "Ora iesire: " << oraIesire << endl;
    };

    //metode virtuale pure
    virtual void scrieInCsv( const string &oras) const = 0;
    virtual void citesteDinCsv( const string& oras) const = 0;

    void modificaOra(const string& oraNouaIntrare, const string& oraNouaIesire) {
        oraIntrare = oraNouaIntrare;
        oraIesire = oraNouaIesire;
    }

    int calculeazaOreLucrate(const string &oraIntrare, const string &oraIesire) const {
        int oraIntrareInt, oraIesireInt;

        // Extrage orele din string
        stringstream ssIntrare(oraIntrare), ssIesire(oraIesire);
        ssIntrare >> oraIntrareInt;
        ssIesire >> oraIesireInt;

        // calculez diferenta dintre ore ca sa mi dea numarul de ore lucrate
        int diferentaOre = oraIesireInt - oraIntrareInt;

        return diferentaOre;
    }


    // metoda pentru calcularea salariului, am setat un salariu standard pentru fiecare functie a angajatului
    virtual double calculeazaSalariu() const {
        int oreLucrate = calculeazaOreLucrate(oraIntrare, oraIesire);
        double salariuPeOra = 0.0;

        if (getFunctie() == "barista") {
            salariuPeOra = 20.0;
        } else if (getFunctie() == "manager") {
            salariuPeOra = 30.0;
        } else if (getFunctie() == "ospatar") {
            salariuPeOra = 15.0;
        }

        return salariuPeOra * oreLucrate;
    }


    //getteri
    string getNume() const {
        return nume;
    }

    string getPrenume() const {
        return prenume;
    }

    string getOraIntrare() const {
        return oraIntrare;
    }

    string getOraIesire() const {
        return oraIesire;
    }
};
#endif
