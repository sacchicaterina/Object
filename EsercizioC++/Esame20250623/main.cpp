#include <iostream>
#include <vector>
#include "Triangle.h"
#include "RightAngleTriangle.h"
#include "Cone.h"

using namespace std;

int main() {
    // Creiamo un contenitore di "Shape" per dimostrare il polimorfismo
    // Questo è il modo più elegante di far vedere che l'ereditarietà funziona
    vector<Shape*> figure;

    cout << "--- CREAZIONE DELLE FIGURE ---" << endl;

    // 1. Un Triangolo generico
    figure.push_back(new Triangle("Triangolo Isoscele", 5.0, 5.0, 8.0));

    // 2. Un Triangolo Rettangolo usando il Named Constructor (da cateti)
    // Nota: usiamo la sintassi NomeClasse::MetodoStatico
    figure.push_back(new RightAngleTriangle(RightAngleTriangle::createFromLegs("Rettangolo dai Cateti", 3.0, 4.0)));

    // 3. Un Triangolo Rettangolo (da ipotenusa e cateto)
    figure.push_back(new RightAngleTriangle(RightAngleTriangle::createFromHypotenuse("Rettangolo da Ipotenusa", 10.0, 6.0)));

    // 4. Un Cono
    figure.push_back(new Cone("Cono Gelato", 3.0, 7.0));

    cout << "Costruzione completata.\n" << endl;

    // --- SHOWCASE ---
    cout << "--- ESECUZIONE DELLO SHOWCASE ---" << endl;
    for (Shape* f : figure) {
        f->print(); // Qui entra in gioco il polimorfismo!
        cout << "---------------------------------" << endl;
    }

    // --- PULIZIA MEMORIA ---
    // Importante: avendo usato 'new', dobbiamo liberare la memoria.
    // Qui serve il distruttore virtuale che abbiamo messo in Shape!
    for (Shape* f : figure) {
        delete f;
    }
    figure.clear();

    cout << "\nSimulazione terminata con successo." << endl;

    return 0;
}
