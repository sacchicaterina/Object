#include "RightAngleTriangle.h"
#include <iostream>
#include <cmath>

using namespace std;

// 1. IL COSTRUTTORE PRIVATO
// Richiama semplicemente il costruttore della classe base Triangle
RightAngleTriangle::RightAngleTriangle(const std::string& lbl, double a, double b, double c) 
    : Triangle(lbl, a, b, c) {
    // Non serve aggiungere altro qui, Triangle calcolerà già angoli e perimetro
}

// 2. NAMED CONSTRUCTOR: Da due cateti
RightAngleTriangle RightAngleTriangle::createFromLegs(const std::string& lbl, double leg1, double leg2) {
    // Calcoliamo l'ipotenusa: c = sqrt(a^2 + b^2)
    double hyp = sqrt(pow(leg1, 2) + pow(leg2, 2));
    
    // Restituiamo un oggetto creato con i tre lati completi
    return RightAngleTriangle(lbl, leg1, leg2, hyp);
}

// 3. NAMED CONSTRUCTOR: Da ipotenusa e un cateto
RightAngleTriangle RightAngleTriangle::createFromHypotenuse(const std::string& lbl, double hyp, double leg) {
    // Calcoliamo il cateto mancante: b = sqrt(c^2 - a^2)
    double other_leg = sqrt(pow(hyp, 2) - pow(leg, 2));
    
    // Restituiamo l'oggetto (l'ordine dei lati sarà: cateto1, cateto2, ipotenusa)
    return RightAngleTriangle(lbl, leg, other_leg, hyp);
}

// 4. DISTRUTTORE
RightAngleTriangle::~RightAngleTriangle() {}

// 5. PRINT
void RightAngleTriangle::print() const {
    cout << "--- TRIANGOLO RETTANGOLO ---" << endl;
    // Chiamiamo il print della classe base per mostrare i dettagli comuni
    Triangle::print(); 
    cout << "Nota: Questo triangolo ha un angolo retto (90 gradi)." << endl;
    cout << "----------------------------" << endl;
}
