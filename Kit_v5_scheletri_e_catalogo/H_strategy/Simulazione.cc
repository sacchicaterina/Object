// =====================================================================
// Simulazione.cc  -  IMPLEMENTAZIONE della simulazione              [GRUPPO H]
// =====================================================================

#include "Simulazione.h"

#include <fstream>
#include <stdexcept>

// ===================== COSTRUTTORI =====================

Simulazione::Simulazione(const Sistema& sistema, const Integratore& integratore)
    : sistema_(sistema)
    , integratore_(integratore)
{
}

// ===================== METODI =====================

Stato Simulazione::esegui(const Stato& u0, double dt, int N, const std::string& fileOutput) const
{
    if (dt <= 0.0 || N <= 0)
    {
        throw std::invalid_argument("servono dt > 0 e N > 0");
    }
    std::ofstream out(fileOutput);
    if (!out)
    {
        throw std::runtime_error("impossibile scrivere " + fileOutput);
    }
    Stato u = u0;
    for (int i = 0; i <= N; ++i)
    {
        double t = i * dt;                       // t calcolato, non sommato: niente errori accumulati
        out << t;                                // 1. scrivo la riga: t u0 u1 ...
        for (double x : u)
        {
            out << " " << x;
        }
        out << "\n";
        if (i < N)
        {
            u = integratore_.passo(sistema_, t, u, dt);   // 2. un passo (chiamata virtuale)
        }
    }
    return u;                                    // stato finale
}
