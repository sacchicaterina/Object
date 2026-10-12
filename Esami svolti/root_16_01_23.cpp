// ============================================================================
// COMANDO PER ESEGUIRE QUESTO FILE IN ROOT:
//
//   root -l -q root_16_01_23.cpp+
//
// ============================================================================

#include <iostream>
#include <vector>
#include <cmath>
#include <stdexcept>

#include "TObject.h"
#include "TRandom3.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TVector3.h"
#include "TTree.h"

// TH1F vs TH1D: entrambe sono classi di ROOT per istogrammi 1D, la differenza
// e' solo nella PRECISIONE con cui viene memorizzato il conteggio in ogni bin:
//   - TH1F: usa float (32 bit) per i conteggi di ogni bin
//   - TH1D: usa double (64 bit) per i conteggi di ogni bin
// (la "F" e la "D" finali indicano il tipo, come nella notazione /F e /D dei
// branch dei TTree). TH1D occupa il doppio della memoria per bin rispetto a
// TH1F, ma evita perdita di precisione quando i conteggi per bin diventano
// molto grandi o quando si sommano/pesano molti eventi (utile per fit precisi
// o istogrammi con statistiche elevate). Per un numero di eventi come i 1000
// di questo esercizio la differenza pratica e' trascurabile: si e' scelto
// TH1D qui per coerenza con l'uso di double in tutto il resto del codice
// (masse, posizioni, velocita' sono gia' tutte double). L'header TH1F.h e'
// incluso ma non viene mai usato direttamente in questo file.
static constexpr double kB = 1.380649e-23;


// TObject e' la classe BASE fondamentale di quasi tutta la libreria ROOT:
// praticamente ogni classe ROOT (TH1D, TF1, TFile, TTree, ecc.) eredita da
// TObject, direttamente o indirettamente. Fornisce funzionalita' comuni a
// tutti gli oggetti ROOT, tra cui:
//   - RTTI (Run-Time Type Information): sapere A RUNTIME che tipo e' un
//     oggetto (utile quando ROOT gestisce collezioni eterogenee o legge
//     oggetti da file senza conoscerne a priori il tipo esatto)
//   - I/O automatico: la possibilita' di essere scritto/letto su file .root
//     tramite lo streaming di ROOT (vedi Write()/WriteObject() nel main())
//   - integrazione con le strutture dati di ROOT (TList, TCollection, ecc.)
//   - un sistema di "naming" (GetName()/SetName()) e altre utility comuni
//
// Quando una classe custom come Particle o Ensemble eredita da TObject
// (public TObject qui sotto) e aggiunge ClassDef/ClassImp (vedi piu' sotto),
// "impara" tutte queste funzionalita' gratuitamente, in cambio di dover
// includere TObject.h e generare un dizionario di classe quando si compila
// con ROOT. E' il prezzo da pagare per rendere una classe C++ "nativa" per
// ROOT, invece che una semplice classe C++ che ROOT non sa interpretare da
// sola (come succede nella versione con TTree a branch scalari, dove le
// classi non hanno bisogno di ereditare da TObject).
class Particle : public TObject {

private:

    double mass;
    TVector3 position;
    TVector3 velocity;

public:

    // Costruttore di default: necessario per tre motivi concreti in questo file:
    // 1) std::vector<Particle> come membro di Ensemble: se in futuro si usasse
    //    resize(N) invece di reserve()+emplace_back, il vector dovrebbe costruire
    //    N oggetti "vuoti" subito, e per farlo serve un default constructor.
    // 2) saveEnsembleToTTree_object() lo chiama esplicitamente con "new Particle()"
    //    per creare l'istanza "contenitore" da sovrascrivere ad ogni Fill() del TTree.
    // 3) ROOT I/O: classi che ereditano da TObject con ClassDef/ClassImp richiedono
    //    un costruttore di default accessibile, perche' la lettura da file .root
    //    (streaming) deve poter creare un'istanza vuota da riempire con i dati letti.
    Particle() : mass(0.), position(0., 0., 0.), velocity(0., 0., 0.) {}

    Particle(double m, const TVector3& pos, const TVector3& vel) : mass(m), position(pos), velocity(vel) {

        if (mass <= 0.) {
            throw std::invalid_argument("Particle: mass must be positive");
        }
    }

    double getMass() const { return mass; }

    void setMass(double m) {

        if (m <= 0.) {
            throw std::invalid_argument("Particle: mass must be positive");
        }
        mass = m;
    }

    // getPosition()/getVelocity() restituiscono un intero oggetto TVector3
    // (la classe di ROOT per un vettore 3D), non i singoli numeri x, y, z.
    // Per estrarre una singola componente si usano i metodi built-in di
    // TVector3 stesso: X(), Y(), Z() (es. p.getPosition().X() = prima
    // componente della posizione). Sono metodi della LIBRERIA ROOT, non
    // scritti da noi: non serve definire getX()/getY()/getZ() dentro
    // Particle, perche' TVector3 sa gia' "spacchettarsi" da solo nelle sue
    // tre componenti. Se position fosse stata memorizzata come tre double
    // separati (invece che come TVector3), allora si', avremmo dovuto
    // scrivere noi tre getter distinti.
    const TVector3& getPosition() const { return position; }
    const TVector3& getVelocity() const { return velocity; }

    void setPosition(const TVector3& pos) { position = pos; }
    void setVelocity(const TVector3& vel) { velocity = vel; }

    // Mag2() e' un metodo di TVector3 che restituisce il quadrato della
    // magnitudine del vettore: v.Mag2() = vx^2 + vy^2 + vz^2 (cioe' |v|^2).
    // Esiste anche Mag(), che restituisce |v| (la radice quadrata di Mag2()).
    // Qui serve proprio |v|^2 per la formula dell'energia cinetica:
    //   E_k = 0.5 * m * v^2  =  0.5 * m * |v|^2
    // Usare Mag2() e' anche piu' efficiente di pow(velocity.Mag(), 2) o
    // velocity.Mag() * velocity.Mag(): Mag() calcola internamente una sqrt(),
    // che poi verrebbe annullata elevando di nuovo al quadrato. Mag2() salta
    // direttamente al risultato (vx^2+vy^2+vz^2) senza mai calcolare la radice,
    // risparmiando un'operazione costosa se ripetuta su molte particelle.
    double getKineticEnergy() const { return 0.5 * mass * velocity.Mag2(); }

    // ClassDef e ClassImp sono MACRO di ROOT (espanse dal preprocessore prima
    // della compilazione), non funzioni normali. Servono per integrare questa
    // classe nel sistema di RTTI (Run-Time Type Information) e I/O di ROOT,
    // molto piu' potente di quello standard del C++.
    //
    // ClassDef(Particle, 1) va scritta DENTRO la classe (di solito per ultima,
    // in area pubblica). Si espande generando automaticamente metodi come
    // Class(), IsA(), ClassName() che permettono a ROOT di sapere A RUNTIME
    // che tipo e' un dato oggetto (utile con collezioni polimorfe o quando
    // ROOT legge oggetti da file senza sapere a priori il tipo esatto), e
    // abilita lo STREAMING: il meccanismo che permette a ROOT di scrivere
    // l'oggetto su file .root e ricostruirlo esattamente com'era, campo per
    // campo, quando lo rileggi. Il secondo argomento (1) e' il numero di
    // VERSIONE della classe (schema versioning): se in futuro si modifica la
    // struttura di Particle e si prova a rileggere un vecchio file .root
    // salvato con la versione precedente, ROOT usa questo numero per capire
    // come convertire i dati vecchi al nuovo formato invece di fallire.
    ClassDef(Particle, 1)
};

// ClassImp(Particle) va scritta FUORI dalla classe, a livello globale, subito
// dopo la sua definizione. Si espande generando l'IMPLEMENTAZIONE effettiva
// di alcuni metodi dichiarati da ClassDef (in particolare quelli legati al
// dizionario delle classi di ROOT, la tabella interna che ROOT mantiene per
// sapere "che cos'e'" ogni classe che gestisce). Il rapporto tra ClassDef e
// ClassImp e' simile a quello tra un file header (.h, la dichiarazione) e un
// file sorgente (.cpp, la definizione): se si usasse solo ClassDef senza
// ClassImp, il codice a volte compila ma il linker fallisce, perche' mancano
// i simboli che ClassDef ha solo dichiarato.
//
// Nel main() di questo file, ClassDef/ClassImp (qui e in Ensemble) sono cio'
// che rende possibili WriteObject(), Write() e il branch a oggetto intero nel
// TTree (saveEnsembleToTTree_object): senza di esse Particle ed Ensemble
// sarebbero classi C++ "normali", salvabili solo con branch scalari puri
// (double), mai come oggetto intero scritto/riletto automaticamente da ROOT.
ClassImp(Particle)


class Ensemble : public TObject {

public:

    enum class DistKind { kUniform, kGaussian };

private:

    int N;
    std::vector<Particle> particles;
    double totalMass = 0.0;
    TVector3 centerOfMass;
    double temperature = 0.0;

    // Naming: <grandezza><P1/P2>, es. "massP1" = mass + P1 (parametro 1
    // della distribuzione usata per la MASSA). Ci sono tre distribuzioni
    // indipendenti, una per ciascuna grandezza generata per ogni particella:
    //   mass -> distribuzione per generare la MASSA
    //   pos  -> distribuzione per generare la POSIZIONE (x, y, z)
    //   vel  -> distribuzione per generare la VELOCITA' (vx, vy, vz)
    // Ognuna e' configurabile indipendentemente come gaussiana o uniforme
    // (un DistKind separato per ciascuna: massKind, posKind, velKind), quindi
    // ognuna ha bisogno anche dei propri due parametri numerici P1/P2.
    // P1/P2 = parametri della distribuzione (media/sigma se gaussiana,
    // min/max se uniforme). Vedi il commento su DrawFrom() piu' sotto
    // per il significato esatto a seconda di "kind".
    DistKind massKind = DistKind::kGaussian;
    double massP1 = 1.0;   // media (gaussiana) o minimo (uniforme) della MASSA
    double massP2 = 0.1;   // sigma (gaussiana) o massimo (uniforme) della MASSA

    DistKind posKind = DistKind::kGaussian;
    double posP1 = 0.0;    // media (gaussiana) o minimo (uniforme) della POSIZIONE
    double posP2 = 1.0;    // sigma (gaussiana) o massimo (uniforme) della POSIZIONE

    DistKind velKind = DistKind::kGaussian;
    double velP1 = 0.0;    // media (gaussiana) o minimo (uniforme) della VELOCITA'
    double velP2 = 1.0;    // sigma (gaussiana) o massimo (uniforme) della VELOCITA'

    TRandom3 rng;


    // DrawFrom: "to draw" in inglese, oltre a "disegnare", significa anche
    // "estrarre/campionare" un valore casuale da una distribuzione (come
    // "estrarre un numero" in una lotteria). DrawFrom(kind, p1, p2) quindi
    // significa "estrai un valore dalla distribuzione di tipo kind, con
    // parametri p1, p2": genera un singolo numero casuale campionandolo
    // dalla distribuzione scelta (gaussiana o uniforme), usando il
    // generatore rng condiviso da tutta la classe Ensemble.
    //
    // p1 e p2 sono i due parametri generici della distribuzione statistica
    // scelta: il loro significato cambia a seconda di "kind", perche' questa
    // funzione e' condivisa tra i due tipi di distribuzione supportati:
    //   - se kind == kGaussian: p1 = media (mean), p2 = deviazione standard
    //     (sigma), passati a TRandom3::Gaus(mean, sigma)
    //   - se kind == kUniform:  p1 = minimo (min),  p2 = massimo (max)
    //     dell'intervallo, passati a TRandom3::Uniform(min, max)
    // Non si chiamano "mean"/"sigma" perche' sarebbe un nome scorretto quando
    // la funzione sta invece campionando da una distribuzione uniforme.
    double DrawFrom(DistKind kind, double p1, double p2) {
        if (kind == DistKind::kGaussian) {
            return rng.Gaus(p1, p2);
        } else {
            return rng.Uniform(p1, p2);
        }
    }


    void computeGlobalProperties() {

        totalMass = 0.0;
        // SetXYZ() e' un altro metodo built-in di TVector3 (come X(), Y(),
        // Z(), ma nella direzione opposta: invece di LEGGERE le tre
        // componenti, le IMPOSTA tutte e tre in un colpo solo).
        // centerOfMass.SetXYZ(0., 0., 0.) equivale a scrivere a mano
        // centerOfMass.SetX(0.); centerOfMass.SetY(0.); centerOfMass.SetZ(0.);
        // ma in una singola chiamata. Qui azzera il vettore centerOfMass
        // prima di ricalcolarlo da capo nel ciclo sottostante.
        centerOfMass.SetXYZ(0., 0., 0.);
        double totalKineticEnergy = 0.0;

        for (const auto& p : particles) {
            totalMass += p.getMass();
            centerOfMass += p.getMass() * p.getPosition();
            totalKineticEnergy += p.getKineticEnergy();
        }

        if (totalMass > 0.) {
            centerOfMass *= (1.0 / totalMass);
        }

        double avgKineticEnergy = totalKineticEnergy / N;
        temperature = (2.0 / 3.0) * avgKineticEnergy / kB;
    }


public:

    Ensemble() : N(0) {}

    explicit Ensemble(int num) : N(num) {
        if (num <= 0) {
            throw std::invalid_argument("Ensemble: number of particles must be positive");
        }
    }

    int getN() const { return N; }
    const std::vector<Particle>& getParticles() const { return particles; }

    // Getter per una singola particella per indice. Usa vector::at() invece
    // di operator[]: at() controlla i limiti e lancia std::out_of_range se
    // l'indice non e' valido, mentre operator[] non fa alcun controllo e
    // darebbe undefined behavior su un indice fuori range. getParticles()
    // resta utile per iterare su tutte le particelle (come nei cicli for
    // del main()); getParticle(i) e' comodo per accedere a un singolo
    // elemento per indice senza passare per il vettore intero ogni volta.
    const Particle& getParticle(int i) const { return particles.at(i); }

    double getTotalMass() const { return totalMass; }
    const TVector3& getCenterOfMass() const { return centerOfMass; }
    double getTemperature() const { return temperature; }

    void setMassDistribution(DistKind kind, double p1, double p2) {
        massKind = kind;
        massP1 = p1;
        massP2 = p2;
    }

    void setPositionDistribution(DistKind kind, double p1, double p2) {
        posKind = kind;
        posP1 = p1;
        posP2 = p2;
    }

    void setVelocityDistribution(DistKind kind, double p1, double p2) {
        velKind = kind;
        velP1 = p1;
        velP2 = p2;
    }

    void generate() {

        particles.clear();
        particles.reserve(N);

        for (int i = 0; i < N; i++) {
            double m = DrawFrom(massKind, massP1, massP2);

            TVector3 pos;
            for (int j = 0; j < 3; j++) {
                pos[j] = DrawFrom(posKind, posP1, posP2);
            }

            TVector3 vel;
            for (int j = 0; j < 3; j++) {
                vel[j] = DrawFrom(velKind, velP1, velP2);
            }

            particles.emplace_back(m, pos, vel);

            // Alternativa equivalente con push_back():
            //   particles.push_back(Particle(m, pos, vel));
            // Differenza: push_back() richiede di costruire prima un oggetto
            // Particle temporaneo (qui, con Particle(m, pos, vel)), e poi lo
            // COPIA (o sposta, se disponibile un move constructor) dentro il
            // vector. emplace_back() invece costruisce l'oggetto DIRETTAMENTE
            // nello spazio di memoria del vector, passando gli argomenti (m,
            // pos, vel) al costruttore di Particle "sul posto" (in-place),
            // senza creare una copia temporanea intermedia. Per questo
            // emplace_back() e' generalmente piu' efficiente quando si
            // costruisce l'oggetto direttamente con i suoi argomenti, come
            // in questo caso.
        }

        computeGlobalProperties();
    }

    // Stesso meccanismo spiegato sopra per Particle: ClassDef abilita RTTI e
    // streaming per Ensemble (versione schema 1), ClassImp ne fornisce
    // l'implementazione nel dizionario delle classi di ROOT. Necessari perche'
    // anche Ensemble viene salvato per intero con WriteObject()/Write() nel main().
    ClassDef(Ensemble, 1)
};
ClassImp(Ensemble)




// ===================== SALVATAGGIO CON TTree =====================

// Versione A: colonne scalari (m, x, y, z, vx, vy, vz separate) -- possibile sempre
void saveEnsembleToTTree_scalar(const Ensemble& ens, const std::string& filename) {

    // In C++ esistono due modi diversi di rappresentare una stringa:
    //   - std::string: quello moderno, usato qui come tipo del parametro
    //     "filename".
    //   - const char*: quello "vecchio stile", ereditato dal linguaggio C.
    // Sono due tipi diversi e non si convertono automaticamente l'uno
    // nell'altro. TFile pero' e' una classe di ROOT scritta prima che
    // std::string fosse lo standard (ROOT e' molto piu' vecchio), quindi
    // il suo costruttore vuole un const char*, non una std::string.
    // filename.c_str() e' un metodo di std::string (come .size() o .empty())
    // che restituisce "la versione const char* della stessa stringa": serve
    // a "tradurre" filename nel formato che TFile si aspetta di ricevere.
    TFile fout(filename.c_str(), "RECREATE");

    double m, x, y, z, vx, vy, vz;

    // Branch e' un METODO della classe TTree (chiamata tree.Branch(...),
    // sintassi oggetto.metodo(argomenti)). Un TTree e' organizzato come una
    // tabella: ogni riga rappresenta un "evento" (qui, una particella), e
    // ogni colonna rappresenta una variabile diversa (massa, x, y, z, ecc.).
    // Queste colonne si chiamano "branch" (letteralmente "ramo": TTree sta
    // per "Tree", albero). tree.Branch(nome, indirizzo, tipo) crea una nuova
    // colonna, dicendo a ROOT:
    //   - "m"    -> il nome della colonna (branch)
    //   - &m     -> l'INDIRIZZO della variabile C++ da cui leggere il valore
    //               ogni volta che si chiama tree.Fill()
    //   - "m/D"  -> il tipo di dato nella notazione di ROOT (D=double,
    //               F=float, I=int, ecc.)
    TTree tree("particles", "Ensemble particle data (scalar branches)");
    tree.Branch("m",  &m,  "m/D");
    tree.Branch("x",  &x,  "x/D");
    tree.Branch("y",  &y,  "y/D");
    tree.Branch("z",  &z,  "z/D");
    tree.Branch("vx", &vx, "vx/D");
    tree.Branch("vy", &vy, "vy/D");
    tree.Branch("vz", &vz, "vz/D");

    for (const auto& p : ens.getParticles()) {
        m = p.getMass();
        x = p.getPosition().X();
        y = p.getPosition().Y();
        z = p.getPosition().Z();
        vx = p.getVelocity().X();
        vy = p.getVelocity().Y();
        vz = p.getVelocity().Z();

        // Branch() ha detto a ROOT DOVE guardare (l'indirizzo di m, x, ecc.),
        // ma non ha copiato subito nessun valore. Ogni volta che si chiama
        // Fill(), ROOT legge il valore ATTUALE contenuto in quegli indirizzi
        // di memoria e lo salva come nuova riga della tabella. Per questo si
        // aggiornano prima le variabili (m = p.getMass(); ecc.) e SOLO DOPO
        // si chiama Fill(): chiamare Fill() prima di aggiornarle salverebbe
        // il valore vecchio, rimasto dall'iterazione precedente.
        tree.Fill();
    }

    tree.Write();
    fout.Close();
}


// Versione B: righe con l'intera Particle -- possibile solo con TObject
void saveEnsembleToTTree_object(const Ensemble& ens, const std::string& filename) {

    // Stesso motivo di c_str() spiegato in saveEnsembleToTTree_scalar sopra:
    // TFile vuole un const char*, non una std::string.
    TFile fout(filename.c_str(), "RECREATE");

    Particle* particlePtr = new Particle();

    // Stesso metodo Branch() spiegato in saveEnsembleToTTree_scalar sopra,
    // ma qui il branch non contiene un singolo double: contiene un intero
    // OGGETTO Particle per riga (possibile solo perche' Particle eredita da
    // TObject e ha ClassDef/ClassImp, che permettono a ROOT di sapere come
    // serializzare l'intero oggetto in quella colonna). L'indirizzo passato
    // e' quello del PUNTATORE particlePtr (&particlePtr, un puntatore a
    // puntatore): ROOT lo aggiorna per farlo puntare, ad ogni Fill(), al
    // contenuto che deve scrivere in quella riga.
    TTree tree("particles", "Ensemble particle data (whole objects)");
    tree.Branch("particle", &particlePtr);

    for (const auto& p : ens.getParticles()) {
        *particlePtr = p;
        tree.Fill();
    }

    tree.Write();
    fout.Close();
    delete particlePtr;
}




void root_16_01_23() {

    // 1. Creo l'ensemble con 1000 particelle
    Ensemble ens(1000);

    // 2. Imposto le distribuzioni gaussiane per massa, posizione, velocità
    //    (parametri arbitrari ma sensati: media, sigma)
    ens.setMassDistribution(Ensemble::DistKind::kGaussian, 2.0, 0.3);
    ens.setPositionDistribution(Ensemble::DistKind::kGaussian, 0.0, 5.0);
    ens.setVelocityDistribution(Ensemble::DistKind::kGaussian, 0.0, 2.0);

    // 3. Genero le particelle
    ens.generate();

    const auto& particles = ens.getParticles();

    // 4. Creo istogrammi, li riempio, li fitto con una gaussiana, li salvo come .png

    // ===================== MASSA =====================

    // creo l'istogramma vuoto: nome, titolo;etichettaX;etichettaY, n.bin, min, max
    TH1D hMass("hMass", "Mass distribution;mass;counts", 100, 0.8, 3.2);

    // riempio l'istogramma: per ogni particella, aggiungo la sua massa
    for (const auto& p : particles) {
        hMass.Fill(p.getMass());
    }

    // definisco la funzione di fit: nome, "gaus" = formula gaussiana predefinita, range
    TF1 fMass("fMass", "gaus", 0.8, 3.2);
    // eseguo il fit sull'istogramma; "Q" = quiet, niente output verboso
    hMass.Fit(&fMass, "Q");

    // creo la tela di disegno: nome, titolo, larghezza, altezza (px)
    TCanvas cMass("cMass", "Mass distribution", 800, 600);
    // disegno l'istogramma
    hMass.Draw();
    // disegno il fit sopra, senza cancellare l'istogramma
    fMass.Draw("same");
    // salvo la tela come immagine
    cMass.SaveAs("mass.png");

    // GetParameter() e' un metodo BUILT-IN della classe TF1 (libreria ROOT),
    // come X()/Y()/Z() di TVector3. Restituisce il valore stimato di un
    // parametro della funzione, DOPO che il fit e' stato eseguito con Fit().
    // Gli indici (0, 1, 2, ...) si riferiscono ai parametri della formula
    // usata per costruire fMass. Qui la formula e' "gaus", una funzione
    // PREDEFINITA di ROOT che rappresenta una gaussiana standard:
    //   f(x) = p0 * exp(-0.5 * ((x - p1) / p2)^2)
    // dove ROOT indicizza automaticamente i tre parametri come:
    //   p0 (indice 0) -> normalizzazione/ampiezza
    //   p1 (indice 1) -> media (mean)
    //   p2 (indice 2) -> sigma (deviazione standard)
    // Quindi GetParameter(1) da' la media stimata, GetParameter(2) la sigma
    // stimata. I valori restituiti sono quelli trovati da hMass.Fit(&fMass,
    // "Q") qui sopra: Fit() esegue l'algoritmo di minimizzazione che trova i
    // valori di p0, p1, p2 che meglio approssimano i dati dell'istogramma, e
    // li memorizza dentro l'oggetto fMass; GetParameter(i) li legge da li'.
    // (Esiste anche GetParError(i), non usato qui, che darebbe l'incertezza
    // statistica su quel parametro, se servisse.)
    // stampo media e sigma trovati dal fit (parametro 1 e 2 della gaussiana, 0 è la normalizzazione)
    std::cout << "Mass fit: mean=" << fMass.GetParameter(1)
              << " sigma=" << fMass.GetParameter(2) << std::endl;

    // ===================== POSIZIONE X, Y, Z =====================

    // creo i tre istogrammi vuoti, uno per componente
    TH1D hX("hX", "X position;x;counts", 100, -20, 20);
    TH1D hY("hY", "Y position;y;counts", 100, -20, 20);
    TH1D hZ("hZ", "Z position;z;counts", 100, -20, 20);

    // riempio i tre istogrammi in un solo ciclo sulle particelle
    for (const auto& p : particles) {
        hX.Fill(p.getPosition().X());
        hY.Fill(p.getPosition().Y());
        hZ.Fill(p.getPosition().Z());
    }

    // --- fit e plot di X ---
    TF1 fX("fX", "gaus", -20, 20);
    hX.Fit(&fX, "Q");
    TCanvas cX("cX", "X position", 800, 600);
    hX.Draw();
    fX.Draw("same");
    cX.SaveAs("x.png");
    std::cout << "X fit: mean=" << fX.GetParameter(1)
              << " sigma=" << fX.GetParameter(2) << std::endl;

    // --- fit e plot di Y ---
    TF1 fY("fY", "gaus", -20, 20);
    hY.Fit(&fY, "Q");
    TCanvas cY("cY", "Y position", 800, 600);
    hY.Draw();
    fY.Draw("same");
    cY.SaveAs("y.png");
    std::cout << "Y fit: mean=" << fY.GetParameter(1)
              << " sigma=" << fY.GetParameter(2) << std::endl;

    // --- fit e plot di Z ---
    TF1 fZ("fZ", "gaus", -20, 20);
    hZ.Fit(&fZ, "Q");
    TCanvas cZ("cZ", "Z position", 800, 600);
    hZ.Draw();
    fZ.Draw("same");
    cZ.SaveAs("z.png");
    std::cout << "Z fit: mean=" << fZ.GetParameter(1)
              << " sigma=" << fZ.GetParameter(2) << std::endl;

    // ===================== VELOCITÀ VX, VY, VZ =====================

    // creo i tre istogrammi vuoti, uno per componente
    TH1D hVx("hVx", "Vx distribution;vx;counts", 100, -8, 8);
    TH1D hVy("hVy", "Vy distribution;vy;counts", 100, -8, 8);
    TH1D hVz("hVz", "Vz distribution;vz;counts", 100, -8, 8);

    // riempio i tre istogrammi in un solo ciclo sulle particelle
    for (const auto& p : particles) {
        hVx.Fill(p.getVelocity().X());
        hVy.Fill(p.getVelocity().Y());
        hVz.Fill(p.getVelocity().Z());
    }

    // --- fit e plot di Vx ---
    TF1 fVx("fVx", "gaus", -8, 8);
    hVx.Fit(&fVx, "Q");
    TCanvas cVx("cVx", "Vx distribution", 800, 600);
    hVx.Draw();
    fVx.Draw("same");
    cVx.SaveAs("vx.png");
    std::cout << "Vx fit: mean=" << fVx.GetParameter(1)
              << " sigma=" << fVx.GetParameter(2) << std::endl;

    // --- fit e plot di Vy ---
    TF1 fVy("fVy", "gaus", -8, 8);
    hVy.Fit(&fVy, "Q");
    TCanvas cVy("cVy", "Vy distribution", 800, 600);
    hVy.Draw();
    fVy.Draw("same");
    cVy.SaveAs("vy.png");
    std::cout << "Vy fit: mean=" << fVy.GetParameter(1)
              << " sigma=" << fVy.GetParameter(2) << std::endl;

    // --- fit e plot di Vz ---
    TF1 fVz("fVz", "gaus", -8, 8);
    hVz.Fit(&fVz, "Q");
    TCanvas cVz("cVz", "Vz distribution", 800, 600);
    hVz.Draw();
    fVz.Draw("same");
    cVz.SaveAs("vz.png");
    std::cout << "Vz fit: mean=" << fVz.GetParameter(1)
              << " sigma=" << fVz.GetParameter(2) << std::endl;

    // ===================== SALVATAGGIO ENSEMBLE =====================

    // apro un file ROOT in scrittura (RECREATE = sovrascrive se esiste già)
    TFile fout("ensemble.root", "RECREATE");

    // --- Opzione A: WriteObject() -----------------------------------
    // metodo di TFile, chiamato SUL FILE, passandogli il puntatore all'oggetto
    // e il nome/chiave con cui salvarlo. Funziona anche con oggetti che
    // NON ereditano da TObject (con alcune limitazioni), ed è esplicito
    // su quale file stai usando.
    //
    // Prende DUE argomenti:
    //   1) &ens: l'INDIRIZZO (puntatore) dell'oggetto da salvare. ens e'
    //      dichiarato come variabile normale (Ensemble ens(1000);, non un
    //      puntatore); l'operatore & davanti a una variabile ne restituisce
    //      l'indirizzo di memoria, trasformandola al volo in un puntatore
    //      (&ens e' di tipo Ensemble*). WriteObject vuole un puntatore (non
    //      l'oggetto per valore) perche' deve leggere/serializzare i dati a
    //      quell'indirizzo per scriverli sul file, senza fare una copia
    //      inutile dell'oggetto.
    //   2) "ensemble": il NOME/CHIAVE con cui l'oggetto viene salvato dentro
    //      il file .root. Un file ROOT funziona come un "archivio" con piu'
    //      oggetti dentro, ciascuno identificato da una stringa-chiave (come
    //      dare un nome a un file dentro una cartella zip). Questa stessa
    //      stringa serve poi per RILEGGERE l'oggetto dal file (es. con
    //      TFile::Get("ensemble")). Serve un nome perche' potresti salvare
    //      piu' oggetti dello stesso tipo (o piu' versioni dello stesso
    //      oggetto, vedi Write() sotto) nello stesso file, e ti serve un modo
    //      per distinguerli.
    fout.WriteObject(&ens, "ensemble");

    // --- Opzione B: Write() -----------------------------------------
    // metodo di TObject, chiamato DIRETTAMENTE SULL'OGGETTO da salvare.
    // Richiede che l'oggetto erediti da TObject (Ensemble lo fa, grazie
    // a ClassDef/ClassImp). Scrive nel file "corrente" attualmente aperto.
    //
    // Prende UN SOLO argomento: "ensemble_v2", il nome/chiave con cui salvare
    // l'oggetto (stesso concetto di chiave spiegato sopra per WriteObject).
    // Non serve passare un indirizzo/puntatore qui, perche' Write() e' un
    // metodo CHIAMATO SULL'OGGETTO stesso (ens.Write(...), non fout.Write()):
    // l'oggetto "sa gia' chi e'" (e' se stesso), quindi non serve dirglielo
    // esplicitamente come con WriteObject. "Nel file corrente attualmente
    // aperto" significa che Write() scrive nell'ultimo TFile aperto nel
    // programma (qui, fout), senza bisogno di chiamarlo esplicitamente su
    // fout come si fa con WriteObject.
    //
    // Risultato pratico: nello stesso file ensemble.root finiscono DUE copie
    // salvate dello stesso oggetto ens, distinguibili per nome ("ensemble"
    // da WriteObject, "ensemble_v2" da Write") - due modi diversi di ottenere
    // lo stesso risultato, mostrati qui a confronto.
    ens.Write("ensemble_v2");

    // ---Opzione C: Salvataggio tramite TTree -------------------------
    saveEnsembleToTTree_scalar(ens, "ensemble_scalar.root");
    saveEnsembleToTTree_object(ens, "ensemble_object.root");

    // chiudo il file
    fout.Close();

    std::cout << "Fatto. Plot salvati come .png, ensemble salvato in ensemble.root" << std::endl;

}
