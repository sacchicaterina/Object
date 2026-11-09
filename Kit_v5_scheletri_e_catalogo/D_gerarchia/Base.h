// =====================================================================
// Base.h  -  CLASSE BASE ASTRATTA                    [GRUPPI D, E, F: gerarchie]
// D = famiglia di forme (Shape -> Triangle, Rectangle, Sphere, Cone, ...)
// E = stessi dati, calcoli diversi (DataSet -> Arithmetic, Geometric, Harmonic)
// F = famiglia fisica con contatore (Solid, IdealGas, Capacitor, RLC)
// Ordine di lavoro: Base.h/.cc -> Derivata.h/.cc -> app.cpp -> altre derivate
//
// EREDITARIETA' = "e' un": un Rettangolo E' UNA Forma.
// La BASE dice COSA sa fare ogni oggetto della famiglia (virtuali puri = 0);
// ogni DERIVATA dice COME lo fa (override).
//
// REGOLE della base:
//   * distruttore SEMPRE virtual
//   * virtual ... = 0  -> virtuale PURO: la base non lo scrive, ogni derivata DEVE farlo
//   * con almeno un virtuale puro la base e' ASTRATTA: non si puo' creare "Base b;"
//   * operator<< si scrive UNA volta, sulla base: chiama print, che e' virtuale
//
// Altre richieste (seconda derivata, livello 2, contatore, dati, protected, ...):
// CATALOGO.md, sezione GERARCHIE.
// =====================================================================

#ifndef BASE_H
#define BASE_H

#include <ostream>     // std::ostream -> print e operator<<
#include <string>      // std::string  -> label

// Commento: che cosa rappresenta la FAMIGLIA, unita', che cosa e' comune (qui)
// e che cosa dipende dalla derivata (i virtuali puri).
class Base
{
public:
    // ================= COSTRUTTORI =================

    // Regolare: la parte comune a tutte le derivate
    explicit Base(const std::string& label);

    // Distruttore: SEMPRE virtual nella base (cosi' delete su un Base* chiama quello giusto)
    virtual ~Base();

    // ================= GETTER =================
    const std::string& label() const;

    // ================= SETTER =================
    void setLabel(const std::string& label);

    // ================= METODI =================
    // Virtuali PURI: ogni derivata concreta li DEVE scrivere (con override).
    // es. area(), volume(), mean(), capacita()
    virtual double calcolo() const = 0;

    // ================= OPERATORI =================
    // (solo se il testo li chiede: CATALOGO)

    // ================= STAMPA =================
    virtual void print(std::ostream& os) const = 0;

protected:
    // ================= ATTRIBUTI PROTECTED =================
    // (visibili dalle derivate ma non dal main: CATALOGO D5)

private:
    // ================= ATTRIBUTI =================
    std::string label_;                  // nome dell'oggetto

    // ================= METODI DI SERVIZIO =================
    // (controlli, update: CATALOGO)
};

// ================= FUNZIONI LIBERE =================
// Una sola, sulla BASE: funziona per ogni derivata perche' print e' virtuale.
std::ostream& operator<<(std::ostream& os, const Base& b);

#endif
