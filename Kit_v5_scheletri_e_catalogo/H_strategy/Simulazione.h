// =====================================================================
// Simulazione.h  -  usa un Sistema e un Integratore tramite la BASE  [GRUPPO H]
// =====================================================================

#ifndef SIMULAZIONE_H
#define SIMULAZIONE_H

#include "Integratore.h"
#include <string>

class Simulazione
{
public:
    // ================= COSTRUTTORI =================
    Simulazione(const Sistema& sistema, const Integratore& integratore);

    // ================= METODI =================
    // Da t = 0, N passi di ampiezza dt; scrive su file le colonne  t u0 u1 ...
    // Restituisce lo stato finale.
    Stato esegui(const Stato& u0, double dt, int N, const std::string& fileOutput) const;

private:
    // ================= ATTRIBUTI =================
    // Riferimenti: la simulazione USA gli oggetti, non li possiede (vanno nella lista).
    const Sistema& sistema_;
    const Integratore& integratore_;
};

#endif
