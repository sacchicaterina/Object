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

private:
  /*
   * GESTIONE DEL CONTATORE STATICO instanceCount: due possibilita'.
   *
   * 1) VERSIONE CON inline (quella usata qui sotto, C++17+):
   *    static inline int instanceCount = 0;
   *    Dichiarazione E inizializzazione in un'unica riga, dentro la classe.
   *
   * 2) VERSIONE SENZA inline (stile classico, pre-C++17, valida ANCHE
   *    restando in un unico file .cpp come questo):
   *
   *      static int instanceCount;              // dentro la classe
   *      ...
   *      int Resistor::instanceCount = 0;        // fuori dalla classe
   *
   *    Questa seconda forma e' quella OBBLIGATORIA se il progetto e'
   *    diviso in Resistor.h e Resistor.cc: la dichiarazione va
   *    nell'header, la definizione va nel .cc.
   */
  static inline int instanceCount = 0;

public :
  
  Resistor(double r) : CircuitElement(r) { instanceCount++; }

  // costruttore di copia: necessario per il corretto funzionamento del
  // contatore (il costruttore di copia generato automaticamente dal
  // compilatore non saprebbe che deve incrementare instanceCount)
  Resistor(const Resistor& other) : CircuitElement(other) { instanceCount++; }

  ~Resistor() { instanceCount--; }

  static int getInstancesCount() { return instanceCount; }

  // "element" e' il dato ereditato dalla base CircuitElement: qui il
  // getter gli da' un nome specifico e chiaro (getResistor), anche se
  // internamente il dato si chiama sempre "element"
  double getResistor() const { return element; }

  void setResistor(double r) {
    if (r <= 0.0) {
            throw std::invalid_argument("Resistor value must be positive.");
        }
    element = r;
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
    return "Resistor r = " + std::to_string(element) + " ohm";
  }

};



class Inductor : public CircuitElement {

private:
  // Stesso discorso fatto per Resistor::instanceCount (vedi commento sopra).
  static inline int instanceCount = 0;

public :

  Inductor(double l) : CircuitElement(l) { instanceCount++; }

  Inductor(const Inductor& other) : CircuitElement(other) { instanceCount++; }

  ~Inductor() { instanceCount--; }

  static int getInstancesCount() { return instanceCount; }

  double getInductor() const { return element; }

  void setInductor(double l) {
    if (l <= 0.0) {
            throw std::invalid_argument("Inductor value must be positive.");
        }
    element = l;
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
    return "Inductor l = " + std::to_string(element) + "H";
  }

};



class Capacitor : public CircuitElement {

private:
  // Stesso discorso fatto per Resistor::instanceCount (vedi commento sopra).
  static inline int instanceCount = 0;

public :

  Capacitor(double c) : CircuitElement(c) { instanceCount++; }

  Capacitor(const Capacitor& other) : CircuitElement(other) { instanceCount++; }

  ~Capacitor() { instanceCount--; }

  static int getInstancesCount() { return instanceCount; }

  double getCapacitor() const { return element; }

  void setCapacitor(double c) {
    if (c <= 0.0) {
            throw std::invalid_argument("Capacitor value must be positive.");
        }
    element = c;
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
    return "Capacitor c = " + std::to_string(element) + "F";
  }

};




class RLCfilter {


protected:

  // R, L, C maiuscoli sono i NOMI DEGLI ATTRIBUTI membro
  // (non nomi di classi!): RLCfilter "ha-un" Resistor chiamato R,
  // "ha-un" Inductor chiamato L, "ha-un" Capacitor chiamato C.
  // Questo e' COMPOSIZIONE (has-a), non ereditarieta' (is-a):
  // RLCfilter non eredita da Resistor/Inductor/Capacitor, li contiene
  // come parti.
  Resistor R;
  Inductor L;
  Capacitor C;
  // NOTA DI DESIGN: omega_c, bandwidth, Q_factor NON sono piu' attributi
  // salvati (niente piu' "double omega_c; double bandwidth; double Q_factor;"
  // e niente piu' update() da richiamare ad ogni setter). Sono tutte
  // grandezze DERIVATE da R, L, C, quindi vengono ricalcolate al volo
  // tramite metodi ogni volta che servono: in questo modo non possono
  // MAI andare fuori sincrono rispetto a R, L, C, indipendentemente da
  // come/dove vengono modificati.

public:

  virtual ~RLCfilter() = default;

  // NOTA sulla initializer list: qui "R(r), L(l), C(c)" NON sono
  // chiamate a costruttori di classi base (non c'e' nessuna
  // ereditarieta' in gioco qui) -> sono inizializzazioni di ATTRIBUTI.
  // "R(r)" significa "inizializza l'attributo membro R (di tipo
  // Resistor) copiandolo dal parametro r", usando il costruttore di
  // copia di Resistor. Il parametro si chiama "r" minuscolo apposta,
  // per non confonderlo ne' con la classe Resistor ne' con l'attributo R.
  RLCfilter(const Resistor& r, const Inductor& l, const Capacitor& c) : R(r), L(l), C(c) {}

  Resistor getR() const { return R; }
  Inductor getL() const { return L; }
  Capacitor getC() const { return C; }

  // omega_c = 1/sqrt(LC): calcolato al volo, sempre coerente con L e C correnti
  double getOmegaC() const {
    //accedo ai valori numerici di L e C tramite la chiamata del metodo getter oggetto.metodo
    //essendo in un'altra classe dove non c'è ereditarietà si deve creare l'oggetto e accedere tramite getter
    return 1.0 / std::sqrt(L.getInductor() * C.getCapacitor());
  }

  // ogni topologia di filtro definisce la propria formula per la bandwidth;
  // rimane pura virtuale, ma ora RITORNA il valore invece di scriverlo
  // in un attributo
  virtual double getBandwidth() const = 0;

  // Q = omega_c / bandwidth: derivato a sua volta da altre due grandezze
  // derivate, calcolato al volo componendo le due chiamate sopra
  double getQualityFactor() const {
    return getOmegaC() / getBandwidth();
  }

  virtual std::string filterType() const = 0;

  // I setter ora si limitano a modificare il dato primario: non serve
  // piu' nessuna chiamata di "risincronizzazione" (update()), perche'
  // non c'e' piu' nulla da tenere sincronizzato manualmente.
  void setR(double r) { R.setResistor(r); }
  void setL(double l) { L.setInductor(l); }
  void setC(double c) { C.setCapacitor(c); }

};



class HighPass : public RLCfilter {

public:

  // Costruttore semplificato: nessuna chiamata a update() necessaria,
  // perche' non c'e' piu' nessuno stato derivato da inizializzare.
  // Qui invece "RLCfilter(r, l, c)" E' davvero la chiamata al
  // costruttore della classe BASE (RLCfilter) -> caso di ereditarieta',
  // diverso da "R(r)" visto sopra dentro RLCfilter stesso.
  HighPass(const Resistor& r, const Inductor& l, const Capacitor& c) : RLCfilter(r, l, c) {}

  // bandwidth = omega_c per un HighPass: ricalcolata ogni volta
  double getBandwidth() const override { return getOmegaC(); }

  std::string filterType() const override { return "HighPass"; }

};


class LowPass : public RLCfilter {

public:

  LowPass(const Resistor& r, const Inductor& l, const Capacitor& c) : RLCfilter(r, l, c) {}

  double getBandwidth() const override { return getOmegaC(); }

  // Il testo dell'esame dice esplicitamente che il LowPass "stores" il
  // damping factor zeta; qui lo interpretiamo mantenendo comunque il
  // dato "disponibile" per l'utente esterno tramite un getter dedicato,
  // ma calcolato al volo dalla formula (coerente con R, L, C correnti)
  // invece di salvarlo come attributo sincronizzato manualmente.
  double getDampingFactor() const {
    return (std::sqrt(L.getInductor() / C.getCapacitor())) / (2 * R.getResistor());
  }

  std::string filterType() const override { return "LowPass"; }

};



class BandPassSeries : public RLCfilter {

public:

  BandPassSeries(const Resistor& r, const Inductor& l, const Capacitor& c) : RLCfilter(r, l, c) {}

  double getBandwidth() const override { return R.getResistor() / L.getInductor(); }

  std::string filterType() const override { return "BandPassSeries"; }

};



class BandPassShunt : public RLCfilter {

public:

  BandPassShunt(const Resistor& r, const Inductor& l, const Capacitor& c) : RLCfilter(r, l, c) {}

  double getBandwidth() const override { return 1.0 / (R.getResistor() * C.getCapacitor()); }

  std::string filterType() const override { return "BandPassShunt"; }

};


// NOTA SU friend: questa funzione NON e' dichiarata "friend" dentro
// RLCfilter, e non ce n'e' bisogno. "friend" serve solo quando una
// funzione esterna deve accedere a membri private/protected bypassando
// l'incapsulamento. Qui invece operator<< usa SOLO metodi pubblici
// (filterType(), getR(), getL(), getC(), getOmegaC(), getBandwidth(),
// getQualityFactor()) esattamente come farebbe qualunque altro codice
// fuori dalla classe (es. il main()) -> quindi passa dall'interfaccia
// pubblica come tutti, senza privilegi speciali.
// friend servirebbe SOLO se si provasse ad accedere direttamente agli
// attributi protected R, L, C (es. "filt.R.getResistor()" invece di
// "filt.getR().getResistor()"): in quel caso, non essendo operator
// ne' un metodo membro ne' una derivata di RLCfilter, il compilatore
// darebbe errore senza una dichiarazione "friend" dentro la classe.
// In quel caso ipotetico, la dichiarazione andrebbe scritta dentro
// RLCfilter, tipo:
//   friend std::ostream& operator<<(std::ostream&, const RLCfilter&);
// IMPORTANTE: "friend" NON e' soggetto alle sezioni public/private/
// protected -> scriverla sotto "public:", "private:" o "protected:"
// e' ESATTAMENTE la stessa cosa, non cambia nulla nel comportamento.
// Metterla vicino ai public e' solo una convenzione di leggibilita',
// non un requisito del linguaggio.
// La funzione resterebbe comunque definita FUORI dalla classe come
// funzione libera: "friend" concede solo il permesso di accesso, non
// la rende un membro della classe.
// Passare sempre dai getter pubblici (come si fa qui) e' la scelta di
// design piu' pulita: mantiene l'incapsulamento pieno e rende
// operator<< indipendente dai dettagli interni della classe.
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

    //uso dell'overloading di <<
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
    // Nomi diversi (suffisso _p) per non confliggere con hp, lp, bps, bpsh gia' dichiarati sopra.
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
