// =====================================================================
// Object Oriented Programming for Data Processing
// 14 September 2026
// Michela De Angelis 1797244
//
// Applicazione che chiede all'utente quanti modelli di gas simulare,
// li istanzia dinamicamente in base alla scelta dell'utente, li
// memorizza in un vector<GasModel*> e ne stampa i valori di DeltaU.
// =====================================================================

#include "gas_models.h"
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

int main()
{
    // --- Lettura del numero totale di modelli da simulare ---
    int numModels;
    std::cout << "Quanti modelli di gas vuoi simulare? ";
    std::cin >> numModels;

    std::vector<GasModel*> models;
    models.reserve(numModels);

    // --- Istanziazione dinamica di ciascun modello di gas ---
    for (int i = 1; i <= numModels; ++i)
    {
        std::cout << "\n--- Modello di gas #" << i << " ---\n";
        std::cout << "Scegli il tipo di gas:\n"
                  << "  1) Gas ideale\n"
                  << "  2) Gas con Cv dipendente dalla temperatura\n"
                  << "  3) Gas di van der Waals\n";

        int choice;
        std::cout << "Scelta: ";
        std::cin >> choice;

        // Finche' la scelta non e' 1, 2 o 3 viene richiesta di nuovo.
        while (choice != 1 && choice != 2 && choice != 3)
        {
            std::cout << "Scelta non valida, riprova.\n";
            std::cout << "Scelta: ";
            std::cin >> choice;
        }

        std::string label;
        std::cout << "Nome del gas: ";
        std::cin >> label;

        double Cv0;
        std::cout << "Cv0 [J/(mol*K)]: ";
        std::cin >> Cv0;

        double n;
        std::cout << "Numero di moli n [mol]: ";
        std::cin >> n;

        double T1;
        std::cout << "Temperatura iniziale T1 [K]: ";
        std::cin >> T1;

        double T2;
        std::cout << "Temperatura finale T2 [K]: ";
        std::cin >> T2;

        GasModel* model = nullptr;
        bool success = true;

        try
        {
            if (choice == 1)
            {
                model = new IdealGasModel(Cv0, n, label, T1, T2);
            }
            else if (choice == 2)
            {
                double beta;
                std::cout << "Coefficiente termico beta [J/(mol*K^2)]: ";
                std::cin >> beta;

                model = new TemperatureDependentGas(Cv0, n, label, T1, T2, beta);
            }
            else
            {
                double beta;
                std::cout << "Coefficiente termico beta [J/(mol*K^2)]: ";
                std::cin >> beta;

                double a;
                std::cout << "Parametro di van der Waals a [Pa*m^6/mol^2]: ";
                std::cin >> a;

                double Vm1;
                std::cout << "Volume molare iniziale Vm1 [m^3/mol]: ";
                std::cin >> Vm1;

                double Vm2;
                std::cout << "Volume molare finale Vm2 [m^3/mol]: ";
                std::cin >> Vm2;

                model = new VanDerWaalsGas(Cv0, n, label, T1, T2, beta, a, Vm1, Vm2);
            }
        }
        catch (const std::invalid_argument& e)
        {
            // Parametri fisicamente non validi: si scarta questo tentativo
            // e si ripete l'inserimento per lo stesso indice di modello.
            std::cout << "Errore: " << e.what() << " Riprova.\n";
            --i;
            success = false;
        }

        if (success)
        {
            models.push_back(model);
        }
    }

    // --- Stampa sequenziale dei risultati ---
    std::cout << "\n===== Risultati =====\n";
    for (const auto* model : models)
    {
        std::cout << "Gas \"" << model->getLabel() << "\": "
                  << "DeltaU = " << model->computeDeltaU() << " J\n";
    }

    // --- Deallocazione della memoria (i puntatori sono stati creati con new) ---
    for (auto* model : models)
    {
        delete model;
    }
    models.clear();

    return 0;
}
