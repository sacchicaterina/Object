// app.cpp - VERSIONE 1: minima ridondanza
//
// Esame di Computing Methods for Physics - 6 Febbraio 2023
// Parte 1 - C++
//
// Filosofia di questa versione: si salva solo il dato "primitivo"
// necessario (Q per lo stato elettrico, epsRVec per la geometria/
// materiale). Tutto il resto (dV, U, N) viene ricalcolato al volo
// ogni volta che serve, per evitare qualsiasi rischio di stati
// incoerenti tra dati derivati e dati salvati.

#include <iostream>
#include <vector>
#include <cmath>
#include <stdexcept>

constexpr double EPS0 = 8.854e-12; // permittivita' del vuoto [F/m]

// ---------------------------------------------------------------------
// Classe Capacitor
// ---------------------------------------------------------------------
class Capacitor {
protected:
    double R1, R2;                  // geometria
    std::vector<double> epsRVec;    // permittivita' relativa/e (lunghezza 1 qui)
    double Q;                       // UNICO dato elettrico salvato

    // Costruttore "grezzo": protected, riceve solo Q (non dV, non U -
    // si ricalcolano al volo quando servono).
    Capacitor(double R1, double R2, const std::vector<double>& epsRVec, double Q)
        : R1(R1), R2(R2), epsRVec(epsRVec), Q(Q) {}

public:
    static double computeCapacitance(double R1, double R2, double epsR) {
        if (R1 <= 0.0) {
            throw std::invalid_argument("R1 deve essere positivo.");
        }
        if (R2 <= 0.0) {
            throw std::invalid_argument("R2 deve essere positivo.");
        }
        if (R2 <= R1) {
            throw std::invalid_argument("R2 deve essere maggiore di R1.");
        }
        if (epsR < 1.0) {
            throw std::invalid_argument("epsR deve essere maggiore o uguale a 1.");
        }
        return 4.0 * M_PI * EPS0 * epsR * R1 * R2 / (R2 - R1);
    }

    // --- Named constructor 1: costruzione da Q ---
    static Capacitor fromCharge(double Q, double R1, double R2, double epsR) {
        if (Q <= 0.0) {
            throw std::invalid_argument("Q deve essere positiva (carica sull'armatura positiva).");
        }
        computeCapacitance(R1, R2, epsR);   // solo per validare la geometria
        return Capacitor(R1, R2, {epsR}, Q);
    }

    // --- Named constructor 2: costruzione da deltaV ---
    static Capacitor fromVoltage(double dV, double R1, double R2, double epsR) {
        if (dV <= 0.0) {
            throw std::invalid_argument("dV deve essere positiva (coerente con Q > 0).");
        }
        double C = computeCapacitance(R1, R2, epsR);
        double Q = C * dV;
        return Capacitor(R1, R2, {epsR}, Q);
    }

    // --- Named constructor 3: costruzione da U ---
    static Capacitor fromEnergy(double U, double R1, double R2, double epsR) {
        if (U <= 0.0) {
            throw std::invalid_argument("U deve essere positiva (altrimenti Q non sarebbe > 0).");
        }
        double C = computeCapacitance(R1, R2, epsR);
        double Q = std::sqrt(2.0 * U * C);
        return Capacitor(R1, R2, {epsR}, Q);
    }

    // --- Getters ---
    double getR1() const { return R1; }
    double getR2() const { return R2; }
    double getEpsR() const { return epsRVec[0]; }
    double getCharge() const { return Q; }

    virtual double getCapacitance() const { return computeCapacitance(R1, R2, epsRVec[0]); }

    // dV e U calcolati al volo, mai salvati
    double getPotentialDifference() const { return Q / getCapacitance(); }
    double getEnergy() const { return 0.5 * Q * getPotentialDifference(); }

    // --- Setters ---
    // Molto piu' semplici: nessun ricalcolo di grandezze derivate,
    // perche' quelle non sono mai salvate.
    void setCharge(double Q_in) {
        if (Q_in <= 0.0) {
            throw std::invalid_argument("Q deve essere positiva (carica sull'armatura positiva).");
        }
        Q = Q_in;
    }

    void setPotentialDifference(double dV_in) {
        if (dV_in <= 0.0) {
            throw std::invalid_argument("dV deve essere positiva (coerente con Q > 0).");
        }
        double C = getCapacitance();
        Q = C * dV_in;
    }

    void setEnergy(double U_in) {
        if (U_in <= 0.0) {
            throw std::invalid_argument("U deve essere positiva (altrimenti Q non sarebbe > 0).");
        }
        double C = getCapacitance();
        Q = std::sqrt(2.0 * U_in * C);
    }

    virtual void print() const {
        std::cout << "Capacitor: R1=" << R1 << " m, R2=" << R2 << " m, epsR=" << epsRVec[0]
                  << ", C=" << getCapacitance() << " F, Q=" << Q
                  << " C, dV=" << getPotentialDifference() << " V, U=" << getEnergy() << " J\n";
    }

    virtual ~Capacitor() = default;
};

// ---------------------------------------------------------------------
// Classe CompositeCapacitor
// ---------------------------------------------------------------------
class CompositeCapacitor : public Capacitor {
private:
    CompositeCapacitor(double R1, double R2, const std::vector<double>& epsRVec, double Q)
        : Capacitor(R1, R2, epsRVec, Q) {}

public:
    static double computeCompositeCapacitance(double R1, double R2,
                                                const std::vector<double>& epsRVec) {
        int N = static_cast<int>(epsRVec.size());
        double thickness = (R2 - R1) / N;
        double invCtot = 0.0;
        for (int i = 0; i < N; i++) {
            double ri_int = R1 + i * thickness;
            double ri_est = R1 + (i + 1) * thickness;
            double c_i = computeCapacitance(ri_int, ri_est, epsRVec[i]);
            invCtot += 1.0 / c_i;
        }
        return 1.0 / invCtot;
    }

    // --- Named constructor 1: costruzione da Q ---
    static CompositeCapacitor fromCharge(double Q, double R1, double R2,
                                          const std::vector<double>& epsRVec) {
        if (Q <= 0.0) {
            throw std::invalid_argument("Q deve essere positiva (carica sull'armatura positiva).");
        }
        computeCompositeCapacitance(R1, R2, epsRVec);   // valida geometria/vettore
        return CompositeCapacitor(R1, R2, epsRVec, Q);
    }

    // --- Named constructor 2: costruzione da deltaV ---
    static CompositeCapacitor fromVoltage(double dV, double R1, double R2,
                                           const std::vector<double>& epsRVec) {
        if (dV <= 0.0) {
            throw std::invalid_argument("dV deve essere positiva (coerente con Q > 0).");
        }
        double C = computeCompositeCapacitance(R1, R2, epsRVec);
        double Q = C * dV;
        return CompositeCapacitor(R1, R2, epsRVec, Q);
    }

    // --- Named constructor 3: costruzione da U ---
    static CompositeCapacitor fromEnergy(double U, double R1, double R2,
                                          const std::vector<double>& epsRVec) {
        if (U <= 0.0) {
            throw std::invalid_argument("U deve essere positiva (altrimenti Q non sarebbe > 0).");
        }
        double C = computeCompositeCapacitance(R1, R2, epsRVec);
        double Q = std::sqrt(2.0 * U * C);
        return CompositeCapacitor(R1, R2, epsRVec, Q);
    }

    // --- Getter sui singoli strati (N calcolato al volo da epsRVec.size()) ---
    int getNumberOfLayers() const { return static_cast<int>(epsRVec.size()); }
    double getLayerEpsR(int i) const { return epsRVec[i]; }
    double getLayerInnerRadius(int i) const { return R1 + i * (R2 - R1) / getNumberOfLayers(); }
    double getLayerOuterRadius(int i) const { return R1 + (i + 1) * (R2 - R1) / getNumberOfLayers(); }
    double getLayerCapacitance(int i) const {
        return computeCapacitance(getLayerInnerRadius(i), getLayerOuterRadius(i), epsRVec[i]);
    }

    double getCapacitance() const { return computeCompositeCapacitance(R1, R2, epsRVec); }

    void print() const {
        std::cout << "CompositeCapacitor: R1=" << R1 << " m, R2=" << R2
                  << " m, N=" << getNumberOfLayers() << " strati, C_tot=" << getCapacitance()
                  << " F, Q=" << Q << " C, dV=" << getPotentialDifference()
                  << " V, U=" << getEnergy() << " J\n";
    }
};

// ---------------------------------------------------------------------
// main
// ---------------------------------------------------------------------
int main() {
    std::cout.precision(4);
    std::cout << std::scientific;

    std::cout << "=== Capacitor ===\n\n";
    Capacitor c1 = Capacitor::fromCharge(1.0e-9, 0.01, 0.02, 1.0);
    Capacitor c2 = Capacitor::fromVoltage(5.0, 0.01, 0.02, 2.5);
    Capacitor c3 = Capacitor::fromEnergy(2.0e-8, 0.01, 0.02, 1.0);
    std::cout << "-- fromCharge --\n"; c1.print();
    std::cout << "-- fromVoltage --\n"; c2.print();
    std::cout << "-- fromEnergy --\n"; c3.print();

    std::cout << "\n=== CompositeCapacitor ===\n\n";
    std::vector<double> epsRLayers = {1.0, 2.5, 4.0};
    CompositeCapacitor comp1 = CompositeCapacitor::fromCharge(1.0e-9, 0.01, 0.02, epsRLayers);
    CompositeCapacitor comp2 = CompositeCapacitor::fromVoltage(5.0, 0.01, 0.02, epsRLayers);
    CompositeCapacitor comp3 = CompositeCapacitor::fromEnergy(2.0e-8, 0.01, 0.02, epsRLayers);
    std::cout << "-- fromCharge --\n"; comp1.print();
    std::cout << "-- fromVoltage --\n"; comp2.print();
    std::cout << "-- fromEnergy --\n"; comp3.print();

    std::cout << "\n-- uso polimorfico --\n";
    Capacitor* polyPtr = &comp1;
    std::cout << "Capacitanza via puntatore a base: " << polyPtr->getCapacitance() << " F\n";

    
}
