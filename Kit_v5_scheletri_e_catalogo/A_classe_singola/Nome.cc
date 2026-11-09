// =====================================================================
// Nome.cc  -  IMPLEMENTAZIONE della classe Nome       [GRUPPO A: classe singola]
// Regola: copia la riga dal .h, togli il ;, metti Nome:: davanti al
// nome, tieni const, togli explicit/virtual/override/static/= 0 e i
// valori di default, apri { }.
// =====================================================================

#include "Nome.h"      // SEMPRE per primo: il proprio header

// ----- Include: cio' che serve nei CORPI dei metodi -----
#include <stdexcept>   // throw std::invalid_argument
// (altri include: CATALOGO.md, scheda "INCLUDE")

// ===================== COSTANTI =====================
// (costanti fisiche, pi: CATALOGO)

// ===================== COSTRUTTORI =====================
// Ordine: lista di inizializzazione (nell'ordine degli attributi) -> controlli.

// Regolare
Nome::Nome(const std::string& label, double a)
    : label_(label)
    , a_(a)
{
    controlla(a_);
}

// ===================== GETTER =====================

const std::string& Nome::label() const
{
    return label_;
}

double Nome::a() const
{
    return a_;
}

// ===================== SETTER =====================
// PRIMA il controllo, POI l'assegnazione.

void Nome::setLabel(const std::string& label)
{
    label_ = label;
}

void Nome::setA(double a)
{
    controlla(a);    // se a non e' valido l'eccezione parte qui...
    a_ = a;          // ...e questa riga non viene eseguita
}

// ===================== METODI =====================

// calcolo = 0.5 * a_ * a_
// Costanti decimali: 0.5 e non 1/2 (divisione intera = 0!). Potenze: std::pow, MAI ^.
double Nome::calcolo() const
{
    return 0.5 * a_ * a_;
}

// Dimezza a
void Nome::modifica()
{
    setA(a_ / 2.0);  // passa dal setter: il controllo vale anche qui
}

// ===================== OPERATORI =====================
// (solo se il testo li chiede: CATALOGO)

// ===================== STAMPA =====================

void Nome::print(std::ostream& os) const
{
    os << label_ << ": a = " << a_ << " [unita']";
}

// ===================== METODI DI SERVIZIO =====================
// Nell'if va il CONTRARIO del vincolo (quando il valore e' SBAGLIATO).

void Nome::controlla(double a) const
{
    if (a <= 0.0)
    {
        throw std::invalid_argument("a deve essere > 0");
    }
}

// ===================== FUNZIONI LIBERE =====================
// Niente Nome:: davanti: non sono metodi.

std::ostream& operator<<(std::ostream& os, const Nome& x)
{
    x.print(os);
    return os;
}
