// =====================================================================
// Modello.cc  -  IMPLEMENTAZIONE del modello                        [GRUPPO H]
// =====================================================================

#include "Modello.h"

#include <stdexcept>

// ===================== COSTRUTTORI =====================

Modello::Modello(double omega)
    : omega_(omega)
{
    if (omega_ <= 0.0)
    {
        throw std::invalid_argument("omega deve essere > 0");
    }
}

// ===================== METODI =====================

// u[0] = x, u[1] = v.   t non serve qui: nome commentato (niente warning)
Stato Modello::derivate(double /*t*/, const Stato& u) const
{
    return {u[1], -omega_ * omega_ * u[0]};  // (dx/dt, dv/dt)
}
