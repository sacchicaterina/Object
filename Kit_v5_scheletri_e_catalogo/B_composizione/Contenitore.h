// =====================================================================
// Contenitore.h  -  la classe che CONTIENE gli Elemento  [GRUPPO B: composizione]
// IDEA: ogni Elemento sa calcolare le SUE cose; il Contenitore le mette
//       insieme (somma, media, massimo, ...) con un ciclo su tutti gli elementi.
//
// Tanti elementi -> std::vector<Elemento> (questo file).
// Esattamente DUE elementi fissi (es. BBH) -> CATALOGO.md, scheda "DUE ELEMENTI FISSI".
// =====================================================================

#ifndef CONTENITORE_H
#define CONTENITORE_H

#include "Elemento.h"  // serve: l'attributo contiene degli Elemento
#include <ostream>     // std::ostream -> print
#include <vector>      // std::vector  -> numero di elementi deciso durante l'esecuzione
#include <cstddef>     // std::size_t  -> numeri e indici

// Commento: che cosa rappresenta l'INSIEME, unita', scelte di progetto
// (es. "il totale e' calcolato al volo, non memorizzato").
class Contenitore
{
public:
    // ================= COSTRUTTORI =================

    // Vuoto: nessun elemento
    Contenitore();

    // Da un vector di elementi gia' pronti
    explicit Contenitore(const std::vector<Elemento>& elementi);

    // ================= GETTER =================
    std::size_t numero() const;                         // quanti elementi
    const Elemento& elemento(std::size_t i) const;      // l'i-esimo (da 0); lancia se non esiste

    // ================= SETTER =================
    // (se il testo li chiede: CATALOGO)

    // ================= METODI =================
    // Somma delle a di tutti gli elementi [unita']
    double totale() const;

    // ================= OPERATORI =================
    // (solo se il testo li chiede: CATALOGO)

    // ================= STAMPA =================
    void print(std::ostream& os) const;

private:
    // ================= ATTRIBUTI =================
    std::vector<Elemento> elementi_;                    // COMPOSIZIONE: tanti Elemento
    // ================= METODI DI SERVIZIO =================
    // (controlli, update: CATALOGO)

};

// ================= FUNZIONI LIBERE =================
std::ostream& operator<<(std::ostream& os, const Contenitore& c);

#endif
