// =====================================================================
// Nome.h  -  CLASSE CON OPERATORI                         [GRUPPO C: operatori]
// Esempi: Capacitor + e || (02/2022), RLC (01/2025), Frac (09/2024),
//         Polinomio (01/2024), Matrix (05/2022).
//
// LA REGOLA DEGLI OPERATORI
//   * operatore DENTRO la classe -> un argomento in MENO: in  a + b,  a e' l'oggetto
//     su cui si chiama il metodo, b e' l'unico argomento (rhs = quello a destra).
//   * dentro operator+ NON si usa +, dentro operator|| NON si usa ||:
//     si scrive LA FORMULA e si restituisce un oggetto NUOVO:  return Nome(formula);
//   * nel main si usano come in matematica:  a + b,  a || b,  2.0 * a.
//
// Qui: operator+ come MODELLO. Gli altri (-, *, ||, +=, ==, [], 2.0 * a, ...)
// e gli altri costruttori sono nel CATALOGO.md: cerca la frase del testo.
// =====================================================================

#ifndef NOME_H
#define NOME_H

#include <ostream>     // std::ostream -> print e operator<<

// Commento: che cosa rappresenta, unita', vincoli, e il significato FISICO
// degli operatori (es. "+ = serie, || = parallelo").
class Nome
{
public:
    // ================= COSTRUTTORI =================

    // Regolare: a [unita'], vincolo: a > 0.  Un solo argomento -> explicit
    explicit Nome(double a);

    // ================= GETTER =================
    double a() const;                            // [unita']

    // ================= SETTER =================
    // Prima il controllo, poi l'assegnazione. Il costruttore lo riusa.
    void setA(double a);

    // ================= METODI =================
    // (calcoli: CATALOGO o il testo)

    // ================= OPERATORI =================
    // MODELLO: restituisce Nome, un argomento const Nome&, const in fondo.
    // a + b = a_ + rhs.a_      (la TUA formula, es. serie: 1/(1/a_ + 1/rhs.a_))
    Nome operator+(const Nome& rhs) const;

    // ================= STAMPA =================
    void print(std::ostream& os) const;

private:
    // ================= ATTRIBUTI =================
    double a_;                                   // es. capacita' [F]
    // ================= METODI DI SERVIZIO =================
    // (controlli, update: CATALOGO)

};

// ================= FUNZIONI LIBERE =================
// Servono quando a SINISTRA dell'operatore non c'e' un Nome (qui: lo stream).
std::ostream& operator<<(std::ostream& os, const Nome& x);

#endif
