#include <iostream>
#include <cmath>
#include <stdexcept>

constexpr double EPS0 = 8.854e-12; // permittivita' del vuoto [F/m]

// Etichetta che indica cosa rappresenta il parametro "value" del costruttore
enum class Quantity { Charge, Voltage, Energy };

class Capacitor {
private:
    double R1, R2, epsR;
    double Q, dV, U;

    static double computeCapacitance(double R1, double R2, double epsR) {
        if (R1 <= 0.0)      throw std::invalid_argument("R1 deve essere positivo.");
        if (R2 <= 0.0)      throw std::invalid_argument("R2 deve essere positivo.");
        if (R2 <= R1)       throw std::invalid_argument("R2 deve essere maggiore di R1.");
        if (epsR < 1.0)     throw std::invalid_argument("epsR deve essere >= 1.");
        return 4.0 * M_PI * EPS0 * epsR * R1 * R2 / (R2 - R1);
    }

public:
    // Unico costruttore: "value" e' Q, dV o U a seconda di "type"
    Capacitor(double R1_in, double R2_in, double epsR_in, double value, Quantity type)
        : R1(R1_in), R2(R2_in), epsR(epsR_in) {
        double C = computeCapacitance(R1, R2, epsR);

        switch (type) {
            case Quantity::Charge:
                Q  = value;
                dV = Q / C;
                break;
            case Quantity::Voltage:
                dV = value;
                Q  = C * dV;
                break;
            case Quantity::Energy:
                if (value < 0.0) throw std::invalid_argument("U non puo' essere negativa.");
                Q  = std::sqrt(2.0 * value * C);
                dV = Q / C;
                break;
        }
        U = 0.5 * Q * dV;
    }

    double getR1() const { return R1; }
    double getR2() const { return R2; }
    double getEpsR() const { return epsR; }
    double getCharge() const { return Q; }
    double getPotentialDifference() const { return dV; }
    double getEnergy() const { return U; }
    double getCapacitance() const { return computeCapacitance(R1, R2, epsR); }

    void print() const {
        std::cout << "Capacitor: R1=" << R1 << " m, R2=" << R2 << " m, epsR=" << epsR
                  << ", C=" << getCapacitance() << " F, Q=" << Q
                  << " C, dV=" << dV << " V, U=" << U << " J\n";
    }
};

int main() {
   
    Capacitor c1(0.01, 0.02, 1.0, 1.0e-9, Quantity::Charge);
    Capacitor c2(0.01, 0.02, 2.5, 5.0,    Quantity::Voltage);
    Capacitor c3(0.01, 0.02, 1.0, 2.0e-8, Quantity::Energy);

    std::cout << "-- da Q --\n"; c1.print();
    std::cout << "-- da dV --\n"; c2.print();
    std::cout << "-- da U --\n"; c3.print();

}





// =====================================================================
// OPZIONE A — 3 costruttori C++ "veri", distinti tramite overload.
//
// PROBLEMA: i costruttori "da Q", "da dV" e "da U" avrebbero tutti la
// stessa firma (double, double, double, double), perche' il compilatore
// distingue gli overload SOLO in base al TIPO dei parametri, non ai
// loro nomi (Q_in, dV_in, U_in sono etichette solo per noi umani).
// Con 3 firme identiche il codice non compila ("cannot be overloaded").
//
// SOLUZIONE: il costruttore da Q resta con la firma naturale
// (double, double, double, double). Per gli altri due (dV e U), che
// altrimenti avrebbero la STESSA firma tra loro, si aggiunge un
// parametro "char mode" davanti, cosi' la sequenza di tipi diventa
// diversa (char, double, double, double, double) e il compilatore
// li accetta. All'interno, un if/else distingue 'V' da 'U' a runtime.
//
// E' meno elegante del Named Constructor Idiom (opzione B), ma e'
// l'unica delle 3 opzioni che rispetta ALLA LETTERA la richiesta del
// testo "the class must provide 3 constructors".
// =====================================================================

#include <iostream>
#include <cmath>
#include <stdexcept>

constexpr double EPS0 = 8.854e-12; // permittivita' del vuoto [F/m]

class Capacitor {
private:
    double R1, R2, epsR;  // geometria e materiale
    double Q, dV, U;      // carica, differenza di potenziale, energia

    static double computeCapacitance(double R1, double R2, double epsR) {
        if (R1 <= 0.0)      throw std::invalid_argument("R1 deve essere positivo.");
        if (R2 <= 0.0)      throw std::invalid_argument("R2 deve essere positivo.");
        if (R2 <= R1)       throw std::invalid_argument("R2 deve essere maggiore di R1.");
        if (epsR < 1.0)     throw std::invalid_argument("epsR deve essere >= 1.");
        return 4.0 * M_PI * EPS0 * epsR * R1 * R2 / (R2 - R1);
    }

public:
    // --- Costruttore 1: da Q ---
    // Firma distinta dalle altre: il primo parametro e' un double,
    // che qui rappresenta direttamente la carica.
    Capacitor(double Q_in, double R1_in, double R2_in, double epsR_in)
        : R1(R1_in), R2(R2_in), epsR(epsR_in), Q(Q_in) {
        double C = computeCapacitance(R1, R2, epsR);
        dV = Q / C;
        U  = 0.5 * Q * dV;
    }

    // --- Costruttore 2 e 3: da dV oppure da U ---
    // Distinto dal precedente perche' il primo parametro e' un char,
    // non un double: il compilatore lo riconosce come overload diverso.
    // mode = 'V'  ->  value e' interpretato come dV
    // mode = 'U'  ->  value e' interpretato come U
    Capacitor(char mode, double value, double R1_in, double R2_in, double epsR_in)
        : R1(R1_in), R2(R2_in), epsR(epsR_in) {
        double C = computeCapacitance(R1, R2, epsR);

        if (mode == 'V') {
            dV = value;
            Q  = C * dV;
        } else if (mode == 'U') {
            if (value < 0.0) throw std::invalid_argument("U non puo' essere negativa.");
            Q  = std::sqrt(2.0 * value * C);
            dV = Q / C;
        } else {
            throw std::invalid_argument("mode deve essere 'V' oppure 'U'.");
        }
        U = 0.5 * Q * dV;
    }

    double getR1() const { return R1; }
    double getR2() const { return R2; }
    double getEpsR() const { return epsR; }
    double getCharge() const { return Q; }
    double getPotentialDifference() const { return dV; }
    double getEnergy() const { return U; }
    double getCapacitance() const { return computeCapacitance(R1, R2, epsR); }

    void print() const {
        std::cout << "Capacitor: R1=" << R1 << " m, R2=" << R2 << " m, epsR=" << epsR
                  << ", C=" << getCapacitance() << " F, Q=" << Q
                  << " C, dV=" << dV << " V, U=" << U << " J\n";
    }
};

int main() {

    Capacitor c1(1.0e-9, 0.01, 0.02, 1.0);        // da Q
    Capacitor c2('V', 5.0, 0.01, 0.02, 2.5);      // da dV
    Capacitor c3('U', 2.0e-8, 0.01, 0.02, 1.0);   // da U

    std::cout << "-- da Q --\n"; c1.print();
    std::cout << "-- da dV --\n"; c2.print();
    std::cout << "-- da U --\n"; c3.print();

}
