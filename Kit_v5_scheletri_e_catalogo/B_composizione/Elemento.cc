// =====================================================================
// Elemento.cc  -  IMPLEMENTAZIONE di Elemento           [GRUPPO B: composizione]
// Regola: copia la riga dal .h, togli il ;, metti Elemento:: davanti al
// nome, tieni const, togli explicit/static/valori di default, apri { }.
// =====================================================================

#include "Elemento.h"

#include <stdexcept>   // throw std::invalid_argument

// ===================== COSTANTI =====================
// (costanti fisiche, pi: CATALOGO)

// ===================== COSTRUTTORI =====================

// Regolare
Elemento::Elemento(double a)
    : a_(a)
{
    controlla(a_);
}

// ===================== GETTER =====================

double Elemento::a() const
{
    return a_;
}

// ===================== SETTER =====================
// (se il testo li chiede: CATALOGO)

// ===================== METODI =====================
// (calcoli: CATALOGO o il testo)

// ===================== OPERATORI =====================
// (solo se il testo li chiede: CATALOGO)

// ===================== STAMPA =====================

void Elemento::print(std::ostream& os) const
{
    os << "a = " << a_ << " [unita']";
}

// ===================== METODI DI SERVIZIO =====================

void Elemento::controlla(double a) const
{
    if (a <= 0.0)
    {
        throw std::invalid_argument("a deve essere > 0");
    }
}

// ===================== FUNZIONI LIBERE =====================

std::ostream& operator<<(std::ostream& os, const Elemento& e)
{
    e.print(os);
    return os;
}
