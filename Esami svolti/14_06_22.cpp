class BlackHole {
  
private:
  
    double m;
    double chi;

    // Costruttore privato: riceve solo i 2 parametri fisici indipendenti,
    // gia' pronti. Non fa nessuna logica di "quale coppia mi hanno
    // passato": si limita ad assegnare e validare. Non accessibile
    // dall'esterno.
    BlackHole(double mVal, double chiVal) : m(mVal), chi(chiVal) {
        if (mVal <= 0.0) {
            throw std::invalid_argument("BlackHole: mass must be strictly positive.");
        }
        if (chiVal < 0.0 || chiVal > 1.0) {
            throw std::invalid_argument("BlackHole: chi must be in the range [0, 1].");
        }
    }

public:
    // --- Named constructor 1: da (m, chi) ---
    static BlackHole FromMassChi(double mVal, double chiVal) {
        return BlackHole(mVal, chiVal);
    }

    // --- Named constructor 2: da (m, mirr) ---
    static BlackHole FromMassMirr(double mVal, double mirrVal) {
        double x = 2.0 * mirrVal * mirrVal / (mVal * mVal) - 1.0;
        if (x < -1.0 || x > 1.0) {
            throw std::invalid_argument(
                "BlackHole: inconsistent m and mirr values (chi would be undefined).");
        }
        double chiVal = std::sqrt(1.0 - x * x);
        return BlackHole(mVal, chiVal);
    }

    // --- Named constructor 3: da (chi, mirr) ---
    static BlackHole FromChiMirr(double chiVal, double mirrVal) {
        double mVal = std::sqrt(2.0 * mirrVal * mirrVal / (1.0 + std::sqrt(1.0 - chiVal * chiVal)));
        return BlackHole(mVal, chiVal);
    }

    // --- Named constructor 4: da solo m (chi assunto 0) ---
    static BlackHole FromMass(double mVal) {
        return BlackHole(mVal, 0.0);
    }

    // --- Named constructor 5: da solo chi (m assunto 1) ---
    static BlackHole FromChi(double chiVal) {
        return BlackHole(1.0, chiVal);
    }

    // --- Costruttore di copia ---
    BlackHole(const BlackHole& other) : m(other.m), chi(other.chi) {}

    // ---- Getter (m e chi letti direttamente; mirr e EPenrose calcolati
    //      on-demand, cosi' non serve mai "ricordarsi" di aggiornarli) ----
    double GetMass() const { return m; }
    double GetSpin() const { return chi; }

    double GetIrreducibleMass() const { 
        return std::sqrt(m * m / 2.0 * (1.0 + std::sqrt(1.0 - chi * chi)));
    }

    double GetPenroseEnergy() const {
        return m - GetIrreducibleMass();
    }

    // ---- Setter (solo sui 2 parametri fisici indipendenti: m e chi) ----
    void SetMass(double mVal) {
        if (mVal <= 0.0) {
            throw std::invalid_argument("SetMass: mass must be strictly positive.");
        }
        m = mVal;
    }

    void SetSpin(double chiVal) {
        if (chiVal < 0.0 || chiVal > 1.0) {
            throw std::invalid_argument("SetSpin: chi must be in [0, 1].");
        }
        chi = chiVal;
    }

  void Print() const {
    std::cout << "Black hole parameters:" << std::endl;
    std::cout << "  m          = " << m << " Msun" << std::endl;
    std::cout << "  chi        = " << chi << std::endl;
    std::cout << "  m_irr      = " << GetIrreducibleMass() << " Msun" << std::endl;
    std::cout << "  E_Penrose  = " << GetPenroseEnergy() << " Msun" << std::endl;
  }

  
};

 

class BinaryBlackHole {


private:

  BlackHole bh1;
  BlackHole bh2;
  double theta1;
  double theta2;
  const double PI = std::acos(-1.0);
  double chi_eff;


public:

  BinaryBlackHole(double m1, double chi1, double th1, double m2, double chi2, double th2) :
    bh1(BlackHole::FromMassChi(m1, chi1)), bh2(BlackHole::FromMassChi(m2, chi2)), theta1(th1), theta2(th2) {

    if (th1 < 0.0 || th1 > PI) {
       throw std::invalid_argument("BinaryBlackHole: theta1 must be in [0, pi].");
    }
    if (th2 < 0.0 || th2 > PI) {
       throw std::invalid_argument("BinaryBlackHole: theta2 must be in [0, pi].");
    }

    chi_eff = (chi1 * m1 * cos(th1) + chi2 * m2 * cos(th2)) / (m1 + m2)

  }

  //costruttore di copia
  BinaryBlackHole(const BinaryBlackHole& other) : bh1(other.bh1), bh2(other.bh2), theta1(other.theta1), theta2(other.theta2) {}


  // ---- Getter ----

    // Accesso ai due black hole come oggetti interi (tutti i loro dati
    // sono già disponibili tramite i getter di BlackHole stesso)
    const BlackHole& GetFirstBlackHole() const { return bh1; }
    const BlackHole& GetSecondBlackHole() const { return bh2; }

    double GetTheta1() const { return theta1; }
    double GetTheta2() const { return theta2; }

    double GetEffectiveSpin() const { return chieff; }


    void Print() const {
    std::cout << "Binary black hole system:\n";

    std::cout << "-- Black hole 1 --\n";
    bh1.Print();   //print di Blackhole 

    std::cout << "-- Black hole 2 --\n";
    bh2.Print();

    std::cout << "-- Binary parameters --\n";
    std::cout << "  theta1  = " << theta1 << " rad\n";
    std::cout << "  theta2  = " << theta2 << " rad\n";
    std::cout << "  chi_eff = " << chieff << "\n";
}
               

};





int main() {

    std::cout << "=== Costruzione dei singoli black hole ===" << std::endl << std::endl;

    BlackHole bh1(BlackHole::FromMassChi(30.0, 0.7));
    std::cout << "bh1 (da massa e chi):" << std::endl;
    bh1.Print();
    std::cout << std::endl;

    BlackHole bh2(BlackHole::FromMassMirr(30.0, bh1.GetIrreducibleMass()));
    std::cout << "bh2 (da massa e mirr, deve coincidere con bh1):" << std::endl;
    bh2.Print();
    std::cout << std::endl;

    BlackHole bh3(BlackHole::FromChiMirr(0.7, bh1.GetIrreducibleMass()));
    std::cout << "bh3 (da chi e mirr, deve coincidere con bh1):" << std::endl;
    bh3.Print();
    std::cout << std::endl;

    BlackHole bh4(BlackHole::FromMass(20.0));
    std::cout << "bh4 (da sola massa, chi assunto 0):" << std::endl;
    bh4.Print();
    std::cout << std::endl;

    BlackHole bh5(BlackHole::FromChi(0.5));
    std::cout << "bh5 (da solo chi, massa assunta 1):" << std::endl;
    bh5.Print();
    std::cout << std::endl;

    BlackHole bh6(bh1);   // costruttore di copia
    bh6.SetSpin(0.9);
    std::cout << "bh6 (copia di bh1, poi modificato lo spin):" << std::endl;
    bh6.Print();
    std::cout << "bh1 non e' stato toccato dalla modifica su bh6:" << std::endl;
    bh1.Print();
    std::cout << std::endl;

    std::cout << "=== Sistema binario di black hole ===" << std::endl << std::endl;

    BinaryBlackHole bbh(30.0, 0.7, 0.3,     // m1, chi1, theta1
                        20.0, 0.4, 1.2);    // m2, chi2, theta2

    bbh.Print();
    std::cout << std::endl;

    std::cout << "--- Accesso ai dati tramite doppia chiamata ---" << std::endl;
    std::cout << "Massa BH1: " << bbh.GetFirstBlackHole().GetMass() << " Msun" << std::endl;
    std::cout << "Chi BH1:   " << bbh.GetFirstBlackHole().GetSpin() << std::endl;
    std::cout << "Massa BH2: " << bbh.GetSecondBlackHole().GetMass() << " Msun" << std::endl;
    std::cout << "Chi BH2:   " << bbh.GetSecondBlackHole().GetSpin() << std::endl;
    std::cout << std::endl;

    BinaryBlackHole bbhCopy(bbh);   // costruttore di copia
    std::cout << "bbhCopy (copia di bbh):" << std::endl;
    bbhCopy.Print();
    std::cout << std::endl;

    std::cout << "=== Test eccezione: angolo fuori range ===" << std::endl;
    try {
        BinaryBlackHole bbhBad(30.0, 0.7, 4.0,   // theta1 = 4.0 > pi
                               20.0, 0.4, 1.2);
    } catch (const std::invalid_argument& e) {
        std::cout << "Eccezione catturata: " << e.what() << std::endl;
    }

}
