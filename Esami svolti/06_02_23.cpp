// app.cpp
//
// Esame di Computing Methods for Physics - 6 Febbraio 2023
// Parte 1 - C++
//
// NOTA DI DESIGN: sia Capacitor che CompositeCapacitor rappresentano
// la permittivita' relativa internamente come un vector<double>
// (epsRVec). Per Capacitor questo vettore ha sempre lunghezza 1 (un
// solo dielettrico); per CompositeCapacitor ha lunghezza N (uno per
// strato). Questo evita di dover inventare un valore fittizio per
// epsR quando CompositeCapacitor richiama il costruttore della base.

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
    double Q, dV, U;                // carica, differenza di potenziale, energia

    // Costruttore "grezzo": protected, riceve tutto gia' pronto, non fa
    // calcoli. epsRVec puo' avere lunghezza 1 (Capacitor) o N
    // (CompositeCapacitor) - stesso costruttore per entrambi i casi,
    // nessun valore fittizio necessario.
    Capacitor(double R1, double R2, const std::vector<double>& epsRVec,
              double Q, double dV, double U)
        : R1(R1), R2(R2), epsRVec(epsRVec), Q(Q), dV(dV), U(U) {}

public:
    // Capacita' geometrica di un singolo strato/dielettrico: dipende
    // solo da R1, R2, epsR. Resta statica: serve anche PRIMA che un
    // oggetto esista (nei costruttori).
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
        double C  = computeCapacitance(R1, R2, epsR);
        double dV = Q / C;
        double U  = 0.5 * Q * dV;
        return Capacitor(R1, R2, {epsR}, Q, dV, U);   // {epsR} = vettore di 1 elemento
    }

    // --- Named constructor 2: costruzione da deltaV ---
    static Capacitor fromVoltage(double dV, double R1, double R2, double epsR) {
        if (dV <= 0.0) {
            throw std::invalid_argument("dV deve essere positiva (coerente con Q > 0).");
        }
        double C = computeCapacitance(R1, R2, epsR);
        double Q = C * dV;
        double U = 0.5 * Q * dV;
        return Capacitor(R1, R2, {epsR}, Q, dV, U);
    }

    // --- Named constructor 3: costruzione da U ---
    static Capacitor fromEnergy(double U, double R1, double R2, double epsR) {
        if (U <= 0.0) {
            throw std::invalid_argument("U deve essere positiva (altrimenti Q non sarebbe > 0).");
        }
        double C  = computeCapacitance(R1, R2, epsR);
        double Q  = std::sqrt(2.0 * U * C);
        double dV = Q / C;
        return Capacitor(R1, R2, {epsR}, Q, dV, U);
    }

    // --- Getters ---
    double getR1() const { return R1; }
    double getR2() const { return R2; }
    double getEpsR() const { return epsRVec[0]; }   // Capacitor ha sempre 1 solo elemento
    double getCharge() const { return Q; }
    double getPotentialDifference() const { return dV; }
    double getEnergy() const { return U; }

    virtual double getCapacitance() const { return computeCapacitance(R1, R2, epsRVec[0]); }

    // --- Setters ---
    void setCharge(double Q_in) {
        if (Q_in <= 0.0) {
            throw std::invalid_argument("Q deve essere positiva (carica sull'armatura positiva).");
        }
        Q = Q_in;
        double C = getCapacitance();
        dV = Q / C;
        U  = 0.5 * Q * dV;
    }

    void setPotentialDifference(double dV_in) {
        if (dV_in <= 0.0) {
            throw std::invalid_argument("dV deve essere positiva (coerente con Q > 0).");
        }
        dV = dV_in;
        double C = getCapacitance();
        Q = C * dV;
        U = 0.5 * Q * dV;
    }

    void setEnergy(double U_in) {
        if (U_in <= 0.0) {
            throw std::invalid_argument("U deve essere positiva (altrimenti Q non sarebbe > 0).");
        }
        double C = getCapacitance();
        Q  = std::sqrt(2.0 * U_in * C);
        dV = Q / C;
        U  = U_in;
    }

    virtual void print() const {
        std::cout << "Capacitor: R1=" << R1 << " m, R2=" << R2 << " m, epsR=" << epsRVec[0]
                  << ", C=" << getCapacitance() << " F, Q=" << Q
                  << " C, dV=" << dV << " V, U=" << U << " J\n";
    }

    virtual ~Capacitor() = default;
};

// ---------------------------------------------------------------------
// Classe CompositeCapacitor
// ---------------------------------------------------------------------
class CompositeCapacitor : public Capacitor {
private:
    // Costruttore "grezzo": privato. Qui epsRVec (ereditato) ha
    // lunghezza N, non 1 - stesso identico costruttore della base,
    // nessun valore fittizio necessario.
    CompositeCapacitor(double R1, double R2, const std::vector<double>& epsRVec,
                        double Q, double dV, double U)
        : Capacitor(R1, R2, epsRVec, Q, dV, U) {}

public:
    // Versione statica: calcola Ctot dati R1, R2 e il vettore di N
    // permittivita' (una per strato). Serve nei costruttori, PRIMA che
    // un oggetto CompositeCapacitor esista.
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
        double C  = computeCompositeCapacitance(R1, R2, epsRVec);
        double dV = Q / C;
        double U  = 0.5 * Q * dV;
        return CompositeCapacitor(R1, R2, epsRVec, Q, dV, U);
    }

    // --- Named constructor 2: costruzione da deltaV ---
    static CompositeCapacitor fromVoltage(double dV, double R1, double R2,
                                           const std::vector<double>& epsRVec) {
        if (dV <= 0.0) {
            throw std::invalid_argument("dV deve essere positiva (coerente con Q > 0).");
        }
        double C = computeCompositeCapacitance(R1, R2, epsRVec);
        double Q = C * dV;
        double U = 0.5 * Q * dV;
        return CompositeCapacitor(R1, R2, epsRVec, Q, dV, U);
    }

    // --- Named constructor 3: costruzione da U ---
    static CompositeCapacitor fromEnergy(double U, double R1, double R2,
                                          const std::vector<double>& epsRVec) {
        if (U <= 0.0) {
            throw std::invalid_argument("U deve essere positiva (altrimenti Q non sarebbe > 0).");
        }
        double C  = computeCompositeCapacitance(R1, R2, epsRVec);
        double Q  = std::sqrt(2.0 * U * C);
        double dV = Q / C;
        return CompositeCapacitor(R1, R2, epsRVec, Q, dV, U);
    }

    // --- Getter sui singoli strati ---
    int getNumberOfLayers() const { return static_cast<int>(epsRVec.size()); }
    double getLayerEpsR(int i) const { return epsRVec[i]; }
    double getLayerInnerRadius(int i) const { return R1 + i * (R2 - R1) / getNumberOfLayers(); }
    double getLayerOuterRadius(int i) const { return R1 + (i + 1) * (R2 - R1) / getNumberOfLayers(); }
    double getLayerCapacitance(int i) const {
        return computeCapacitance(getLayerInnerRadius(i), getLayerOuterRadius(i), epsRVec[i]);
    }

    // Ridefinisce getCapacitance() di Capacitor (polimorfismo).
    double getCapacitance() const { return computeCompositeCapacitance(R1, R2, epsRVec); }

    void print() const {
        std::cout << "CompositeCapacitor: R1=" << R1 << " m, R2=" << R2
                  << " m, N=" << getNumberOfLayers() << " strati, C_tot=" << getCapacitance()
                  << " F, Q=" << Q << " C, dV=" << dV << " V, U=" << U << " J\n";
    }
};

// ---------------------------------------------------------------------
// main: applicazione dimostrativa (app.cpp)
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

    std::vector<double> epsRLayers = {1.0, 2.5, 4.0}; // 3 strati

    CompositeCapacitor comp1 = CompositeCapacitor::fromCharge(1.0e-9, 0.01, 0.02, epsRLayers);
    CompositeCapacitor comp2 = CompositeCapacitor::fromVoltage(5.0, 0.01, 0.02, epsRLayers);
    CompositeCapacitor comp3 = CompositeCapacitor::fromEnergy(2.0e-8, 0.01, 0.02, epsRLayers);

    std::cout << "-- fromCharge --\n"; comp1.print();
    std::cout << "-- fromVoltage --\n"; comp2.print();
    std::cout << "-- fromEnergy --\n"; comp3.print();

    std::cout << "\n-- dettaglio strati di comp1 --\n";
    for (int i = 0; i < comp1.getNumberOfLayers(); i++) {
        std::cout << "  strato " << i
                  << ": r_int=" << comp1.getLayerInnerRadius(i)
                  << " m, r_est=" << comp1.getLayerOuterRadius(i)
                  << " m, epsR=" << comp1.getLayerEpsR(i)
                  << ", C_i=" << comp1.getLayerCapacitance(i) << " F\n";
    }

    std::cout << "\n-- uso polimorfico: collezione eterogenea di condensatori --\n";
    std::vector<Capacitor*> capacitors;
    capacitors.push_back(&c1);      // Capacitor semplice
    capacitors.push_back(&comp1);   // CompositeCapacitor
    capacitors.push_back(&comp2);   // CompositeCapacitor
 
    double totalEnergy = 0.0;
    for (const auto& cap : capacitors) {
        cap->print();
        totalEnergy += cap->getEnergy();
    }
    std::cout << "Energia totale immagazzinata: " << totalEnergy << " J\n";
 
    // Stessa idea, ma con oggetti creati sull'heap: ogni "new" richiede
    // un "delete" corrispondente per non perdere memoria.
    std::cout << "\n-- uso polimorfico con new/delete --\n";
    std::vector<Capacitor*> dynamicCapacitors;
 
    Capacitor* simplePtr = new Capacitor(c2);
    dynamicCapacitors.push_back(simplePtr);
 
    CompositeCapacitor* compositePtr = new CompositeCapacitor(comp3);
    dynamicCapacitors.push_back(compositePtr);
 
    double totalEnergyDynamic = 0.0;
    for (const auto& cap : dynamicCapacitors) {
        cap->print();
        totalEnergyDynamic += cap->getEnergy();
    }
    std::cout << "Energia totale immagazzinata: " << totalEnergyDynamic << " J\n";
 
    for (const auto& cap : dynamicCapacitors) {
        delete cap;
    }
    dynamicCapacitors.clear();
 
    
}
 
