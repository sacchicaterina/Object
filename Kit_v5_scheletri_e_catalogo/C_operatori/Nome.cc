// =====================================================================
// Nome.cc  -  IMPLEMENTAZIONE di Nome                     [GRUPPO C: operatori]
// Regola: copia la riga dal .h, togli il ;, metti Nome:: davanti al nome
// (anche Nome::operator+), tieni const, togli explicit e valori di default.
// Dentro un metodo:  a_ = il valore di QUESTO oggetto,  rhs.a_ = quello dell'ALTRO
// (si puo': private vale per la CLASSE, non per il singolo oggetto).
// =====================================================================

#include "Nome.h"

#include <stdexcept>   // throw std::invalid_argument

// ===================== COSTANTI =====================
// (costanti fisiche, pi: CATALOGO)

// ===================== COSTRUTTORI =====================

// Regolare: parto da zero e lascio controllare e assegnare al setter
Nome::Nome(double a)
    : a_(0.0)
{
    setA(a);
}

// ===================== GETTER =====================

double Nome::a() const
{
    return a_;
}

// ===================== SETTER =====================

void Nome::setA(double a)
{
    if (a <= 0.0)                      // nell'if: quando il valore e' SBAGLIATO
    {
        throw std::invalid_argument("a deve essere > 0");
    }
    a_ = a;                            // poi l'assegnazione
}

// ===================== METODI =====================
// (calcoli: CATALOGO o il testo)

// ===================== OPERATORI =====================
// Schema: return Nome( formula con a_ e rhs.a_ );

// a + b = a_ + rhs.a_
Nome Nome::operator+(const Nome& rhs) const
{
    return Nome(a_ + rhs.a_);
}

// ===================== STAMPA =====================

void Nome::print(std::ostream& os) const
{
    os << "a = " << a_ << " [unita']";
}

// ===================== METODI DI SERVIZIO =====================
// (controlli, update: CATALOGO)

// ===================== FUNZIONI LIBERE =====================

std::ostream& operator<<(std::ostream& os, const Nome& x)
{
    x.print(os);
    return os;
}
