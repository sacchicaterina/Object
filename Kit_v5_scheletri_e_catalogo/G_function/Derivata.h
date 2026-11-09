// =====================================================================
// Derivata.h  -  derivata della classe data dal testo               [GRUPPO G]
// es. Polynomial : public Function,  Sine : public Function
//
// REGOLE:
//   * class Derivata : public Function
//   * TUTTI i virtuali puri (= 0) della base, con firma IDENTICA (copiala dal
//     testo!) + override
//   * name() e' gia' scritto nella base: non serve riscriverlo
// Altre richieste (polinomio, parametri, primitiva, rapporto, tabella su file):
// CATALOGO.md, sezione FUNCTION.
// =====================================================================

#ifndef DERIVATA_H
#define DERIVATA_H

#include "Function.h"  // la classe del testo
#include <ostream>     // std::ostream -> print e operator<<

// Commento: che funzione e', formule di value e deriv.
class Derivata : public Function
{
public:
    // ================= COSTRUTTORI =================

    // Regolare: il nome va alla base
    explicit Derivata(const std::string& name);

    // ================= GETTER =================
    // (name() c'e' gia' nella base)

    // ================= SETTER =================
    // (se il testo li chiede: CATALOGO)

    // ================= METODI =================
    // Virtuali puri della base: firma IDENTICA + override
    // value = std::sin(x)       deriv = std::cos(x)
    double value(double x) const override;
    double deriv(double x) const override;

    // ================= OPERATORI =================
    // (solo se il testo li chiede: CATALOGO)

    // ================= STAMPA =================
    void print(std::ostream& os) const;

private:
    // ================= ATTRIBUTI =================
    // (parametri della funzione: CATALOGO G2, G7)

    // ================= METODI DI SERVIZIO =================
    // (controlli, update: CATALOGO)
};

// ================= FUNZIONI LIBERE =================
std::ostream& operator<<(std::ostream& os, const Derivata& d);

#endif
