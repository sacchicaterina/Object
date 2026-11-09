// =====================================================================
// Derivata.h  -  CLASSE DERIVATA CONCRETA             [GRUPPI D, E, F: gerarchie]
// es. Rectangle : public Shape
//
// REGOLE della derivata:
//   * class Derivata : public Base
//   * scrive TUTTI i virtuali puri della base, con la STESSA firma (const compreso)
//     e override in fondo (solo nel .h)
//   * gli attributi privati della base non si vedono: si usano i getter (label())
// Per un'altra derivata: CATALOGO D1 (sorella) o D2 (di secondo livello).
// =====================================================================

#ifndef DERIVATA_H
#define DERIVATA_H

#include "Base.h"      // serve: Derivata eredita da Base

// Commento: che cosa rappresenta, unita', vincoli, formula di calcolo().
class Derivata : public Base
{
public:
    // ================= COSTRUTTORI =================

    // Regolare: label (va alla base) e x [unita'] > 0
    Derivata(const std::string& label, double x);

    // ================= GETTER =================
    double x() const;                    // [unita']

    // ================= SETTER =================
    void setX(double x);

    // ================= METODI =================
    // Virtuali della base: STESSA firma + override
    // calcolo = x_ * x_         (es. area del quadrato)
    double calcolo() const override;

    // ================= OPERATORI =================
    // (solo se il testo li chiede: CATALOGO)

    // ================= STAMPA =================
    void print(std::ostream& os) const override;

private:
    // ================= ATTRIBUTI =================
    double x_;                           // [unita']

    // ================= METODI DI SERVIZIO =================
    void controlla(double x) const;      // lancia se x non e' valido
};

#endif
