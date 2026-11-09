// =====================================================================
// Coppia.h  -  ESATTAMENTE DUE Elemento                 [GRUPPO B: composizione]
// Usala INVECE di Contenitore quando gli elementi sono due e fissi
// (es. BBH = due BH). Cancella l'altro file e toglilo dal comando di compilazione.
// =====================================================================

#ifndef COPPIA_H
#define COPPIA_H

#include "Elemento.h"  // serve: gli attributi sono di tipo Elemento
#include <ostream>

// Commento: che cosa rappresenta la coppia, unita', che cosa e' memorizzato.
class Coppia
{
public:
    // ================= COSTRUTTORI =================

    // Riceve i due elementi gia' costruiti (e gia' controllati da Elemento)
    Coppia(const Elemento& primo, const Elemento& secondo);

    // ================= GETTER =================
    const Elemento& primo() const;           // oggetti: const&
    const Elemento& secondo() const;

    // ================= SETTER =================
    void setPrimo(const Elemento& e);
    void setSecondo(const Elemento& e);

    // ================= METODI =================
    // totale = a1 + a2          (es. massa totale)
    double totale() const;

    // ================= OPERATORI =================
    // (solo se il testo li chiede: CATALOGO)

    // ================= STAMPA =================
    void print(std::ostream& os) const;

private:
    // ================= ATTRIBUTI =================
    // Elemento non ha costruttore di default: questi vanno costruiti NELLA LISTA.
    Elemento primo_;                         // COMPOSIZIONE
    Elemento secondo_;                       // COMPOSIZIONE

    // ================= METODI DI SERVIZIO =================
    // (controlli, update: CATALOGO)
};

// ================= FUNZIONI LIBERE =================
std::ostream& operator<<(std::ostream& os, const Coppia& c);

#endif
