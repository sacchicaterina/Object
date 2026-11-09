// =====================================================================
// Derivata.cc  -  IMPLEMENTAZIONE della derivata                    [GRUPPO G]
// =====================================================================

#include "Derivata.h"

#include <stdexcept>
#include <cmath>       // std::sin, std::cos, std::pow, ...

// ===================== COSTANTI =====================
// (costanti fisiche, pi: CATALOGO)

// ===================== COSTRUTTORI =====================

// Regolare: la BASE si costruisce nella lista
Derivata::Derivata(const std::string& name)
    : Function(name)
{
}

// ===================== GETTER =====================

// ===================== SETTER =====================

// ===================== METODI =====================

// value = sin(x)
double Derivata::value(double x) const
{
    return std::sin(x);
}

// deriv = cos(x)         (derivata calcolata a mano dalla formula di value)
double Derivata::deriv(double x) const
{
    return std::cos(x);
}

// ===================== OPERATORI =====================
// (solo se il testo li chiede: CATALOGO)

// ===================== STAMPA =====================

void Derivata::print(std::ostream& os) const
{
    os << name() << ": f(1) = " << value(1.0) << ", f'(1) = " << deriv(1.0);   // name(): della base
}

// ===================== METODI DI SERVIZIO =====================

// ===================== FUNZIONI LIBERE =====================

std::ostream& operator<<(std::ostream& os, const Derivata& d)
{
    d.print(os);
    return os;
}
