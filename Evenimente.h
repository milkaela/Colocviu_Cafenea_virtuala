#ifndef EVENIMENTE_H
#define EVENIMENTE_H

#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

class Eveniment {
private: //incapsulare
    string nume;
    string data;
    double cost_total;
    int nr_part;
    double pret_per_participant;
    double profit;

public:
    // Constructor
    Eveniment(const string& nume, const string& data, double cost_total, int nr_part, double pret_per_participant) {
        this->nume = nume;
        this->data = data;
        this->nr_part = nr_part;
        this->cost_total = cost_total;
        this->pret_per_participant = pret_per_participant;
        this->profit = 0.0; // profitul il calculez ulterior
    }

    // Destructor
    virtual ~Eveniment() = default;

    // Getteri
    string getNume() const { return nume; }
    string getData() const { return data; }
    int getNrPart() const { return nr_part; }
    double getCostTotal() const { return cost_total; }
    double getPretPerParticipant() const { return pret_per_participant; }
    double getProfit() const { return profit; }

    // setteri pentru actualizare
    void setNrPart(int nr_part) { this->nr_part = nr_part; }
    void setCostTotal(double cost_total) { this->cost_total = cost_total; }
    void setPretPerParticipant(double pret) { this->pret_per_participant = pret; }
    void setProfit(double profit) { this->profit = profit; }


    double calculeazaProfit( ) const {// metoda pentru calcularea profitului, folosesc const pentru a nu modifica datele
        double marja = pret_per_participant * 0.20; // imi iau o marja de 20% din pretul per participant, adica eu consider ca vor face si o consumatie de macar 20% din pretul per participant
        double venit = (pret_per_participant + marja) * nr_part; //venit total
        return venit - cost_total;
    }


    void afisare() const { //metoda de afisare a datelor
        cout << "Nume: " << nume << endl;
        cout << "Data: " << data << endl;
        cout << "Cost total: " << fixed << setprecision(2) << cost_total << " RON" << endl;
        cout << "Numar participanti: " << nr_part << endl;
        cout << "Pret per participant: " << fixed << setprecision(2) << pret_per_participant << " RON" << endl;
        cout << "Profit: " << fixed << setprecision(2) << profit << " RON" << endl;
    }



    // scriere evenimente in fisier
    void scriereInFisier(const string& oras) {
        double profit = calculeazaProfit();

        string caleFisier = "./Testing/" + oras + "/evenimente_" + oras + ".csv";
        ofstream fout(caleFisier, ios::app); // deschid fisierul in modul append ca sa pot adauga alte evenimente

        if (!fout) {
            cerr << "Eroare la deschiderea fisierului: " << caleFisier << endl;
            return;
        }

        // adaug evenimentul in fisier
        fout << nume << "," << data << "," << fixed << setprecision(2) << cost_total << "," << nr_part << "," << fixed << setprecision(2) << pret_per_participant << ","<< fixed << setprecision(2) << profit << "\n";  // Asigurăm că toate valorile sunt scrise cu 2 zecimale
    //folosesc fixed ca sa afisez un numar fix de zecimale (2) pe care le-am setat cu setprecision
        fout.close();
    }


    // metoda pentru citirea evenimentelor din fisier, am folosit static pentru a nu fi nevoie de un obiect pentru a apela metoda
    static vector<Eveniment> citireDinFisier(const string& oras) { //am folosit vector pentru a returna toate evenimentele, pentru ca o linie contine toate datele unui eveniment
        string caleFisier = "./Testing/" + oras + "/evenimente_" + oras + ".csv";
        vector<Eveniment> evenimente;
        ifstream fisier(caleFisier);

        if (!fisier) {
            cerr << "Eroare la deschiderea fisierului: " << caleFisier << endl;
            return evenimente;
        }

        string linie;
        getline(fisier, linie);  //ignor antetul, pentru ca imi dadea eroare la citirea datelor

        while (getline(fisier, linie)) {
            stringstream ss(linie);
            string nume, data;
            string costTotalStr, nrParticipantiStr, pretPerPersoanaStr, profitStr;

            // citesc fiecare valoare din campuri
            if (!getline(ss, nume, ',') || !getline(ss, data, ',') || !getline(ss, costTotalStr, ',') || !getline(ss, nrParticipantiStr, ',')
            || !getline(ss, pretPerPersoanaStr, ',') || !getline(ss, profitStr, ',')) {
                cerr << "Linie invalida in fisier: " << linie << endl;
                continue;
            }

            try { //exceptii pentru conversia datelor
                // convertesc datele din string in tipurile dorite
                double costTotal = stod(costTotalStr);
                int nrParticipanti = stoi(nrParticipantiStr);
                double pretPerPersoana = stod(pretPerPersoanaStr);
                double profit = stod(profitStr);

                // verific daca datele sunt valide
                if (nrParticipanti <= 0 || costTotal < 0 || pretPerPersoana <= 0) {
                    cerr << "Date invalide pentru eveniment: " << linie << endl;
                    continue;
                }

                // imi creez evenimentul si adaug si profitul calculat anterior
                Eveniment eveniment(nume, data, costTotal, nrParticipanti, pretPerPersoana);
                eveniment.setProfit(profit);

                // si acum il adaug in fisier
                evenimente.push_back(eveniment);

            } catch (const invalid_argument& e) {
                cerr << "Eroare la conversia datelor: " << linie << endl;
                continue;
            } catch (const out_of_range& e) {
                cerr << "Date prea mari pentru a fi convertite: " << linie << endl;
                continue;
            }
        }

        fisier.close();
        return evenimente;
    }

};

#endif //EVENIMENTE_H
