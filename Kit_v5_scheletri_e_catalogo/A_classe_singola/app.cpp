// =====================================================================
// app.cpp  -  PROGRAMMA che mostra tutti i requisiti   [GRUPPO A: classe singola]
// Compilazione:  g++ -std=c++17 -Wall -Wextra Nome.cc app.cpp -o app
// Esecuzione:    ./app
//
// Un blocco per requisito (R1, R2, ...) con il risultato atteso calcolato a mano.
// I casi che devono fallire: ognuno nel suo try/catch, alla fine.
// Metodi -> oggetto.metodo(...)     Operatori -> come in matematica (a + b)
// =====================================================================

#include <iostream>    // std::cout, std::cerr
#include <stdexcept>   // std::exception e derivate (nei catch)

#include "Nome.h"      // un include per ogni classe usata (mai i .cc)

int main()
{
    try
    {
        // R1: costruttore regolare + stampa
        std::cout << "=== R1. Costruttore ===\n";
        Nome x("x", 3.0);
        std::cout << x << "   (atteso x: a = 3)\n\n";

        // R2: getter
        std::cout << "=== R2. Getter ===\n";
        std::cout << "a = " << x.a() << "   (atteso 3)\n\n";

        // R3: metodo di calcolo
        std::cout << "=== R3. Calcolo ===\n";
        std::cout << "calcolo = " << x.calcolo() << "   (atteso 0.5 * 9 = 4.5)\n\n";

        // R4: setter e metodo che modifica
        std::cout << "=== R4. Setter e modifica ===\n";
        x.setA(8.0);
        x.modifica();
        std::cout << x << "   (atteso x: a = 4)\n\n";

        // (nuovi requisiti: copia qui i pezzi "NEL MAIN" del CATALOGO, uno per blocco)

        // Rn: casi che DEVONO fallire (sempre per ultimi)
        std::cout << "=== Rn. Eccezioni ===\n";
        try
        {
            Nome errato("errato", -1.0);
            std::cout << "ERRORE: doveva lanciare un'eccezione\n";
        }
        catch (const std::invalid_argument& e)
        {
            std::cout << "Eccezione catturata: " << e.what() << "\n";
        }

        try
        {
            x.setA(0.0);
            std::cout << "ERRORE: doveva lanciare un'eccezione\n";
        }
        catch (const std::invalid_argument& e)
        {
            std::cout << "Eccezione catturata: " << e.what() << "\n";
        }
        std::cout << "dopo il setter fallito: " << x << "   (atteso a = 4, invariato)\n";
    }
    catch (const std::exception& e)       // rete di sicurezza: errori non previsti
    {
        std::cerr << "Errore: " << e.what() << '\n';
        return 1;
    }
    return 0;
}
