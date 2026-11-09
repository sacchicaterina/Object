// =====================================================================
// Nome.h  -  INTERFACCIA della classe Nome            [GRUPPO A: classe singola]
// Contiene: dichiarazioni (firme con ;), attributi, funzioni libere.
// Non contiene: corpi dei metodi, main, using namespace std.
//
// Qui c'e' solo cio' che serve SEMPRE. Per ogni altra richiesta del testo
// apri CATALOGO.md, cerca (Cmd+F) la frase inglese del testo e copia i pezzi
// .h / .cc / main nelle sezioni con lo stesso titolo.
// =====================================================================

#ifndef NOME_H     // include guard: NOMECLASSE_H, unico per ogni header
#define NOME_H

// ----- Include: solo cio' che serve alle FIRME di questo file -----
#include <ostream>     // std::ostream -> print e operator<<
#include <string>      // std::string  -> etichette, nomi

// Commento obbligatorio: che cosa rappresenta la classe,
// unita' di misura, vincoli, scelte (cosa e' memorizzato, cosa calcolato).
class Nome
{
public:
    // ================= COSTRUTTORI =================
    // Stesso nome della classe, nessun tipo di ritorno.

    // Regolare: riceve tutti i dati.  a [unita'], vincolo: a > 0
    Nome(const std::string& label, double a);

    // ================= GETTER =================
    // Leggono un attributo: sempre const.
    const std::string& label() const;   // oggetti: restituisci const&
    double a() const;                    // numeri: per valore

    // ================= SETTER =================
    // Modificano un attributo: void, mai const. Controllano PRIMA di assegnare.
    void setLabel(const std::string& label);
    void setA(double a);

    // ================= METODI =================
    // Calcolano un risultato: restituiscono il valore, const.
    // Formula in C++ qui nel commento, pronta da copiare nel .cc:
    //   calcolo = 0.5 * a_ * a_
    double calcolo() const;

    // Modificano l'oggetto stesso: void, non const.
    void modifica();

    // ================= OPERATORI =================
    // (solo se il testo li chiede: CATALOGO)

    // ================= STAMPA =================
    void print(std::ostream& os) const;

private:
    // ================= ATTRIBUTI =================
    // Nome con _ finale; commento con significato e unita'.
    std::string label_;                  // etichetta
    double a_;                           // [unita']

    // ================= METODI DI SERVIZIO =================
    // Li usa solo la classe.
    void controlla(double a) const;      // lancia un'eccezione se a non e' valido
};

// ================= FUNZIONI LIBERE (fuori dalla classe) =================
// Stampa: std::cout << x   (delega a print)
std::ostream& operator<<(std::ostream& os, const Nome& x);

#endif
