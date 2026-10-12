// app.cpp
//
// Computing Methods for Physics -- 26 June 2023, Part 1 (C++)
//
// Classe GaltonBoard: simula un Galton board verticale con N righe.
// Tutto in un unico file, metodi scritti direttamente dentro la classe
// (nessuna separazione dichiarazione/implementazione).

#include <iostream>
#include <vector>
#include <random>
#include <stdexcept>
#include <string>
#include <algorithm>

class GaltonBoard {
private:
    int N;                                    // numero di righe del board
    int numExperiments;                       // numero di palline simulate finora
    mutable std::mt19937 rng;                 // generatore di numeri casuali.
                                           // "mutable" permette di modificarlo
                                           // (usarlo cambia il suo stato interno,
                                           // avanza la sequenza pseudo-casuale)
                                           // anche dentro eventuali metodi const,
                                           // che altrimenti non potrebbero toccarlo.
    std::uniform_int_distribution<int> coin;  // 0 o 1, equiprobabili
    std::vector<int> counts;                  // array piatto: conteggio palline per cella

    // --- CONVERSIONE TRIANGOLO -> ARRAY PIATTO ---
    // La riga "row" inizia dopo tutte le celle delle righe precedenti,
    // cioe' dopo 1+2+...+row = row*(row+1)/2 celle. Aggiungendo "col"
    // si arriva esattamente alla cella voluta.
    int index(int row, int col) const {
        return row * (row + 1) / 2 + col;
    }

    // Piccolo aiutante per allineare i numeri nella stampa (niente
    // <iomanip>): aggiunge spazi davanti finche' non si raggiunge
    // la larghezza "width".
    std::string padded(int value, int width) const {
        std::string s = std::to_string(value);
        while (static_cast<int>(s.size()) < width) {
            s = " " + s;
        }
        return s;
    }

public:
    // --- COSTRUTTORE ---
    // Riceve N (numero di righe) e deve:
    //  1) salvare N;
    //  2) costruire la board "vuota": un array 1D di N*(N+1)/2 celle,
    //     tutte inizializzate a 0 (nessuna pallina ancora passata);
    //  3) azzerare il contatore delle palline simulate finora.
    //
    // Nota su rng e coin: sono dichiarati come ATTRIBUTI della classe
    // (non variabili locali dentro dropBead) perche' devono persistere
    // tra una chiamata e l'altra di dropBead(). dropBead() viene
    // richiamato tante volte (una per ogni pallina, anche migliaia di
    // volte con Nexp grande): se rng e coin fossero variabili locali,
    // verrebbero ricreati da zero ad ogni singola chiamata, il che
    // sarebbe sia inefficiente (std::random_device puo' essere lento)
    // sia concettualmente sbagliato (vogliamo UN generatore che continua
    // la sua sequenza pseudo-casuale pallina dopo pallina, non uno nuovo
    // ogni volta). Per questo vengono costruiti una sola volta, qui nel
    // costruttore, e restano "vivi" per tutta la vita dell'oggetto.
    explicit GaltonBoard(int N)
        : N(N),
          numExperiments(0),
          rng(std::random_device{}()),
          coin(0, 1)
    {
        if (N <= 0) {
            throw std::invalid_argument("GaltonBoard: N deve essere positivo.");
        }
        counts.resize(N * (N + 1) / 2); // per gli int, resize riempie gia' di 0
    }

    // --- DISCESA DI UNA SINGOLA PALLINA ---
    // Parte da col=0, segna la cella di partenza, poi per ogni riga da
    // 1 a N-1 lancia la "moneta" (0/1), aggiorna col di conseguenza,
    // e segna la cella attraversata.
    void dropBead() {
        int col = 0;
        counts[index(0, col)] += 1;

        for (int row = 1; row < N; ++row) {
            col += coin(rng); // 0 -> resta (sinistra), 1 -> +1 (destra)
            counts[index(row, col)] += 1;
        }

        ++numExperiments;
    }

    // --- SIMULAZIONE DI Nexp PALLINE ---
    // Basta un ciclo su Nexp che richiama dropBead().
    void runExperiment(int Nexp) {
        for (int i = 0; i < Nexp; ++i) {
            dropBead();
        }
    }

    // --- RESET ---
    // "Dimentica" tutte le simulazioni fatte finora: rimette ogni
    // cella di counts a 0 e azzera il contatore delle palline.
    void reset() {
        // Versione con std::fill (riempie tutto l'intervallo [begin,end) con 0):
        // std::fill(counts.begin(), counts.end(), 0);

        // Versione con range-based for: "auto&" e' un riferimento MODIFICABILE
        // a ogni elemento di counts (senza "&" sarebbe una copia, e modificarla
        // non avrebbe effetto sul vettore vero; con "const auto&" invece non
        // si potrebbe scrivere c = 0, perche' const vieta la modifica).
        for (auto& c : counts) {
            c = 0;
        }

        numExperiments = 0;
    }

    // --- PRINT ---
    // Stampa counts riga per riga, cosi' si vede la struttura triangolare.
    void print() const {
        std::cout << "Galton board con N = " << N
                  << " righe, dopo " << numExperiments
                  << " esperimenti:\n";

        for (int row = 0; row < N; ++row) {
            std::cout << std::string((N - row - 1) * 2, ' '); // indentazione
            for (int col = 0; col <= row; ++col) {
                std::cout << padded(counts[index(row, col)], 4);
            }
            std::cout << '\n';
        }
        std::cout << std::endl;
    }

    // Restituisce il numero totale di palline simulate finora.
    int getNumExperiments() const {
        return numExperiments;
    }
};

// ---------------------------------------------------------------
// main: simula 3 esperimenti indipendenti (10, 100, 1000 palline)
// su un board di 10 righe, come richiesto dal testo.
// ---------------------------------------------------------------

int main() {
    const int N = 10;
    std::vector<int> experiments = {10, 100, 1000};

    for (int Nexp : experiments) {
        GaltonBoard board(N);
        board.runExperiment(Nexp);

        std::cout << "=== Simulazione con Nexp = " << Nexp << " palline ===\n";
        board.print();
    }

}
