#include <iostream>
#include <stdexcept>
#include <vector>
#include <array>
#include <cmath>



class CircuitElement {

protected:

  double element;

public:

  CircuitElement(double el) : element(el) {
    if (el <= 0.0) {
      throw std::invalid_argument("CircuitElement: value must be positive."); }
  }

  virtual ~CircuitElement() = default;

  virtual std::string describe() const = 0;

  
};



class Resistor : public CircuitElement {

  static inline int instanceCount = 0;

public :

  Resistor(double R) : CircuitElement(R) { instanceCount++; }

  Resistor(const Resistor& other) : CircuitElement(other) { instanceCount++; }
  
  ~Resistor() { instanceCount--; }

  static int getInstancesCount() { return instanceCount; }

  double getResistor() const { return element; }
  
  void setResistor(double R) {
    if (R <= 0.0) {
            throw std::invalid_argument("Resistor value must be positive.");
        }
    element = R;
  }

  Resistor operator+(const Resistor& res) const {
    double series = element + res.element;
    return Resistor(series);
  }

 Resistor operator||(const Resistor& res) const {
    double parallel = 1./(1./element + 1./res.element);
    return Resistor(parallel);
  }

  std::string describe() const {
    return "Resistor R = " + std::to_string(element) + " ohm";
  }

};



class Inductor : public CircuitElement {

   static inline int instanceCount = 0;

public :

  Inductor(double L) : CircuitElement(L) { instanceCount++; }

  Inductor(const Inductor& other) : CircuitElement(other) { instanceCount++; }

  ~Inductor() { instanceCount--; }

  static int getInstancesCount() { return instanceCount; }

  double getInductor() const { return element; }
  
  void setInductor(double L) {
    if (L <= 0.0) {
            throw std::invalid_argument("Inductor value must be positive.");
        }
    element = L;
  }

  Inductor operator+(const Inductor& res) const {
    double series = element + res.element;
    return Inductor(series);
  }

  Inductor operator||(const Inductor& res) const {
    double parallel = 1./(1./element + 1./res.element);
    return Inductor(parallel);
  }

   std::string describe() const {
    return "Inductor L = " + std::to_string(element) + "H";
  }

};



class Capacitor : public CircuitElement {

   static inline int instanceCount = 0;

public :

  Capacitor(double C) : CircuitElement(C) { instanceCount++; }

  Capacitor(const Capacitor& other) : CircuitElement(other) { instanceCount++; }

  ~Capacitor() { instanceCount--; }

  static int getInstancesCount() { return instanceCount; }

  double getCapacitor() const { return element; }
  
  void setCapacitor(double C) {
    if (C <= 0.0) {
            throw std::invalid_argument("Capacitor value must be positive.");
        }
    element = C;
  }

  Capacitor operator+(const Capacitor& res) const {
    double parallel = element + res.element;
    return Capacitor(parallel);
  }

  Capacitor operator||(const Capacitor& res) const {
    double series = 1./(1./element + 1./res.element);
    return Capacitor(series);
  }

   std::string describe() const {
    return "Capacitor C = " + std::to_string(element) + "F";
  }

};




class RLCfilter {

  
protected:

  Resistor R;
  Inductor L;
  Capacitor C;
  double omega_c;
  double bandwidth;
  double Q_factor;

  
public:

  virtual ~RLCfilter() = default;

  RLCfilter(const Resistor& r, const Inductor& l, const Capacitor& c) : R(r), L(l), C(c) {}

  Resistor getR() const { return R; }
  Inductor getL() const { return L; }
  Capacitor getC() const { return C; }

  double getOmegaC() const { return omega_c; }
  void setOmegaC(double om) { omega_c = om; }

  double getBandwidth() const { return bandwidth; }
  void setBandwidth(double bw) { bandwidth = bw; }

  double getQualityFactor() const { return Q_factor; }
  void setQualityFactor(double qf) { Q_factor = qf; }


  virtual void calculate_BW() = 0;

  virtual std::string filterType() const = 0;

  void update() {
    omega_c = 1.0 / std::sqrt(L.getInductor() * C.getCapacitor());
    calculate_BW();
    Q_factor = omega_c / bandwidth;
  }

  void setR(double r) {
    R.setResistor(r);
    update();
  }

  void setL(double l) {
    L.setInductor(l);
    update();
  }

  void setC(double c) {
    C.setCapacitor(c);
    update();
  }

};



class HighPass : public RLCfilter {

public:

  HighPass(const Resistor& r, const Inductor& l, const Capacitor& c) : RLCfilter(r, l, c) {
    update();
  }

  void calculate_BW() { bandwidth = omega_c; }

  std::string filterType() const { return "HighPass"; }

};


class LowPass : public RLCfilter {

private:

  double dampling_factor;

public:

  LowPass(const Resistor& r, const Inductor& l, const Capacitor& c) : RLCfilter(r, l, c) {
    update();
  }

  void calculate_BW() {
     bandwidth = omega_c;
     dampling_factor = (std::sqrt(L.getInductor() / C.getCapacitor())) / (2 * R.getResistor());
  }

  double getDampingFactor() const { return dampling_factor; } 

  std::string filterType() const { return "LowPass"; }

};


// operator<< scritto FUORI dalle classi, come funzione libera,
// dopo RLCfilter e LowPass (che gli servono entrambe)
std::ostream& operator<<(std::ostream& os, const RLCfilter& filt) {
    os << filt.filterType() << " ["
       << "R =" << filt.getR().getResistor()
       << " L =" << filt.getL().getInductor()
       << " C =" << filt.getC().getCapacitor()
       << " omega_c =" << filt.getOmegaC()
       << " bandwidth =" << filt.getBandwidth()
       << " Q =" << filt.getQualityFactor();

    const LowPass* lp = dynamic_cast<const LowPass*>(&filt);
    if (lp != nullptr) {
        os << " zeta =" << lp->getDampingFactor();
    }

    os << "]";

    return os;
}



class BandPassSeries : public RLCfilter {

public:

  BandPassSeries(const Resistor& r, const Inductor& l, const Capacitor& c) : RLCfilter(r, l, c) {
    update();
  }

  void calculate_BW() { bandwidth = R.getResistor() / L.getInductor(); }

  std::string filterType() const { return "BandPassSeries"; }

};



class BandPassShunt : public RLCfilter {

public:

  BandPassShunt(const Resistor& r, const Inductor& l, const Capacitor& c) : RLCfilter(r, l, c) {
    update();
  }

  void calculate_BW() { bandwidth = 1.0 / (R.getResistor() * C.getCapacitor()); }

  std::string filterType() const { return "BandPassShunt"; }

};





int main() {
  
    std::cout << "=== Test CircuitElement hierarchy ===\n\n";

    Resistor r1(100.0), r2(220.0);
    Inductor l1(1e-3), l2(2e-3);
    Capacitor c1(1e-6), c2(2e-6);

    std::cout << r1.describe() << std::endl;
    std::cout << l1.describe() << std::endl;
    std::cout << c1.describe() << std::endl;

    std::cout << "\nIstanze -> Resistor: " << Resistor::getInstancesCount()     //richiamo metodo statico
               << ", Inductor: " << Inductor::getInstancesCount()
               << ", Capacitor: " << Capacitor::getInstancesCount() << std::endl;

    Resistor rSeries = r1 + r2;
    Resistor rParallel = r1 || r2;
    std::cout << "\nR1 + R2 (serie): " << rSeries.describe() << std::endl;
    std::cout << "R1 || R2 (parallelo): " << rParallel.describe() << std::endl;

    Inductor lSeries = l1 + l2;
    Inductor lParallel = l1 || l2;
    std::cout << "L1 + L2 (serie): " << lSeries.describe() << std::endl;
    std::cout << "L1 || L2 (parallelo): " << lParallel.describe() << std::endl;

    Capacitor cParallel = c1 + c2;
    Capacitor cSeries = c1 || c2;
    std::cout << "C1 + C2 (parallelo): " << cParallel.describe() << std::endl;
    std::cout << "C1 || C2 (serie): " << cSeries.describe() << std::endl;

    std::cout << "\n=== Test RLCfilter e sottoclassi ===\n\n";

    Resistor r(50.0);
    Inductor l(10e-3);
    Capacitor c(100e-9);

    HighPass hp(r, l, c);
    LowPass lp(r, l, c);
    BandPassSeries bps(r, l, c);
    BandPassShunt bpsh(r, l, c);

    std::cout << "HighPass:        " << hp << std::endl;
    std::cout << "LowPass:         " << lp << std::endl;
    std::cout << "BandPassSeries:  " << bps << std::endl;
    std::cout << "BandPassShunt:   " << bpsh << std::endl;

    std::cout << "\nDamping factor del LowPass: "
               << lp.getDampingFactor() << std::endl;

    // Dimensione fissa e nota a compile-time (sempre 4 tipi di filtro):
    // std::array e' la scelta piu' corretta rispetto a un array C-style
    // o a uno std::vector, perche' comunica esplicitamente che la
    // collezione non deve crescere/restringersi.
    std::array<RLCfilter*, 4> filters = {&hp, &lp, &bps, &bpsh};

    for (const auto* f : filters) {
        std::cout << *f << "\n";
    }

    std::cout << "\n--- Cambio R con setR() e ricontrollo LowPass ---\n";
    lp.setR(200.0);
    std::cout << "LowPass dopo setR(200.0): " << lp << std::endl;

    std::cout << "\n=== Stessa cosa, ma con new invece di & ===\n\n";

    // Versione alternativa: puntatori creati direttamente con new.
    // Nomi diversi (suffisso _p) per non confliggere con hp, lp, bps, bpsh già dichiarati sopra.
    RLCfilter* hp_p = new HighPass(r, l, c);
    RLCfilter* lp_p = new LowPass(r, l, c);
    RLCfilter* bps_p = new BandPassSeries(r, l, c);
    RLCfilter* bpsh_p = new BandPassShunt(r, l, c);

    std::array<RLCfilter*, 4> filters_p = {hp_p, lp_p, bps_p, bpsh_p};

    for (const auto* f : filters_p) {
        std::cout << *f << "\n";
    }

    // ATTENZIONE: qui serve liberare manualmente la memoria,
    // altrimenti si ha un memory leak
    delete hp_p;
    delete lp_p;
    delete bps_p;
    delete bpsh_p;

    
    std::cout << "\n--- Test eccezione: valore non positivo ---\n";
    try {
        Resistor rBad(-10.0);
    } catch (const std::invalid_argument& e) {
        std::cout << "Eccezione catturata: " << e.what() << std::endl;
    }  
  
}
