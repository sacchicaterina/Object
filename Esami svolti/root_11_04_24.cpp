// ============================================================================
// COMANDO PER ESEGUIRE QUESTO FILE IN ROOT:
//
//   root -l -q root_11_04_24.cpp+
//
// ============================================================================
//
// root_11_04_24.cpp
//
// NOTA SUL NOME: il file originale si chiamava "11_04_24.cpp". ROOT pero'
// richiede che, eseguendo "root file.cpp+", esista una funzione con lo
// STESSO NOME del file (senza estensione) - qui verrebbe cercata una
// funzione chiamata "11_04_24", ma non e' un nome valido in C++ (un
// identificatore non puo' iniziare con una cifra). Per questo il file e'
// stato rinominato "root_11_04_24.cpp" e la funzione un tempo chiamata
// "int main()" e' ora "void root_11_04_24()": nomi allineati, nessun altro
// contenuto del file e' stato toccato.
//
// Computing Methods for Physics 1 - Exam 11 April 2024 - Part 1 (C++)
//
// Single-file implementation (no separate .h/.cc) of a class representing
// binary black hole systems, plus a main() that uses ROOT to:
//   - draw 1e4 values of m1, m2 ~ Uniform[5,100] Msun and plot the
//     distribution of the chirp mass Mc;
//   - draw 1e4 spin vectors chi1, chi2 with magnitude ~ Uniform[0,1] and
//     direction uniform on the unit sphere, and plot the distribution of
//     the effective spin parameter chi_eff.
//
// Masses are expressed in solar masses (Msun), between 5 and 100 Msun;
// this range is checked directly with if statements inside the setters.
//
// Compile (example, outside ROOT):
//   g++ root_11_04_24.cpp -o root_11_04_24 `root-config --cflags --libs`
//
// Or run interactively inside ROOT:
//   root -l -q root_11_04_24.cpp+
// ============================================================================

#include <iostream>
#include <cmath>
#include <stdexcept>
#include <array>
#include <string>

#include "TRandom3.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TAxis.h"

// ----------------------------------------------------------------------------
// Class BinaryBlackHole
//
// Represents a binary black hole system characterized by 8 intrinsic
// parameters: the two component masses m1, m2 (in solar masses, Msun) and
// the two dimensionless spin vectors chi1 = (chi1x, chi1y, chi1z),
// chi2 = (chi2x, chi2y, chi2z), with |chi_i| <= 1.
//
// Physical constraints enforced by the class:
//   - m_i in [5, 100] Msun for i = 1, 2
//   - |chi_i| <= 1 for i = 1, 2
// ----------------------------------------------------------------------------
class BinaryBlackHole {
public:
    static constexpr double kMinMass = 5.0;    // Msun
    static constexpr double kMaxMass = 100.0;  // Msun

    // ---- Constructors -------------------------------------------------

    // Regular constructor: build a BBH from masses and spin vectors.
    // Valida direttamente i parametri (CheckMass/CheckSpin) e assegna
    // i membri, senza passare dai setter: il costruttore e i setter
    // restano cosi' due percorsi separati.
    BinaryBlackHole(double m1, double m2,
                     const std::array<double, 3>& chi1,
                     const std::array<double, 3>& chi2)
    {
        CheckMass(m1, "m1");
        CheckMass(m2, "m2");
        CheckSpin(chi1, "chi1");
        CheckSpin(chi2, "chi2");

        mass1 = m1;
        mass2 = m2;
        chi1_ = chi1;
        chi2_ = chi2;
    }

    // Default constructor: equal, non-spinning black holes. Prende un solo
    // parametro "mass" (con valore di default), usato per entrambe le
    // masse -> garantisce sempre m1 = m2. Chiamabile sia come
    // BinaryBlackHole() sia come BinaryBlackHole(massa_scelta).
    BinaryBlackHole(double mass = 10.0)
        : BinaryBlackHole(mass, mass, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0})
    {}

    // Copy constructor.
    BinaryBlackHole(const BinaryBlackHole& other)
        : mass1(other.mass1), mass2(other.mass2),
          chi1_(other.chi1_), chi2_(other.chi2_)
    {}

    // ---- Setters --------------------------------------------------------
    // Ogni setter richiama i controlli privati (CheckMass/CheckSpin) e
    // lancia un'eccezione se il valore non e' fisicamente valido.

    void SetM1(double m1) {
        CheckMass(m1, "m1");
        mass1 = m1;
    }

    void SetM2(double m2) {
        CheckMass(m2, "m2");
        mass2 = m2;
    }

    void SetChi1(const std::array<double, 3>& chi1) {
        CheckSpin(chi1, "chi1");
        chi1_ = chi1;
    }

    void SetChi2(const std::array<double, 3>& chi2) {
        CheckSpin(chi2, "chi2");
        chi2_ = chi2;
    }

    // ---- Getters ----------------------------------------------------------

    double GetM1() const { return mass1; }
    double GetM2() const { return mass2; }

    std::array<double, 3> GetChi1() const { return chi1_; }
    std::array<double, 3> GetChi2() const { return chi2_; }

    double GetChi1x() const { return chi1_[0]; }
    double GetChi1y() const { return chi1_[1]; }
    double GetChi1z() const { return chi1_[2]; }

    double GetChi2x() const { return chi2_[0]; }
    double GetChi2y() const { return chi2_[1]; }
    double GetChi2z() const { return chi2_[2]; }

    // ---- Physics: derived quantities --------------------------------------

    // Effective spin parameter:
    //   chi_eff = (m1*chi1z + m2*chi2z) / (m1 + m2)
    double EffectiveSpin() const {
        return (mass1 * chi1_[2] + mass2 * chi2_[2]) / (mass1 + mass2);
    }

    // Chirp mass:
    //   Mc = (m1*m2)^(3/5) / (m1+m2)^(1/5)
    double ChirpMass() const {
        double num = std::pow(mass1 * mass2, 3.0 / 5.0);
        double den = std::pow(mass1 + mass2, 1.0 / 5.0);
        return num / den;
    }

    // ---- Utility ------------------------------------------------------

    // Dichiaro operator<< come funzione "amica" (friend): e' una funzione
    // libera (definita fuori dalla classe, subito dopo), ma questa riga le
    // da' il permesso di accedere ai membri privati (mass1, mass2, chi1_,
    // chi2_, Norm) come se fosse un metodo della classe.
    friend std::ostream& operator<<(std::ostream& os, const BinaryBlackHole& bbh);

private:
    double mass1;                     // mass of black hole 1 [Msun]
    double mass2;                     // mass of black hole 2 [Msun]
    std::array<double, 3> chi1_;      // dimensionless spin vector of BH1
    std::array<double, 3> chi2_;      // dimensionless spin vector of BH2

    static double Norm(const std::array<double, 3>& v) {
        double sum = 0.0;
        for (int i = 0; i < 3; ++i) {
            sum += v[i] * v[i];
        }
        return std::sqrt(sum);
    }

    static void CheckMass(double m, const std::string& name) {
        if (m < kMinMass || m > kMaxMass) {
            throw std::out_of_range(name + " deve essere tra 5 e 100 Msun");
        }
    }

    static void CheckSpin(const std::array<double, 3>& chi, const std::string& name) {
        double mag = Norm(chi);
        if (mag > 1.0) {
            throw std::out_of_range("|" + name + "| deve essere <= 1");
        }
    }
};

// Implementazione di operator<< come funzione libera (fuori dalla classe):
// e' questa che rende possibile scrivere "std::cout << bbh;", con
// l'oggetto a destra dell'operatore. Grazie alla dichiarazione "friend"
// dentro la classe, questa funzione puo' accedere direttamente ai membri
// privati di bbh (mass1, mass2, chi1_, chi2_) e al metodo privato Norm,
// esattamente come se fosse un metodo membro.
std::ostream& operator<<(std::ostream& os, const BinaryBlackHole& bbh) {
    os << "----------------------------------------------------\n"
       << "Binary Black Hole system:\n"
       << "  m1 = " << bbh.mass1 << " Msun\n"
       << "  m2 = " << bbh.mass2 << " Msun\n"
       << "  chi1 = (" << bbh.chi1_[0] << ", " << bbh.chi1_[1] << ", "
       << bbh.chi1_[2] << "),  |chi1| = " << BinaryBlackHole::Norm(bbh.chi1_) << "\n"
       << "  chi2 = (" << bbh.chi2_[0] << ", " << bbh.chi2_[1] << ", "
       << bbh.chi2_[2] << "),  |chi2| = " << BinaryBlackHole::Norm(bbh.chi2_) << "\n"
       << "  chi_eff = " << bbh.EffectiveSpin() << "\n"
       << "  Mc      = " << bbh.ChirpMass() << " Msun\n"
       << "----------------------------------------------------\n";
    return os;
}

// ----------------------------------------------------------------------------
// BinaryBlackHole(): use the class together with ROOT to generate and plot
// the required distributions.
//
// NOTA: questa funzione era originariamente "int main()". E' stata
// rinominata in "BinaryBlackHole()" (stesso nome del file, senza
// estensione) perche' e' la convenzione richiesta da ROOT quando si
// esegue uno script con "root file.cpp+": ROOT cerca ed esegue
// automaticamente una funzione con lo stesso nome del file. Con "int
// main()" invece root avrebbe cercato una funzione chiamata
// "BinaryBlackHole" senza trovarla, dando errore.
// ----------------------------------------------------------------------------
void root_11_04_24() {
    // gStyle e' l'oggetto globale di ROOT (di tipo TStyle) che controlla
    // l'aspetto di TUTTI i grafici disegnati dopo questa riga: colori,
    // font, margini, e in particolare il box delle statistiche mostrato
    // sopra ogni istogramma.
    // SetOptStat(1111) e' un codice a 4 cifre: ogni cifra (da destra verso
    // sinistra) accende/spegne una voce del box statistiche:
    //   posizione 1 (destra) -> i = integrale
    //   posizione 2          -> o = overflow
    //   posizione 3          -> u = underflow
    //   posizione 4 (sinistra) -> in realta' le prime 2 cifre "11" attivano
    //                             mean (m) e RMS (r) quando scritte per esteso
    // In pratica 1111 e' il valore standard: mostra nome istogramma,
    // numero di entries, media (mean) e deviazione standard (RMS).
    // Per nascondere completamente il box si userebbe gStyle->SetOptStat(0).
    gStyle->SetOptStat(1111);

    // kN e' il numero di estrazioni casuali da fare (1e4 = 10000), come
    // richiesto dal testo dell'esame.
    const int kN = 10000; // 1e4 draws

    // TRandom3 e' il generatore di numeri casuali di ROOT (basato
    // sull'algoritmo Mersenne Twister, piu' affidabile del rand() del C).
    // L'argomento 0 nel costruttore dice a ROOT di scegliere da solo un
    // seed basato sull'orologio di sistema, quindi ogni esecuzione del
    // programma produce numeri diversi (non riproducibili run dopo run).
    TRandom3 rng(0);      // seed 0 => time-based seed

    // ---------------------------------------------------------------
    // 1) Draw m1, m2 ~ Uniform[5,100] and plot the distribution of Mc.
    // ---------------------------------------------------------------

    // TH1D e' la classe di ROOT per un istogramma 1-dimensionale con bin
    // a precisione double. Il costruttore prende:
    //   "hMc"        -> nome interno univoco dell'istogramma (usato da ROOT
    //                   per identificarlo, es. quando lo si salva su file)
    //   "titolo;xlabel;ylabel" -> titolo del grafico e nomi degli assi,
    //                   separati da ';' (sintassi speciale di ROOT)
    //   100          -> numero di bin in cui dividere l'asse x
    //   kMinMass, kMaxMass -> estremi dell'asse x (il range in cui viene
    //                   costruito l'istogramma)
    // L'oggetto viene creato con "new", quindi e' un puntatore: e' la
    // convenzione tipica in ROOT, perche' TH1D si "registra" da solo
    // presso il file/canvas corrente e la sua gestione della memoria
    // segue regole particolari legate al framework.
    TH1D* hMc = new TH1D("hMc", "Chirp mass distribution;M_{c} [M_{#odot}];Counts",
                          100, BinaryBlackHole::kMinMass, BinaryBlackHole::kMaxMass);

    // Ciclo di generazione: ripeto kN volte l'estrazione di un sistema
    // binario casuale e ne calcolo la chirp mass.
    for (int i = 0; i < kN; ++i) {
        // rng.Uniform(a, b) restituisce un numero casuale uniforme
        // nell'intervallo [a, b) -- qui uso i limiti di massa della classe.
        double m1 = rng.Uniform(BinaryBlackHole::kMinMass, BinaryBlackHole::kMaxMass);
        double m2 = rng.Uniform(BinaryBlackHole::kMinMass, BinaryBlackHole::kMaxMass);

        // Costruisco un sistema binario non rotante (spin nulli): per il
        // calcolo della chirp mass gli spin non contano, servono solo m1, m2.
        BinaryBlackHole bbh(m1, m2, {0, 0, 0}, {0, 0, 0});

        // Fill(valore) aggiunge una "entry" all'istogramma: incrementa di 1
        // il conteggio del bin in cui cade bbh.ChirpMass(). Ripetuto kN
        // volte, questo costruisce la distribuzione della chirp mass.
        hMc->Fill(bbh.ChirpMass());
    }

    // TCanvas e' la "finestra" (o superficie di disegno) di ROOT su cui
    // vengono effettivamente disegnati gli istogrammi/grafici. Il
    // costruttore prende: nome interno, titolo della finestra, larghezza
    // e altezza in pixel.
    TCanvas* cMc = new TCanvas("cMc", "Chirp mass distribution", 800, 600);

    // Imposto il colore del bordo delle barre dell'istogramma (blu scuro,
    // "+1" scurisce leggermente il colore base kBlue della palette ROOT).
    hMc->SetLineColor(kBlue + 1);

    // Imposto il colore di riempimento delle barre, con trasparenza:
    // kBlue-9 e' un blu piu' chiaro/tenue, 0.5 e' l'opacita' (50%).
    hMc->SetFillColorAlpha(kBlue - 9, 0.5);

    // Draw() disegna effettivamente l'istogramma sul canvas corrente.
    // L'opzione "HIST" disegna solo il contorno/le barre dell'istogramma,
    // senza barre di errore o marker sui punti.
    hMc->Draw("HIST");

    // SaveAs salva il contenuto del canvas su file. Il formato di output
    // e' dedotto automaticamente dall'estensione: .png produce un'immagine
    // raster, .root salva l'oggetto in formato nativo ROOT (riapribile e
    // ri-analizzabile in una sessione ROOT futura).
    cMc->SaveAs("chirp_mass_distribution.png");
    cMc->SaveAs("chirp_mass_distribution.root");

    // ---------------------------------------------------------------
    // 2) Estraggo |chi1|, |chi2| ~ Uniforme[0,1], direzioni uniformi sulla
    //    sfera unitaria (con TRandom3::Sphere), e disegno la distribuzione
    //    di chi_eff.
    // ---------------------------------------------------------------

    // Nuovo istogramma per chi_eff: 100 bin tra -1 e 1, perche' chi_eff
    // (a differenza della chirp mass) puo' assumere anche valori negativi.
    TH1D* hChiEff = new TH1D("hChiEff",
                              "Effective spin parameter distribution;#chi_{eff};Counts",
                              100, -1.0, 1.0);

    for (int i = 0; i < kN; ++i) {
        double m1 = rng.Uniform(BinaryBlackHole::kMinMass, BinaryBlackHole::kMaxMass);
        double m2 = rng.Uniform(BinaryBlackHole::kMinMass, BinaryBlackHole::kMaxMass);

        // Modulo di ciascuno spin, uniforme in [0,1] come richiesto dal testo.
        double mag1 = rng.Uniform(0.0, 1.0);
        double mag2 = rng.Uniform(0.0, 1.0);

        // TRandom3::Sphere(x, y, z, r) genera un punto casuale SULLA
        // SUPERFICIE di una sfera di raggio r: ogni punto della superficie
        // ha la stessa probabilita' di essere scelto (direzione isotropa,
        // nessuna orientazione nello spazio e' favorita rispetto alle
        // altre). La funzione non restituisce un valore con "return":
        // scrive il risultato direttamente dentro x, y, z, che le passiamo
        // ancora vuote e vengono riempite da lei (passaggio per riferimento).
        //
        // Usando r = mag_i (il modulo dello spin, gia' estratto sopra come
        // uniforme in [0,1]), il punto (x,y,z) ottenuto e' esattamente il
        // vettore di spin completo: ha modulo pari a mag_i (quello scelto
        // da noi) e direzione casuale isotropa (quella generata da Sphere).
        //
        // Dichiaro le 6 variabili (3 componenti per ciascuno dei 2 spin),
        // ancora senza valore: verranno riempite dalle due chiamate a
        // Sphere() subito sotto.
        double x1, y1, z1, x2, y2, z2;
        rng.Sphere(x1, y1, z1, mag1);   // riempie x1,y1,z1: vettore di spin1 completo
        rng.Sphere(x2, y2, z2, mag2);   // riempie x2,y2,z2: vettore di spin2 completo

        // Costruisco il sistema binario con questi spin, usando le
        // parentesi graffe {x1,y1,z1} per costruire al volo lo
        // std::array<double,3> richiesto dal costruttore.
        BinaryBlackHole bbh(m1, m2, {x1, y1, z1}, {x2, y2, z2});

        // Riempio l'istogramma con il valore di chi_eff di questo sistema,
        // calcolato dal metodo EffectiveSpin() della classe.
        hChiEff->Fill(bbh.EffectiveSpin());
    }

    // Stessa procedura di prima: creo un nuovo canvas (finestra di disegno
    // separata da quella della chirp mass), imposto colore bordo/riempimento
    // (qui rosso invece di blu, per distinguere visivamente i due plot),
    // disegno l'istogramma e salvo su file PNG e ROOT.
    TCanvas* cChiEff = new TCanvas("cChiEff", "Effective spin distribution", 800, 600);
    hChiEff->SetLineColor(kRed + 1);
    hChiEff->SetFillColorAlpha(kRed - 9, 0.5);
    hChiEff->Draw("HIST");
    // Salvo anche questo secondo canvas su file, stesso discorso di prima:
    // PNG per un'immagine visualizzabile subito, ROOT per riaprire e
    // ri-analizzare l'istogramma in una sessione ROOT futura.
    cChiEff->SaveAs("chi_eff_distribution.png");
    cChiEff->SaveAs("chi_eff_distribution.root");

    // ---------------------------------------------------------------
    // A quick sanity check / demo of the class interface: default
    // constructor, copy constructor and Print().
    // ---------------------------------------------------------------
    // Piccola verifica finale (non richiesta dal testo, ma utile per
    // controllare che l'interfaccia della classe funzioni bene):
    // creo un oggetto con il costruttore di default, lo stampo con
    // l'operator<< appena definito.
    BinaryBlackHole defaultBBH;
    std::cout << defaultBBH;

    // Creo una copia (costruttore di copia), poi la modifico con dei
    // setter: verifico cosi' che la copia sia davvero indipendente
    // dall'originale (defaultBBH non deve cambiare).
    BinaryBlackHole copyBBH(defaultBBH);
    copyBBH.SetM1(30.0);
    copyBBH.SetChi1({0.1, 0.0, 0.2});
    std::cout << copyBBH;

    // Messaggio finale a schermo, per confermare all'utente dove sono
    // stati salvati i file degli istogrammi generati sopra.
    std::cout << "Histograms saved to chirp_mass_distribution.{png,root} "
                 "and chi_eff_distribution.{png,root}\n";

}
