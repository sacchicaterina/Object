#include "massa.h"

#include <iostream>
#include <cmath>
#include <algorithm> // Necessario per std::max
using namespace std;

//constructor default

massa::massa(){
  m1_ = 1.4 * M_solare;
  m2_ = 1.4 * M_solare;

  X1x_ = 0;
  X1y_ = 0;
  X1z_ = 0;

  X2x_ = 0;
  X2y_ = 0;
  X2z_ = 0;

  Mc_ = pow(m1_ * m2_, 3.0/5.0) / pow(m1_ + m2_, 1.0/5.0);
  v_ = m1_ * m2_ / pow(m1_ + m2_,2.0);

}

//constructor 
massa::massa(double X1x, double X1y, double X1z, double X2x, double X2y, double X2z, double Mc, double v) {

  if (v <= 0 || v > 0.25){
    std::cerr << "Errore: v (" << v << ") fuori dal range (0, 0.25]" << std::endl;
        return; // Esce dal costruttore senza assegnare nulla
  }

  // Calcolo masse individuali temporanee (Inversione)
  // Usiamo 0.6 invece di 3.0/5.0 per brevità, è lo stesso valore.
  double factor = 0.5 * (Mc / std::pow(v, 0.6));
  double delta = std::sqrt(1.0 - 4.0 * v);
    
  double m1_tmp = factor * (1.0 + delta);
  double m2_tmp = factor * (1.0 - delta);
  
  if (m1_ < 1 || m1_ > 200 ||  m2_ < 1 || m2_ > 200){
    std::cerr << "Errore: mi fuori dal range [1, 200]" << std::endl;
        return; // Esce dal costruttore senza assegnare nulla
  }

  // 4. Controllo modulo vettori spin (deve essere <= 1)
    double mod1 = std::sqrt(X1x*X1x + X1y*X1y + X1z*X1z);
    double mod2 = std::sqrt(X2x*X2x + X2y*X2y + X2z*X2z);

    if (mod1 > 1.0 || mod2 > 1.0) {
        std::cerr << "Errore: Il modulo dello spin non può superare 1" << std::endl;
        return;
    }

    // 5. ASSEGNAZIONE DEFINITIVA (Solo se tutti i controlli sono passati)
    Mc_ = Mc;
    v_  = v;
    m1_ = m1_tmp;
    m2_ = m2_tmp;

    X1x_ = X1x; X1y_ = X1y; X1z_ = X1z;
    X2x_ = X2x; X2y_ = X2y; X2z_ = X2z;
    
    std::cout << "Oggetto massa creato con successo." << std::endl;
}

//getters
double massa::Mcvalue() const {return Mc_;}
double massa::vvalue() const {return v_;}
double massa::X1xvalue() const {return X1x_;}
double massa::X1yvalue() const {return X1y_;}
double massa::X1zvalue() const {return X1z_;}
double massa::X2xvalue() const {return X2x_;}
double massa::X2yvalue() const {return X2y_;}
double massa::X2zvalue() const {return X2z_;}
double massa::m1value() const { return m1_; }
double massa::m2value() const { return m2_; }

//setters

void massa::set_X1x(double val) {
    if (std::sqrt(val*val + X1y_*X1y_ + X1z_*X1z_) <= 1.0) X1x_ = val;
    else std::cerr << "Errore: Modulo spin 1 > 1.0" << std::endl;
}
void massa::set_X1y(double val) {
    if (std::sqrt(X1x_*X1x_ + val*val + X1z_*X1z_) <= 1.0) X1y_ = val;
    else std::cerr << "Errore: Modulo spin 1 > 1.0" << std::endl;
}
void massa::set_X1z(double val) {
    if (std::sqrt(X1x_*X1x_ + X1y_*X1y_ + val*val) <= 1.0) X1z_ = val;
    else std::cerr << "Errore: Modulo spin 1 > 1.0" << std::endl;
}

void massa::set_X2x(double val) {
    if (std::sqrt(val*val + X2y_*X2y_ + X2z_*X2z_) <= 1.0) X2x_ = val;
    else std::cerr << "Errore: Modulo spin 2 > 1.0" << std::endl;
}
void massa::set_X2y(double val) {
    if (std::sqrt(X2x_*X2x_ + val*val + X2z_*X2z_) <= 1.0) X2y_ = val;
    else std::cerr << "Errore: Modulo spin 2 > 1.0" << std::endl;
}
void massa::set_X2z(double val) {
    if (std::sqrt(X2x_*X2x_ + X2y_*X2y_ + val*val) <= 1.0) X2z_ = val;
    else std::cerr << "Errore: Modulo spin 2 > 1.0" << std::endl;
}

void massa::set_m1(double val) {
    double v_test = (val * m2_) / std::pow(val + m2_, 2.0);
    if (val >= 1.0 && val <= 200.0 && v_test > 0 && v_test <= 0.25) {
        m1_ = val;
        v_  = v_test;
        Mc_ = std::pow(m1_ * m2_, 0.6) / std::pow(m1_ + m2_, 0.2);
    } else {
        std::cerr << "Errore: m1 non valida o v fuori range" << std::endl;
    }
}

void massa::set_m2(double val) {
    double v_test = (m1_ * val) / std::pow(m1_ + val, 2.0);
    if (val >= 1.0 && val <= 200.0 && v_test > 0 && v_test <= 0.25) {
        m2_ = val;
        v_  = v_test;
        Mc_ = std::pow(m1_ * m2_, 0.6) / std::pow(m1_ + m2_, 0.2);
    } else {
        std::cerr << "Errore: m2 non valida o v fuori range" << std::endl;
    }
}

void massa::set_Mc(double val) {
    // Calcoliamo le masse ipotetiche con la v_ attuale
    double factor = 0.5 * (val / std::pow(v_, 0.6));
    double delta = std::sqrt(1.0 - 4.0 * v_);
    double m1_t = factor * (1.0 + delta);
    double m2_t = factor * (1.0 - delta);

    if (m1_t >= 1.0 && m1_t <= 200.0 && m2_t >= 1.0 && m2_t <= 200.0) {
        Mc_ = val;
        m1_ = m1_t;
        m2_ = m2_t;
    } else {
        std::cerr << "Errore: La nuova Mc produrrebbe masse fuori range" << std::endl;
    }
}

void massa::set_v(double val) {
    if (val <= 0.0 || val > 0.25) {
        std::cerr << "Errore: v deve essere in (0, 0.25]" << std::endl;
        return;
    }
    // Calcoliamo le masse ipotetiche con la Mc_ attuale
    double factor = 0.5 * (Mc_ / std::pow(val, 0.6));
    double delta = std::sqrt(1.0 - 4.0 * val);
    double m1_t = factor * (1.0 + delta);
    double m2_t = factor * (1.0 - delta);

    if (m1_t >= 1.0 && m1_t <= 200.0 && m2_t >= 1.0 && m2_t <= 200.0) {
        v_ = val;
        m1_ = m1_t;
        m2_ = m2_t;
    } else {
        std::cerr << "Errore: La nuova v produrrebbe masse fuori range" << std::endl;
    }
}

//passiamo ai metodi

// Rapporto tra le masse (q = m2 / m1)
double massa::calcolo_mass_ratio() const {
    return m2_ / m1_;
}

// Spin efficace (chi_eff)
double massa::calcolo_spin_effettivo() const {
    return (m1_ * X1z_ + m2_ * X2z_) / (m1_ + m2_);
}

// Precessione dello spin (chi_p)
double massa::calcolo_spin_precession() const {
    double q = calcolo_mass_ratio();
    
    // Coefficienti A1 e A2
    double A1 = 2.0 + (3.0 * q) / 2.0;
    double A2 = 2.0 + 3.0 / (2.0 * q);
    
    // Componenti perpendicolari degli spin
    double chi1_perp = std::sqrt(X1x_ * X1x_ + X1y_ * X1y_);
    double chi2_perp = std::sqrt(X2x_ * X2x_ + X2y_ * X2y_);
    
    // Calcolo della parte con il massimo
    double term_max = std::max(A1 * chi1_perp, A2 * chi2_perp);
    
    return (1.0 / (A1 * m1_ * m1_)) * term_max;
}

// Metodo print: stampa i dati principali in modo leggibile
void massa::print() const {
    std::cout << "--- Sistema Binario Compatto ---" << std::endl;
    std::cout << "Masse: m1 = " << m1_ << ", m2 = " << m2_ << " (Mc = " << Mc_ << ", v = " << v_ << ")" << std::endl;
    std::cout << "Spin 1: (" << X1x_ << ", " << X1y_ << ", " << X1z_ << ")" << std::endl;
    std::cout << "Spin 2: (" << X2x_ << ", " << X2y_ << ", " << X2z_ << ")" << std::endl;
    std::cout << "Parametri Derivati:" << std::endl;
    std::cout << " - Mass Ratio (q): " << calcolo_mass_ratio() << std::endl;
    std::cout << " - Spin Effettivo: " << calcolo_spin_effettivo() << std::endl;
    std::cout << " - Precessione:    " << calcolo_spin_precession() << std::endl;
    std::cout << "--------------------------------" << std::endl;
}

// Overload dell'operatore <<
// Nota: essendo friend, non usiamo "massa::" prima del nome
std::ostream& operator<<(std::ostream& os, const massa& obj) {
    os << "BINARIO[m1:" << obj.m1_ << ", m2:" << obj.m2_ 
       << ", chi_eff:" << obj.calcolo_spin_effettivo() << "]";
    return os;
}
