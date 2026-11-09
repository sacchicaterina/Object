// =====================================================================
// Derivata.cc  -  IMPLEMENTAZIONE della derivata      [GRUPPI D, E, F: gerarchie]
// Regole: Derivata:: davanti; niente override nel .cc.
// Nel costruttore: la BASE si costruisce per PRIMA, nella lista: : Base(label), x_(x)
// =====================================================================

#include "Derivata.h"

#include <stdexcept>   // std::invalid_argument

// ===================== COSTANTI =====================
// (costanti fisiche, pi: CATALOGO)

// ===================== COSTRUTTORI =====================

// Regolare: prima la base, poi i miei attributi, poi i controlli
Derivata::Derivata(const std::string& label, double x)
    : Base(label)
    , x_(x)
{
    controlla(x_);
}

// ===================== GETTER =====================

double Derivata::x() const
{
    return x_;
}

// ===================== SETTER =====================

void Derivata::setX(double x)
{
    controlla(x);
    x_ = x;
}

// ===================== METODI =====================

// calcolo = x_ * x_
double Derivata::calcolo() const
{
    return x_ * x_;
}

// ===================== OPERATORI =====================
// (solo se il testo li chiede: CATALOGO)

// ===================== STAMPA =====================

void Derivata::print(std::ostream& os) const
{
    // label_ e' privato della base: si usa il getter label()
    os << "Derivata '" << label() << "': x = " << x_ << ", calcolo = " << calcolo();
}

// ===================== METODI DI SERVIZIO =====================

void Derivata::controlla(double x) const
{
    if (x <= 0.0)
    {
        throw std::invalid_argument("x deve essere > 0");
    }
}

// ===================== FUNZIONI LIBERE =====================
// (operator<< c'e' gia' nella base: vale anche per Derivata)
