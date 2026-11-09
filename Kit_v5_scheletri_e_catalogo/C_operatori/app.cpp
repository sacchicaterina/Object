// =====================================================================
// app.cpp  -  PROGRAMMA che mostra tutti i requisiti     [GRUPPO C: operatori]
// Compilazione:  g++ -std=c++17 -Wall -Wextra Nome.cc app.cpp -o app
// Esecuzione:    ./app
//
// Metodi -> oggetto.metodo(...)  (a.a(), a.setA(5.0))
// Operatori -> come in matematica (a + b, a || b, 2.0 * a)
// =====================================================================

#include <iostream>
#include <stdexcept>

#include "Nome.h"

int main()
{
    try
    {
        // R1: costruttore + stampa
        std::cout << "=== R1. Costruttore ===\n";
        Nome a(2.0);
        Nome b(3.0);
        std::cout << "a: " << a << "   (atteso 2)\n";
        std::cout << "b: " << b << "   (atteso 3)\n\n";

        // R2: getter e setter
        std::cout << "=== R2. Getter e setter ===\n";
        std::cout << "a.a() = " << a.a() << "   (atteso 2)\n";
        Nome s(1.0);
        s.setA(5.0);
        std::cout << "dopo setA(5): " << s << "   (atteso 5)\n\n";

        // R3: operatori
        std::cout << "=== R3. Operatori ===\n";
        std::cout << "a + b = " << (a + b) << "   (atteso 5: con la TUA formula, calcolalo a mano)\n\n";

        // (nuovi requisiti: copia qui i pezzi "NEL MAIN" del CATALOGO, uno per blocco)

        // Rn: casi che DEVONO fallire
        std::cout << "=== Rn. Eccezioni ===\n";
        try
        {
            Nome errato(-1.0);
            std::cout << "ERRORE: doveva lanciare un'eccezione\n";
        }
        catch (const std::invalid_argument& e)
        {
            std::cout << "Eccezione catturata: " << e.what() << "\n";
        }

        try
        {
            s.setA(0.0);
            std::cout << "ERRORE: doveva lanciare un'eccezione\n";
        }
        catch (const std::invalid_argument& e)
        {
            std::cout << "Eccezione catturata: " << e.what() << "\n";
        }
        std::cout << "dopo il setter fallito: " << s << "   (atteso 5, invariato)\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "Errore: " << e.what() << '\n';
        return 1;
    }
    return 0;
}
