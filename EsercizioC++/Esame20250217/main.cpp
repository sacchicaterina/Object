#include "Polynomial.h"
#include "VectorField.h"
#include <iostream>
#include <vector>

using namespace std;

int main() {
  // 1. Creiamo i coefficienti per le tre componenti del campo
    // Fx = 1*x^2 (a4=1)
    vector<double> coeff_x = {0, 0, 6, 0, 1.0, 0, 6, 0, 5, 2};
    // Fy = 1*y^2 (a5=1)
    vector<double> coeff_y = {2, 3, 3, 0, 0, 1.0, 0, 0, 4, 0};
    // Fz = 1*z^2 (a6=1)
    vector<double> coeff_z = {0, 0, 0, 0, 3, 0, 1.0, 0, 7, 0};

    // 2. Creiamo gli oggetti Polynomial
    Polynomial Px("Componente X", coeff_x);
    Polynomial Py("Componente Y", coeff_y);
    Polynomial Pz("Componente Z", coeff_z);

    // 3. Creiamo il campo vettoriale passandogli gli INDIRIZZI (&)
    // Qui avviene il polimorfismo: Polynomial viene trattato come Function*
    VectorField campo(&Px, &Py, &Pz);

    // 4. Eseguiamo i calcoli in un punto di prova
    double x = 1.0, y = 4.0, z = 8.0;

    cout << "--- TEST CAMPO VETTORIALE ---" << endl;
    
    // Usiamo il metodo printVector che abbiamo progettato
    campo.printVector(x, y, z);

    return 0;
}
