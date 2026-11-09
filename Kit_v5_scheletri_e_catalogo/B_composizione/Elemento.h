// =====================================================================
// Elemento.h  -  la classe "PEZZO"                     [GRUPPO B: composizione]
// COMPOSIZIONE = una classe CONTIENE oggetti di un'altra:
//   il Contenitore HA degli Elemento (es. PointChargeDist HA delle PointCharge).
// Ordine di lavoro: 1) Elemento completo e PROVATO  2) Contenitore  3) app.cpp
//
// Qui c'e' solo cio' che serve SEMPRE. Il resto (posizione, seconda grandezza,
// contributo in un punto, ...) e' nel CATALOGO.md: cerca la frase del testo.
// =====================================================================

#ifndef ELEMENTO_H
#define ELEMENTO_H

#include <ostream>     // std::ostream -> print e operator<<

// Commento: che cosa rappresenta UN elemento, unita' di misura, vincoli.
class Elemento
{
public:
    // ================= COSTRUTTORI =================

    // Regolare: a [unita'], vincolo: a > 0.  Un solo argomento -> explicit
    explicit Elemento(double a);

    // ================= GETTER =================
    double a() const;                    // [unita']

    // ================= SETTER =================
    // (se il testo li chiede: CATALOGO)

    // ================= METODI =================
    // Calcoli che riguardano UN SOLO elemento: il Contenitore poi li mette insieme.

    // ================= OPERATORI =================
    // (solo se il testo li chiede: CATALOGO)

    // ================= STAMPA =================
    void print(std::ostream& os) const;

private:
    // ================= ATTRIBUTI =================
    double a_;                           // es. carica [C], massa [kg]

    // ================= METODI DI SERVIZIO =================
    void controlla(double a) const;      // lancia un'eccezione se a non e' valido
};

// ================= FUNZIONI LIBERE =================
// Stampa: std::cout << e   (delega a print)
std::ostream& operator<<(std::ostream& os, const Elemento& e);

#endif
