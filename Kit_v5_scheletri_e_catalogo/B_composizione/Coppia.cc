// =====================================================================
// Coppia.cc  -  IMPLEMENTAZIONE di Coppia               [GRUPPO B: composizione]
// Gli Elemento si usano con i LORO getter: primo_.a()
// (primo_.a_ non si puo': a_ e' privato di Elemento).
// =====================================================================

#include "Coppia.h"

#include <stdexcept>

// ===================== COSTANTI =====================
// (costanti fisiche, pi: CATALOGO)

// ===================== COSTRUTTORI =====================

Coppia::Coppia(const Elemento& primo, const Elemento& secondo)
    : primo_(primo)                          // copia dell'Elemento ricevuto
    , secondo_(secondo)
{
}

// ===================== GETTER =====================

const Elemento& Coppia::primo() const
{
    return primo_;
}

const Elemento& Coppia::secondo() const
{
    return secondo_;
}

// ===================== SETTER =====================

void Coppia::setPrimo(const Elemento& e)
{
    primo_ = e;
}

void Coppia::setSecondo(const Elemento& e)
{
    secondo_ = e;
}

// ===================== METODI =====================

// totale = a1 + a2
double Coppia::totale() const
{
    return primo_.a() + secondo_.a();
}

// ===================== OPERATORI =====================
// (solo se il testo li chiede: CATALOGO)

// ===================== STAMPA =====================

void Coppia::print(std::ostream& os) const
{
    os << "Coppia:\n"
       << "  primo:   " << primo_ << "\n"       // riusa operator<< di Elemento
       << "  secondo: " << secondo_ << "\n"
       << "  totale = " << totale();
}

// ===================== METODI DI SERVIZIO =====================
// (controlli, update: CATALOGO)

// ===================== FUNZIONI LIBERE =====================

std::ostream& operator<<(std::ostream& os, const Coppia& c)
{
    c.print(os);
    return os;
}
