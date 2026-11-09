// =====================================================================
// Base.cc  -  IMPLEMENTAZIONE della base             [GRUPPI D, E, F: gerarchie]
// Regole .h -> .cc: Base:: davanti al nome; niente ; ; const ripetuto;
// NON si ripetono virtual, override, = 0, explicit, static.
// I virtuali PURI (= 0) qui NON si scrivono.
// =====================================================================

#include "Base.h"

#include <stdexcept>

// ===================== COSTANTI =====================
// (costanti fisiche, pi: CATALOGO)

// ===================== COSTRUTTORI =====================

// Regolare
Base::Base(const std::string& label)
    : label_(label)
{
}

// Distruttore: niente da fare, ma va scritto (e' dichiarato nel .h)
Base::~Base()
{
}

// ===================== GETTER =====================

const std::string& Base::label() const
{
    return label_;
}

// ===================== SETTER =====================

void Base::setLabel(const std::string& label)
{
    label_ = label;
}

// ===================== METODI =====================
// (calcolo e' virtuale puro: non si scrive qui)

// ===================== OPERATORI =====================
// (solo se il testo li chiede: CATALOGO)

// ===================== STAMPA =====================
// (print e' virtuale puro: non si scrive qui)

// ===================== METODI DI SERVIZIO =====================
// (controlli, update: CATALOGO)

// ===================== FUNZIONI LIBERE =====================

std::ostream& operator<<(std::ostream& os, const Base& b)
{
    b.print(os);        // chiamata virtuale: esegue il print della derivata vera
    return os;
}
