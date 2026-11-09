// =====================================================================
// Integratore.h  -  STRATEGY PATTERN                                [GRUPPO H]
// Esempio d'esame: Satellite (01/2022): Euler e RungeKutta2, params.ini, sim.dat.
//
// IDEA: l'ALGORITMO e' un oggetto. La Simulazione riceve "un Integratore" (la base)
// e non sa quale algoritmo concreto usa: lo si sceglie nel main (polimorfismo).
//
// I PEZZI:
//   Sistema      (qui)            base astratta: du/dt = f(t, u)     -> la FISICA
//   Modello      (Modello.h)      il sistema concreto del testo
//   Integratore  (qui)            base astratta: un passo u(t) -> u(t + dt)
//   Euler        (qui)            un integratore concreto
//   Simulazione  (Simulazione.h)  ripete i passi e scrive il file
// Altre richieste (RK2, params.ini, confronto, altro modello): CATALOGO, sezione STRATEGY.
// =====================================================================

#ifndef INTEGRATORE_H
#define INTEGRATORE_H

#include <vector>
#include <string>

// Lo stato del sistema: u = (x, v) oppure (x, y, vx, vy), ...
using Stato = std::vector<double>;

// ================= IL PROBLEMA FISICO =================
// du/dt = f(t, u)
class Sistema
{
public:
    virtual ~Sistema() = default;
    virtual Stato derivate(double t, const Stato& u) const = 0;
};

// ================= LA STRATEGIA =================
// Un passo di integrazione
class Integratore
{
public:
    virtual ~Integratore() = default;
    // u(t) -> u(t + dt)
    virtual Stato passo(const Sistema& s, double t, const Stato& u, double dt) const = 0;
    virtual std::string nome() const = 0;
};

// ================= STRATEGIE CONCRETE =================

// Euler:  u_{i+1} = u_i + f(t_i, u_i) * dt
class Euler : public Integratore
{
public:
    Stato passo(const Sistema& s, double t, const Stato& u, double dt) const override;
    std::string nome() const override;
};

// ================= ALTRE STRATEGIE =================
// (RK2, ...: CATALOGO S1)

#endif
