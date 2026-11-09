// =====================================================================
// app.cpp  -  PROGRAMMA che mostra tutti i requisiti   [GRUPPO B: composizione]
// Compilazione:  g++ -std=c++17 -Wall -Wextra Elemento.cc Contenitore.cc Coppia.cc app.cpp -o app
// (tieni Contenitore OPPURE Coppia: cancella l'altro, qui, negli include e il suo blocco R)
// Esecuzione:    ./app
//
// Prima si prova l'Elemento da solo, poi il Contenitore.
// I casi che devono fallire: ognuno nel suo try/catch, alla fine.
// =====================================================================

#include <iostream>
#include <stdexcept>
#include <vector>

#include "Elemento.h"
#include "Contenitore.h"
#include "Coppia.h"

int main()
{
    try
    {
        // R1: l'elemento da solo
        std::cout << "=== R1. Elemento ===\n";
        Elemento e(2.0);
        std::cout << e << "   a = " << e.a() << "   (atteso 2)\n\n";

        // R2: il contenitore
        std::cout << "=== R2. Contenitore ===\n";
        std::vector<Elemento> lista = {Elemento(1.0), Elemento(2.0), Elemento(3.0)};
        Contenitore c(lista);
        std::cout << c;                                             // operator<< -> print
        std::cout << "numero = " << c.numero() << "   (atteso 3)\n";
        std::cout << "elemento(1): " << c.elemento(1) << "   (atteso a = 2)\n";
        std::cout << "totale = " << c.totale() << "   (atteso 6)\n\n";

        // R3: la coppia (due elementi fissi)
        std::cout << "=== R3. Coppia ===\n";
        Coppia coppia(Elemento(1.0), Elemento(3.0));
        std::cout << coppia << "   (atteso totale 4)\n";
        coppia.setPrimo(Elemento(5.0));
        std::cout << "dopo setPrimo(5): totale = " << coppia.totale() << "   (atteso 8)\n\n";

        // (nuovi requisiti: copia qui i pezzi "NEL MAIN" del CATALOGO, uno per blocco)

        // Rn: casi che DEVONO fallire
        std::cout << "=== Rn. Eccezioni ===\n";
        try
        {
            Elemento errato(-1.0);
            std::cout << "ERRORE: doveva lanciare un'eccezione\n";
        }
        catch (const std::invalid_argument& ex)
        {
            std::cout << "Eccezione catturata: " << ex.what() << "\n";
        }

        try
        {
            c.elemento(100);
            std::cout << "ERRORE: doveva lanciare un'eccezione\n";
        }
        catch (const std::out_of_range& ex)
        {
            std::cout << "Eccezione catturata (indice): " << ex.what() << "\n";
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Errore: " << ex.what() << '\n';
        return 1;
    }
    return 0;
}
