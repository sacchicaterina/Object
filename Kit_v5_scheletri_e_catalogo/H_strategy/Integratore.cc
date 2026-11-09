// =====================================================================
// Integratore.cc  -  IMPLEMENTAZIONE degli integratori              [GRUPPO H]
// =====================================================================

#include "Integratore.h"

#include <cstddef>     // std::size_t

// ===================== METODI DI SERVIZIO =====================

// u + k * v, componente per componente (static = visibile solo in questo file)
static Stato combina(const Stato& u, double k, const Stato& v)
{
    Stato r(u.size());
    for (std::size_t i = 0; i < u.size(); ++i)
    {
        r[i] = u[i] + k * v[i];
    }
    return r;
}

// ===================== EULER =====================

Stato Euler::passo(const Sistema& s, double t, const Stato& u, double dt) const
{
    return combina(u, dt, s.derivate(t, u));     // u + f(t, u) * dt
}

std::string Euler::nome() const
{
    return "Euler";
}
