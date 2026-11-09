// =====================================================================
// Modello.h  -  il SISTEMA FISICO concreto: qui va la fisica del testo  [GRUPPO H]
// Esempio: oscillatore armonico, u = (x, v):   dx/dt = v,   dv/dt = -omega^2 x
// =====================================================================

#ifndef MODELLO_H
#define MODELLO_H

#include "Integratore.h"   // per Sistema e Stato

class Modello : public Sistema
{
public:
    // ================= COSTRUTTORI =================

    // omega [rad/s] > 0
    explicit Modello(double omega);

    // ================= METODI =================
    // du/dt = (v, -omega^2 x)
    Stato derivate(double t, const Stato& u) const override;

private:
    // ================= ATTRIBUTI =================
    double omega_;                           // [rad/s]
};

#endif
