// =====================================================================
// app.cpp  -  PROGRAMMA che mostra tutti i requisiti  [GRUPPI D, E, F: gerarchie]
// Compilazione:  g++ -std=c++17 -Wall -Wextra Base.cc Derivata.cc app.cpp -o app
//                (aggiungi i .cc delle altre derivate)
// Esecuzione:    ./app
//
// POLIMORFISMO: un riferimento (o puntatore) alla BASE puo' indicare qualunque
// derivata; chiamando un metodo virtuale parte la versione della derivata VERA.
// =====================================================================

#include <iostream>
#include <stdexcept>

#include "Base.h"
#include "Derivata.h"

// ===================== FUNZIONI DEL MAIN =====================
// (funzioni di servizio del programma: CATALOGO D11)

int main()
{
    try
    {
        // R1: costruttore della derivata + stampa
        std::cout << "=== R1. Derivata ===\n";
        Derivata d("d", 3.0);
        std::cout << d << "   (atteso x = 3, calcolo = 9)\n\n";

        // R2: getter e setter (anche quelli ereditati dalla base)
        std::cout << "=== R2. Getter e setter ===\n";
        d.setX(4.0);
        d.setLabel("quadrato");                   // metodo della BASE, usato sulla derivata
        std::cout << d.label() << ": x = " << d.x() << ", calcolo = " << d.calcolo() << "   (atteso 4 e 16)\n\n";

        // R3: polimorfismo con un riferimento alla base
        std::cout << "=== R3. Polimorfismo ===\n";
        const Base& rif = d;                      // rif "vede" una Base, ma l'oggetto e' una Derivata
        std::cout << "tramite la base: " << rif << "   (stampa della Derivata)\n";
        std::cout << "calcolo = " << rif.calcolo() << "   (atteso 16)\n\n";

        // (nuovi requisiti: copia qui i pezzi "NEL MAIN" del CATALOGO, uno per blocco)

        // Rn: casi che DEVONO fallire
        std::cout << "=== Rn. Eccezioni ===\n";
        try
        {
            Derivata errata("errata", -1.0);
            std::cout << "ERRORE: doveva lanciare un'eccezione\n";
        }
        catch (const std::invalid_argument& e)
        {
            std::cout << "Eccezione catturata: " << e.what() << "\n";
        }
        // Base b("b");   // NON compila: la base e' astratta (ha virtuali puri)
    }
    catch (const std::exception& e)
    {
        std::cerr << "Errore: " << e.what() << '\n';
        return 1;
    }
    return 0;
}
