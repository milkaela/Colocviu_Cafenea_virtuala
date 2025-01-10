#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <sstream>
#include "Barista.h"
#include "Manager.h"
#include "Ospatar.h"
#include "Produse.h"
#include "Comenzi.h"
#include "Evenimente.h"
#include "Raport.h"
#include "Import_Export.h"

using namespace std;

int main() {

    // Lista orașelor disponibile
    vector<string> oraseDisponibile = {"Bucuresti", "Brasov", "Iasi", "Timisoara", "Cluj"};

    string oras;
    cout << "Pentru ce oras doriti sa faceti modificari/vizualizari?(Bucuresti, Brasov, Cluj, Iasi, Timisoara)\n";
    cin >> oras;

    // verific daca orasul este valid
    try {
        // Verificăm dacă orașul este valid
        if (find(oraseDisponibile.begin(), oraseDisponibile.end(), oras) != oraseDisponibile.end()) {
            cout << "Orasul " << oras << " este valid.\n";
        } else {
            throw invalid_argument("Orasul nu este valid.");
        }
    } catch (const invalid_argument& e) {
        cout << "Eroare: " << e.what() << "\n";
        return 1; // Ieșire cu cod de eroare
    }

    //pentru angajati am facut diferite modificari/ vizualizari direct aici, in main
    vector<unique_ptr<Angajat>> angajati; // vector folosit pentru a stoca angajatii si acces aleator la elemente prin indexare,
    // folosesc unique pentru a ma asigura ca nu sunt invalidate obiecte daca fac insertii sau stergeri(in timpul redimensionarii vectorului) si pentru ca asa sunt gestionate automat obiectele de acest tip

    // dupa ce citesc numele orasului setez calea pentru fisiere
    string caleFisier = "./Testing/" + oras + "/angajati_" + oras + ".csv";

    //citesc angajatii din fisier
    ifstream fin(caleFisier);
    if (fin.is_open()) {
        string linie;
        while (getline(fin, linie)) {
            stringstream ss(linie);
            string functie, nume, prenume, oraIntrare, oraIesire;

            // citesc datele din linie
            getline(ss, functie, ',');
            getline(ss, nume, ',');
            getline(ss, prenume, ',');
            getline(ss, oraIntrare, ',');
            getline(ss, oraIesire, ',');

            // creez obiectul in functie de functie
            if (functie == "barista") {
                angajati.push_back(make_unique<Barista>(nume, prenume, oraIntrare, oraIesire));
            } else if (functie == "manager") {
                angajati.push_back(make_unique<Manager>(nume, prenume, oraIntrare, oraIesire));
            } else if (functie == "ospatar") {
                angajati.push_back(make_unique<Ospatar>(nume, prenume, oraIntrare, oraIesire));
            }
        }
        fin.close();
    } else {
        cout << "Nu a fost gasit fisierul CSV pentru angajati.\n";
    }

    // Meniu principal
    while (true) {
        cout << "\nMeniu principal:\n";
        cout << "1. Gestionare angajati\n";
        cout << "2. Gestionare produse si comenzi\n";
        cout << "3. Gestionare evenimente\n";
        cout << "4. Generare raport\n";
        cout << "5. Iesire\n";
        cout << "Alegeti o optiune: ";

        int optiune;
        while (!(cin >> optiune) || optiune < 1 || optiune > 5) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Input invalid. Incercati din nou.\n";
            cout << "Alegeti o optiune (1-2): ";
        }

        cin.ignore();

        if (optiune == 1) {
            // gestionare angajati
            while (true) {
                cout << "\nGestionare angajati:\n";
                cout << "1. Afisare angajati (romana/english)\n";
                cout << "2. Adaugare angajat\n";
                cout << "3. Modificare program angajat\n";
                cout << "4. Stergere angajat\n";
                cout << "5. Revenire la meniul principal\n";
                cout << "Alegeti o optiune: ";

                int subOptiune;
                while (!(cin >> subOptiune) || subOptiune < 1 || subOptiune > 5) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(),
                               '\n'); //pentru ca nu se blocheze programul daca se introduce un caracter gresit, cu numeric limits ignor toate caracterele pana la \n
                    cout << "Input invalid. Incercati din nou.\n";
                    cout << "Alegeti o optiune (1-5): ";
                }

                cin.ignore();

                if (subOptiune == 1) {
                    cout << "1. Romana\n";
                    cout << "2. English\n" << endl;
                    int subOptiuneAfisare;
                    cin >> subOptiuneAfisare;
                    if (subOptiuneAfisare == 1) {
                        cout << "\nLista angajatilor existenti:\n";
                        for (const auto &angajat: angajati) {
                            angajat->afisare();
                            cout << endl;
                        }
                    } else if (subOptiuneAfisare == 2) {
                        cout << "\nList of existing employees:\n" << endl;
                        afisareAngajatiTradusi(oras);
                    } else {
                        cout << "Optiune invalida.\n";
                    }
                } else if (subOptiune == 2) {
                    string functie, nume, prenume, oraIntrare, oraIesire;
                    cout << "Functie (barista/manager/ospatar): ";
                    cin >> functie;
                    cout << "Nume: ";
                    cin >> nume;
                    cout << "Prenume: ";
                    cin >> prenume;
                    cout << "Ora intrare (HH:MM): ";
                    cin >> oraIntrare;
                    cout << "Ora iesire (HH:MM): ";
                    cin >> oraIesire;

                    if (functie == "barista") {
                        angajati.push_back(make_unique<Barista>(nume, prenume, oraIntrare, oraIesire));
                        cout << "Barista adaugat cu succes.\n";
                    } else if (functie == "manager") {
                        angajati.push_back(make_unique<Manager>(nume, prenume, oraIntrare, oraIesire));
                        cout << "Manager adaugat cu succes.\n";
                    } else if (functie == "ospatar") {
                        angajati.push_back(make_unique<Ospatar>(nume, prenume, oraIntrare, oraIesire));
                        cout << "Ospatar adaugat cu succes.\n";
                    } else {
                        cout << "Functie necunoscuta. Angajatul nu a fost adaugat.\n";
                    }
                } else if (subOptiune == 3) {
                    string numeCautat, prenumeCautat;
                    cout << "Introdu numele angajatului: ";
                    cin >> numeCautat;
                    cout << "Introdu prenumele angajatului: ";
                    cin >> prenumeCautat;

                    bool gasit = false;
                    for (auto &angajat: angajati) {
                        if (angajat->getNume() == numeCautat && angajat->getPrenume() == prenumeCautat) {
                            gasit = true;
                            string oraIntrareNoua, oraIesireNoua;
                            cout << "Introdu noua ora de intrare (HH:MM): ";
                            cin >> oraIntrareNoua;
                            cout << "Introdu noua ora de iesire (HH:MM): ";
                            cin >> oraIesireNoua;

                            angajat->modificaOra(oraIntrareNoua, oraIesireNoua);
                            cout << "Program modificat cu succes pentru " << numeCautat << " " << prenumeCautat
                                 << ".\n";
                            break;
                        }
                    }
                    if (!gasit) {
                        cout << "Angajatul nu a fost gasit.\n";
                    }
                } else if (subOptiune == 4) {
                    string numeCautat, prenumeCautat;
                    cout << "Introdu numele angajatului: ";
                    cin >> numeCautat;
                    cout << "Introdu prenumele angajatului: ";
                    cin >> prenumeCautat;

                    bool gasit = false;
                    for (auto it = angajati.begin(); it != angajati.end(); ++it) {
                        if ((*it)->getNume() == numeCautat && (*it)->getPrenume() == prenumeCautat) {
                            angajati.erase(it);
                            gasit = true;
                            cout << "Angajatul " << numeCautat << " " << prenumeCautat << " a fost sters.\n";
                            break;
                        }
                    }
                    if (!gasit) {
                        cout << "Angajatul nu a fost gasit.\n";
                    }
                } else if (subOptiune == 5) {
                    break;
                }
            }
            // salvarea angajaților în fișier
            ofstream fout(caleFisier);
            if (!fout.is_open()) {
                cerr << "Eroare la deschiderea fișierului pentru salvarea angajaților.\n";
                return 1;
            }
            for (const auto &angajat: angajati) {
                angajat->scrieInCsv(oras);
            }
            fout.close();
        } else if (optiune == 2) {
            while (true) {
                cout << "\nGestionare produse:\n";
                cout << "1. Afisare produse (romana/english)\n";
                cout << "2. Adaugare | Actualizare produse\n";
                cout << "3. Stergere produs\n";
                cout << "4. Comanda\n";
                cout << "5. Vizualizare comenzi(romana/english)\n";
                cout << "6. Costul total al ingredientelor\n";
                cout << "7. Revenire la meniul principal\n";
                cout << "Alegeti o optiune: ";
                vector<shared_ptr<Produs>> produse;
                citireDinFisier(produse, oras);

                int subOptiune;
                while (!(cin >> subOptiune) || subOptiune < 1 || subOptiune > 7) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Input invalid. Incercati din nou.\n";
                    cout << "Alegeti o optiune (1-4): ";
                }

                cin.ignore();

                if (subOptiune == 1) {
                    cout << "1. Romana\n";
                    cout << "2. English\n" << endl;
                    int subOptiuneAfisare;
                    cin >> subOptiuneAfisare;
                    if (subOptiuneAfisare == 1) {
                        cout << "\n--- Produse disponibile ---\n";
                        for (const auto &produs: produse) {
                            produs->afisare();
                        }
                    } else if (subOptiuneAfisare == 2) {
                        cout << "\n--- Available products ---\n";
                        afisareTraducereProduseCSV(oras);
                    } else {
                        cout << "Optiune invalida.\n";
                    }
                    // afisez produsele
                } else if (subOptiune == 2) {
                    cout << "1. Adaugati/actualizati un ingredient\n";
                    cout << "2. Adaugati/actualizati un produs finit\n";
                    int suboptiune;
                    cin >> suboptiune;

                    if (suboptiune == 1) {
                        string nume;
                        double cantitate, pret;

                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Introduceti numele ingredientului: ";
                        getline(cin, nume);
                        cout << "Introduceti cantitatea: ";
                        cin >> cantitate;
                        cout << "Introduceti pretul de achizitie: ";
                        cin >> pret;
                        Ingredient ingredient(nume, cantitate, pret);
                        adaugaSauActualizeazaProdus(oras, ingredient);
                        citireDinFisier(produse, oras);
                    } else if (suboptiune == 2) {
                        string nume;
                        double cantitate, pret;

                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Introduceti numele produsului: ";
                        getline(cin, nume);
                        cout << "Introdu cantitatea: ";
                        cin >> cantitate;
                        cin.ignore();
                        cout << "Introdu pretul de achizitie: ";
                        cin >> pret;
                        ProdusFinit produsFinit(nume, cantitate, pret);
                        adaugaSauActualizeazaProdus(oras, produsFinit);
                        citireDinFisier(produse, oras);


                    } else if (suboptiune == 2) {
                        string nume;
                        double cantitate, pret;

                        cout << "Introduceti numele produsului finit: ";
                        getline(cin, nume);
                        cout << "Introduceti cantitatea: ";
                        cin >> cantitate;
                        cout << "Introduceti pretul de vanzare: ";
                        cin >> pret;

                        ProdusFinit produsFinit(nume, cantitate, pret);
                        adaugaSauActualizeazaProdus(oras, produsFinit);
                        citireDinFisier(produse, oras); // actualizam vectorul de produse
                    } else {
                        cout << "Optiune invalida.\n";
                    }
                } else if (subOptiune == 3) {
                    // Ștergem un produs
                    string numeProdus;
                    cout << "Introduceti numele produsului pe care doriti sa-l stergeti: ";
                    cin >> numeProdus;

                    stergereProdus(oras, numeProdus);
                    citireDinFisier(produse, oras);
                } else if (subOptiune == 4) {
                    Comanda comand;
                    comand.creareComanda(produse, oras);
                } else if (subOptiune == 5) {
                    cout << "1. Romana\n";
                    cout << "2. English\n" << endl;
                    int subOptiuneAfisare;
                    cin >> subOptiuneAfisare;
                    if (subOptiuneAfisare == 1) {
                        vector<Comanda> comenzi = Comanda::citireComenzi(oras);
                        cout << "Comenzi existente:\n";
                        Comanda comanda;
                        comanda.afisareComenzi(comenzi);
                    } else {
                        cout << "Existing orders:\n";
                        vector<Comanda> comenzile = Comanda::citireComenzi(oras);
                        afisareComenziInEngleza(comenzile);
                    }
                }
                if (subOptiune == 6) {
                    Comanda comanda;
                    double cost = comanda.calculeazaCostTotalIngrediente(oras);
                    cout << "Costul total al ingredientelor este: " << cost << " RON\n";
                } else if (subOptiune == 7) {
                    break;
                }
            }
        } else if (optiune == 3) {
            vector<Eveniment> evenimente = Eveniment::citireDinFisier(oras);
            cout << "\nGestionare evenimente:\n";
            cout << "1. Adaugare eveniment\n";
            cout << "2. Afisare evenimente(romana/english)\n";
            cout << "3. Calcul profit total din evenimente\n";
            cout << "Alegeti o optiune: ";

            int subOpțiune;
            while (!(cin >> subOpțiune) || subOpțiune < 1 || subOpțiune > 3) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Input invalid. Incercati din nou.\n";
                cout << "Alegeti o optiune (1-3): ";
            }

            cin.ignore();

            if (subOpțiune == 1) {
                string nume, data;
                int nr_part;
                double cost_total, pretPerPersoana;
                cout << "Introduceti numele evenimentului: ";
                getline(cin, nume);
                cout << "Introduceti data evenimentului(ZZ.MM.YY): ";
                getline(cin, data);
                cout << "Introduceti costul total: ";
                cin >> cost_total;
                cout << "Introduceti numarul de participanti: ";
                cin >> nr_part;
                cout << "Introduceti pretul per persoana: ";
                cin >> pretPerPersoana;

                Eveniment eveniment(nume, data, cost_total, nr_part, pretPerPersoana);
                eveniment.scriereInFisier(oras);

                cout << "Eveniment adaugat cu succes.\n";
            } else if (subOpțiune == 2) {
                cout << "1. Romana\n";
                cout << "2. English\n" << endl;
                int subOptiuneAfisare;
                cin >> subOptiuneAfisare;

                if (subOptiuneAfisare == 1) {
                    cout << "Evenimente existente:\n";
                    for (const auto &eveniment: evenimente) {
                        eveniment.afisare();
                        cout << "-------------------------------------\n";
                    }
                } else {
                    cout << "Existing events:\n";
                    afisareEvenimenteTraduse(oras);
                }
            } else if (subOpțiune == 3) {
                double profitTotal = 0;
                for (const auto &eveniment: evenimente) { // Adaug const pentru ca nu modific obiectele
                    profitTotal += eveniment.getProfit(); // Folosim profitul citit din fișier sau setat
                }
                cout << "Profitul total din evenimente este: " << profitTotal << " RON\n";
            }
        } else if (optiune == 4) {
            Raport::generateReport(angajati, oras);
            cout << "1. Romana\n";
            cout << "2. English\n" << endl;
            int subOptiuneAfisare;
            cin >> subOptiuneAfisare;
            if (subOptiuneAfisare == 1) {
                cout << "Raport generat:\n";
                afiseazaRaportDinFisier(oras);
            } else {
                cout << "Generated report:\n";
                afisareRaportTradus(oras);
            }
        } else if (optiune == 5) {
            cout << "La revedere!\n";
            break;
        }
    }
    return 0;
}
