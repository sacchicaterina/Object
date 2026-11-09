// =====================================================================
// Contenitore.cc  -  IMPLEMENTAZIONE di Contenitore     [GRUPPO B: composizione]
//
// SCHEMA del "ciclo sugli elementi" (quasi tutti i metodi sono cosi'):
//   1. parto da zero                         double somma = 0.0;
//   2. prendo gli elementi uno alla volta    for (const Elemento& e : elementi_)
//   3. chiedo all'elemento il SUO valore         e.a()
//   4. lo accumulo                               somma += e.a();
//   5. restituisco                           return somma;
// =====================================================================

#include "Contenitore.h"

#include <stdexcept>   // std::out_of_range

// ===================== COSTANTI =====================
// (costanti fisiche, pi: CATALOGO)

// ===================== COSTRUTTORI =====================

// Vuoto
Contenitore::Contenitore()
    : elementi_()
{
}

// Da un vector di elementi
Contenitore::Contenitore(const std::vector<Elemento>& elementi)
    : elementi_(elementi)
{
}

// ===================== GETTER =====================

std::size_t Contenitore::numero() const
{
    return elementi_.size();
}

// .at() lancia std::out_of_range se i non esiste
const Elemento& Contenitore::elemento(std::size_t i) const
{
    return elementi_.at(i);
}

// ===================== SETTER =====================
// (se il testo li chiede: CATALOGO)

// ===================== METODI =====================

double Contenitore::totale() const
{
    double somma = 0.0;                      // 1. parto da zero
    for (const Elemento& e : elementi_)      // 2. un elemento alla volta
    {
        somma += e.a();                      // 3-4. il suo valore (getter), accumulato
    }
    return somma;                            // 5. il totale
}

// ===================== OPERATORI =====================
// (solo se il testo li chiede: CATALOGO)

// ===================== STAMPA =====================

// Numero di elementi e, riga per riga, indice ed elemento
void Contenitore::print(std::ostream& os) const
{
    os << "Contenitore con " << elementi_.size() << " elementi:\n";
    for (std::size_t i = 0; i < elementi_.size(); ++i)
    {
        os << "  [" << i << "] " << elementi_[i] << "\n";   // riusa operator<< di Elemento
    }
}

// ===================== METODI DI SERVIZIO =====================
// (controlli, update: CATALOGO)

// ===================== FUNZIONI LIBERE =====================

std::ostream& operator<<(std::ostream& os, const Contenitore& c)
{
    c.print(os);
    return os;
}
