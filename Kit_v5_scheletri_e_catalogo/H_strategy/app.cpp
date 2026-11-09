// =====================================================================
// app.cpp  -  STRATEGY: l'integratore si sceglie nel main          [GRUPPO H]
// Compilazione:
//   g++ -std=c++17 -Wall -Wextra Integratore.cc Modello.cc Simulazione.cc app.cpp -o app
// Esecuzione:    ./app        (produce sim.dat: colonne t x v)
// =====================================================================

#include <iostream>
#include <stdexcept>
#include <string>
#include <cmath>

#include "Integratore.h"
#include "Modello.h"
#include "Simulazione.h"

// ===================== FUNZIONI DEL MAIN =====================
// (funzioni di servizio del programma: CATALOGO)

int main()
{
    try
    {
        // R1: parametri (qui fissi; da file: CATALOGO S2)
        std::string metodo = "Euler";
        double dt = 0.001;
        int N = 1000;
        double omega = 1.0, x0 = 1.0, v0 = 0.0;

        // R2: il sistema fisico e lo stato iniziale
        Modello sistema(omega);
        Stato u0 = {x0, v0};

        // R3: STRATEGY: l'algoritmo scelto per nome
        Euler euler;
        const Integratore* scelto = nullptr;     // puntatore alla BASE
        if (metodo == "Euler")
        {
            scelto = &euler;
        }
        else
        {
            throw std::invalid_argument("metodo sconosciuto: " + metodo);
        }

        // R4: simulazione e confronto con la soluzione esatta
        Simulazione sim(sistema, *scelto);
        Stato fine = sim.esegui(u0, dt, N, "sim.dat");
        double T = N * dt;
        std::cout << scelto->nome() << ": x(T) = " << fine[0]
                  << "   (esatto x0 cos(omega T) = " << x0 * std::cos(omega * T) << ")\n";
        std::cout << "scritto sim.dat\n\n";

        // (nuovi requisiti: copia qui i pezzi "NEL MAIN" del CATALOGO, uno per blocco)
    }
    catch (const std::exception& e)
    {
        std::cerr << "Errore: " << e.what() << '\n';
        return 1;
    }
    return 0;
}
