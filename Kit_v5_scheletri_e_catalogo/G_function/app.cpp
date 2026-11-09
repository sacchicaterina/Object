// =====================================================================
// app.cpp  -  PROGRAMMA che mostra tutti i requisiti                [GRUPPO G]
// Compilazione:  g++ -std=c++17 -Wall -Wextra Derivata.cc app.cpp -o app
//                (Function.h non ha un .cc: e' tutta nell'header)
// Esecuzione:    ./app
// =====================================================================

#include <iostream>
#include <stdexcept>
#include <cmath>

#include "Function.h"
#include "Derivata.h"

// ===================== FUNZIONI DEL MAIN =====================
// (funzioni di servizio del programma: CATALOGO G5)

int main()
{
    try
    {
        // R1: costruttore + stampa
        std::cout << "=== R1. Derivata ===\n";
        Derivata f("seno");
        std::cout << f << "   (atteso sin(1) = 0.841, cos(1) = 0.540)\n\n";

        // R2: polimorfismo con un riferimento alla classe del testo
        std::cout << "=== R2. Tramite la base ===\n";
        const Function& g = f;
        std::cout << g.name() << "(0) = " << g.value(0.0) << ", derivata in 0 = " << g.deriv(0.0) << "   (atteso 0 e 1)\n\n";

        // (nuovi requisiti: copia qui i pezzi "NEL MAIN" del CATALOGO, uno per blocco)
    }
    catch (const std::exception& e)
    {
        std::cerr << "Errore: " << e.what() << '\n';
        return 1;
    }
    return 0;
}
