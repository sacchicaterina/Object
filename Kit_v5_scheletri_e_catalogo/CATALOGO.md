# CATALOGO DEI PEZZI — C++

## Come si usa

1. Parti dallo **scheletro pulito** del gruppo giusto (cartelle `A_classe_singola`, `B_composizione`, `C_operatori`, `D_gerarchia`, `G_function`, `H_strategy`).
2. Leggi il testo e sottolinea ogni richiesta.
3. Per ogni richiesta cerca qui (**Cmd+F**) una parola della frase inglese del testo, oppure guarda l'**INDICE**.
4. Ogni scheda dice **dove** va ogni pezzo:
   - **NEL Nome.h → COSTRUTTORI** = nel file `Nome.h`, in fondo alla sezione `COSTRUTTORI`
   - **NEL app.cpp** = nel main, al posto della riga `// (nuovi requisiti: ...)`
   - **NEL ... → INCLUDE** = sotto gli altri `#include` di quel file
   - **NEL app.cpp → FUNZIONI DEL MAIN** = sopra `int main()`, nella sezione con quel nome
   - **SOSTITUISCI in Nome.cc → \`riga\`** = cerca quella riga e metti al suo posto il pezzo nuovo
   - **NUOVO FILE** = crea il file nella cartella (VS Code: File > New File)
5. Copia i pezzi, poi **rinomina** con "Match Whole Word" (Alt+Cmd+W): `Nome` → `Capacitor`, `a` → `C`, ...
6. **Compila dopo ogni scheda.** Se si rompe, sai quale è stata.

Le schede con **Richiede:** vanno fatte DOPO quelle indicate.
Ogni scheda di questo catalogo è stata provata: copiata nello scheletro, compilata con `-Wall -Wextra` senza warning, ed eseguita.

---

## INDICE

| Scheda | Cosa | Il testo dice... |
|---|---|---|
| **I0** | Quale `#include` per cosa | — |
| | **COSTRUTTORI** | |
| K1 | Costruttore di default | "default constructor" |
| K2 | Default che chiede i dati | "asks the user", "from the keyboard" |
| K3 | Costruttore di copia | "copy constructor" |
| K4 | Costruttore con un solo argomento | "constructor that takes only ..." |
| K5 | Valore di default di un argomento | "if not specified", "by default ... = 1" |
| K6 | Seconda via: dalla formula | "can be linked to ... by the formula" |
| K7 | Named constructor | "build from X or from Y" (stessi tipi) |
| | **ATTRIBUTI** | |
| T1 | Aggiungere un attributo numerico | una grandezza in più (spin, carica, ...) |
| T2 | Vettore 3D (`std::array`) | "vector", "position", "spin vector", "3 components" |
| T3 | Elenco di numeri (`std::vector`) | "list of values", "N measurements" |
| T4 | Grandezza memorizzata + `update()` | "store ... as an attribute", "data member" |
| T5 | Contatore di oggetti | "number of instances", "how many objects exist" |
| T6 | Costanti: π, ε₀, G, ... | una costante fisica nella formula |
| | **METODI** | |
| M1 | Metodo che restituisce un oggetto nuovo | "returns the inverse", "returns a new ..." |
| M2 | Scrivere su file | "write to a text file", "save" |
| M3 | Controlli con intervalli | "must be in [a, b]", "(0, 1]" |
| M4 | Formule: potenze, radici, esponenziali | formula con potenze frazionarie |
| | **COMPOSIZIONE** (scheletro B) | |
| B0 | Seconda grandezza dell'elemento | "mass and spin", "charge and mass" |
| B1 | Posizione dell'elemento | "located at", "position" |
| B2 | Contributo vettoriale in un punto + somma | "electric field at", "total force", "superposition" |
| B3 | Contributo scalare in un punto + somma | "potential at", "energy" |
| B4 | Aggiungere e togliere elementi | "add", "remove the i-th" |
| B5 | Costruttore che legge da file | "reads ... from a file" |
| B6 | Costruttore che chiede N elementi | "asks the user for N ..." |
| B7 | Media, massimo, minimo | "mean", "largest", "smallest" |
| B8 | Contare con una condizione | "how many ... above ..." |
| B9 | Ordinare | "sort" |
| B10 | Centro pesato (di massa, di carica) | "center of mass" |
| B11 | Totale memorizzato nel contenitore | "store the total" |
| B12 | Salvare gli elementi su file | "write ... to a file" |
| B13 | Grandezza della coppia memorizzata | "store chi_eff", "effective spin" |
| | **OPERATORI** (scheletro C) | |
| O1 | `-` `*` `/` tra due oggetti | "overload -, *, /" |
| O2 | Oggetto × numero e numero × oggetto | "multiply by a number", "2 * a" |
| O3 | `+=` `-=` `*=` | "+=", "compound" |
| O4 | `==` `!=` `<` | "compare", "equal", "sort" |
| O5 | Meno unario `-a` | "opposite" |
| O6 | Serie e parallelo (`+` e `||`) | "series", "parallel", "||" |
| O7 | Indice `a[i]` | "access the i-th", "[]" |
| O8 | Memoria dinamica (regola del tre) | "new", "dynamic memory" |
| O9 | Classi con più attributi (Frac, complessi) | "fraction", "numerator", "complex" |
| | **GERARCHIE** (scheletro D: gruppi D, E, F) | |
| D1 | Un'altra derivata (sorella) | "Triangle and Rectangle derive from Shape" |
| D2 | Derivata di secondo livello | "Cube derives from Square", "extends" |
| D3 | Virtuale con versione di default | "can be redefined", "name of the shape" |
| D4 | Metodo comune nella base | "for every ...", "energy = Q^2/2C for all" |
| D5 | Attributo protected | "accessible by derived classes" |
| D6 | Elenco polimorfico `vector<Base*>` | "store different shapes", "loop over all" |
| D7 | Contatore nella gerarchia | "number of objects existing" |
| D8 | Dati nella base, calcoli nelle derivate | "DataSet", "arithmetic / geometric / harmonic mean" |
| D9 | "Q oppure ΔV": `enum class` | "either the charge or the voltage" |
| D10 | Regole ed errori delle gerarchie | — |
| D11 | Funzione che accetta qualunque derivata | "a function that works for any shape" |
| | **FUNCTION DATA DAL TESTO** (scheletro G) | |
| G1 | Distruttore virtuale nella classe del testo | (manca nel testo) |
| G2 | Polinomio (coefficienti, grado memorizzato) | "Polynomial", "coefficients", "degree" |
| G3 | Funzione fatta di altre funzioni | "Rational = N/D", "sum", "composition" |
| G4 | Metodo in più: primitiva | "primitive", "integral" |
| G5 | Controllo: derivata numerica | — (verifica di `deriv`) |
| G6 | Tabella x, f, f' su file | "write ... for N points", "plot" |
| G7 | Parametri della funzione con setter | "A sin(ωx)", "amplitude", "frequency" |
| G8 | Funzione di più variabili | "value(x, y, z)", "deriv(..., var)" |
| | **STRATEGY** (scheletro H) | |
| S1 | Runge-Kutta 2 | "RK2", "second-order Runge-Kutta" |
| S2 | Parametri da `params.ini` | "parameter file", "params.ini" |
| S3 | Confronto tra integratori | "compare the integrators" |
| S4 | Un altro sistema fisico (orbita) | "satellite", "gravitational field" |
| S5 | Controllo: energia conservata | — (verifica fisica) |
| | **PROGRAMMA** (il main) | |
| P1 | Numeri casuali e file `.dat` | "draw random values", "store in a text file" |
| P2 | Leggere un file di dati nel main | "read the values from ..." |
| P3 | Unità comode (pF, nC) | valori in pF, nC, km ... |

---

## I0 · QUALE INCLUDE PER COSA

Nel **.h** solo ciò che serve alle **firme** (tipi di argomenti, ritorni, attributi). Nel **.cc** ciò che serve nei **corpi**.

| Uso | Include |
|---|---|
| `std::ostream` (print, `operator<<`) | `<ostream>` |
| `std::cout`, `std::cin`, `std::cerr` | `<iostream>` |
| `std::string` | `<string>` |
| `std::vector` | `<vector>` |
| `std::array` | `<array>` |
| `std::size_t` | `<cstddef>` |
| `std::invalid_argument`, `std::out_of_range`, `std::domain_error`, `std::runtime_error` | `<stdexcept>` |
| `std::pow`, `std::sqrt`, `std::exp`, `std::sin`, `std::acos`, `std::fabs` | `<cmath>` |
| `std::ifstream` (leggere), `std::ofstream` (scrivere) | `<fstream>` |
| `std::sort`, `std::max`, `std::min` | `<algorithm>` |
| `std::gcd` (MCD), `std::accumulate` | `<numeric>` |
| numeri casuali (`std::mt19937`, distribuzioni) | `<random>` |
| `std::setprecision`, `std::fixed` | `<iomanip>` |
| `std::map` | `<map>` |

---

# COSTRUTTORI

## K1 · COSTRUTTORE DI DEFAULT
**Il testo dice:** "default constructor", "with default values"  
**Scheletro:** A — vale per ogni classe: cambia `Nome` e gli attributi.

**NEL Nome.h** → COSTRUTTORI
```cpp
    // Di default: label = "senza nome", a = 1
    Nome();
```

**NEL Nome.cc** → COSTRUTTORI
```cpp
// Di default: label = "senza nome", a = 1
Nome::Nome()
    : label_("senza nome")       // un valore per OGNI attributo,
    , a_(1.0)                    // nell'ordine in cui sono dichiarati
{
}
```

**NEL app.cpp**
```cpp
        // Rk: costruttore di default
        std::cout << "=== Costruttore di default ===\n";
        Nome d;                                   // SENZA parentesi: "Nome d();" non crea un oggetto!
        std::cout << d << "   (atteso senza nome: a = 1)\n\n";
```

> **Attenzione:** i valori di default scrivili anche nel commento del `.h`: chi legge l'header deve saperli.

---

## K2 · COSTRUTTORE DI DEFAULT CHE CHIEDE I DATI
**Il testo dice:** "default constructor that asks the user", "from the keyboard", "interactively"  
**Scheletro:** A

**NEL Nome.h** → COSTRUTTORI
```cpp
    // Di default: chiede a all'utente da tastiera
    Nome();
```

**NEL Nome.cc** → INCLUDE
```cpp
#include <iostream>    // std::cout, std::cin
```

**NEL Nome.cc** → COSTRUTTORI
```cpp
// Di default: chiede a all'utente
Nome::Nome()
    : label_("da tastiera")
    , a_(0.0)                    // valore provvisorio, sovrascritto subito sotto
{
    std::cout << "Inserisci a [unita']: ";
    std::cin >> a_;
    if (!std::cin)               // l'utente ha scritto qualcosa che non e' un numero
    {
        throw std::invalid_argument("input non numerico");
    }
    controlla(a_);               // anche i dati dell'utente vanno controllati
}
```

**NEL app.cpp**
```cpp
        // Rk: costruttore che chiede i dati
        std::cout << "=== Costruttore da tastiera ===\n";
        Nome d;
        std::cout << d << "\n\n";
```

---

## K3 · COSTRUTTORE DI COPIA
**Il testo dice:** "copy constructor"  
**Scheletro:** A

**NEL Nome.h** → COSTRUTTORI
```cpp
    // Di copia: un oggetto nuovo uguale a other
    Nome(const Nome& other);

    // Assegnazione (a = b): chi scrive la copia la dichiara sempre, altrimenti warning.
    // "= default" = fai tu, compilatore, la copia attributo per attributo. Niente nel .cc.
    Nome& operator=(const Nome& other) = default;
```

**NEL Nome.cc** → COSTRUTTORI
```cpp
// Di copia: copio ogni attributo di other (CON il trattino: other.a_)
Nome::Nome(const Nome& other)
    : label_(other.label_)
    , a_(other.a_)
{
}
```

**NEL app.cpp**
```cpp
        // Rk: costruttore di copia
        std::cout << "=== Costruttore di copia ===\n";
        Nome y(x);                                // y e' una copia di x
        y.setA(100.0);                            // cambio la copia...
        std::cout << "copia: " << y << ", originale: " << x << "   (atteso 100 e 4: indipendenti)\n\n";
```

---

## K4 · COSTRUTTORE CON UN SOLO ARGOMENTO
**Il testo dice:** "a constructor that takes only a", "if only a is given"  
**Scheletro:** A

**NEL Nome.h** → COSTRUTTORI
```cpp
    // Solo a: label = "senza nome". Un solo argomento -> explicit
    explicit Nome(double a);
```

**NEL Nome.cc** → COSTRUTTORI
```cpp
// Solo a: delega al costruttore regolare (che fa anche i controlli)
Nome::Nome(double a)
    : Nome("senza nome", a)
{
}
```

**NEL app.cpp**
```cpp
        // Rk: costruttore con un argomento
        std::cout << "=== Un argomento ===\n";
        Nome u(5.0);
        std::cout << u << "   (atteso senza nome: a = 5)\n\n";
```

> **Regola di `explicit`:** costruttore che si può chiamare con **un solo** argomento → `explicit` (nel `.h`, mai nel `.cc`). Impedisce al compilatore di trasformare da solo un numero in un oggetto.

---

## K5 · VALORE DI DEFAULT DI UN ARGOMENTO
**Il testo dice:** "if not specified, εr = 1", "by default", "optional"

Non serve un costruttore nuovo: si scrive `= valore` **nel .h**, sull'**ultimo** argomento (o sugli ultimi).

```cpp
    Nome(double x, double y, double z = 1.0);     // nel .h: z vale 1 se non lo passi
```
```cpp
Nome::Nome(double x, double y, double z)          // nel .cc: SENZA "= 1.0"
```
Nel main valgono entrambe: `Nome n(1.0, 2.0);` e `Nome n(1.0, 2.0, 4.0);`.

> **Regole:** il default solo nel `.h`; gli argomenti con default vanno **in fondo**. Se dopo il default la chiamata può avere **un solo** argomento, metti `explicit`. Esempio completo: scheda K6.

---

## K6 · SECONDA VIA: COSTRUTTORE DALLA FORMULA
**Il testo dice:** "C can be linked to A and s by the formula ...", "can also be built from ..."  
**Scheletro:** C — esempio: Capacitor, `C = eps0 * epsr * A / s`, con epsr = 1 di default.

L'attributo resta **uno solo** (`a_`): x, y, z servono solo a calcolarlo, non si memorizzano.

**NEL Nome.h** → COSTRUTTORI
```cpp
    // Dalla formula:  a = EPS0 * z * x / y
    // x [m^2] > 0,  y [m] > 0,  z >= 1 (default 1 = vuoto)
    Nome(double x, double y, double z = 1.0);
```

**NEL Nome.cc** → COSTANTI
```cpp
const double EPS0 = 8.854e-12;     // permittivita' del vuoto [F/m]
```

**NEL Nome.cc** → COSTRUTTORI
```cpp
// Dalla formula:  a = EPS0 * z * x / y
Nome::Nome(double x, double y, double z)       // niente "= 1.0" qui
    : a_(0.0)
{
    if (x <= 0.0)                              // un if per ogni vincolo
    {
        throw std::invalid_argument("x deve essere > 0");
    }
    if (y <= 0.0)
    {
        throw std::invalid_argument("y deve essere > 0");
    }
    if (z < 1.0)                               // z >= 1 valido -> sbagliato se < 1
    {
        throw std::invalid_argument("z deve essere >= 1");
    }
    setA(EPS0 * z * x / y);                    // la formula; il setter controlla a
}
```

**NEL app.cpp**
```cpp
        // Rk: costruttore dalla formula
        std::cout << "=== Dalla formula ===\n";
        Nome f(1.0, 1e-3);                        // z = 1 di default
        Nome f4(1.0, 1e-3, 4.0);
        std::cout << f << "   (atteso 8.854e-12 * 1 / 1e-3 = 8.854e-09)\n";
        std::cout << f4 << "   (atteso 4 volte tanto = 3.5416e-08)\n";
        try
        {
            Nome g(1.0, 1e-3, 0.5);
            std::cout << "ERRORE: doveva lanciare un'eccezione\n";
        }
        catch (const std::invalid_argument& e)
        {
            std::cout << "Eccezione catturata: " << e.what() << "\n\n";
        }
```

---

## K7 · NAMED CONSTRUCTOR
**Il testo dice:** "can be built from the mass, or from the irreducible mass, or ..." — più modi di costruire che prendono **gli stessi tipi** (due costruttori `Nome(double)` non possono esistere).  
**Scheletro:** A — esempio d'esame: BH (06/2022), `fromMass`, `fromSpin`, ...

Sono funzioni **static** (si chiamano senza oggetto: `Nome::daB(1.5)`) che controllano, calcolano il dato mancante e restituiscono `Nome(...)`.

**NEL Nome.h** → COSTRUTTORI
```cpp
    // Named constructor: stessa firma, nomi diversi
    static Nome daA(double a);                  // da a
    static Nome daB(double b);                  // da b: a = 2 * b,  b > 0
```

**NEL Nome.h** → METODI DI SERVIZIO
```cpp
    static void controllaB(double b);           // static: serve PRIMA che l'oggetto esista
```

**NEL Nome.cc** → COSTRUTTORI
```cpp
// Da a
Nome Nome::daA(double a)                       // niente static nel .cc
{
    return Nome("da a", a);
}

// Da b:  a = 2 * b
Nome Nome::daB(double b)
{
    controllaB(b);                             // 1. controllo (prima di radici, divisioni...)
    double a = 2.0 * b;                        // 2. formula inversa: il dato che manca
    return Nome("da b", a);                    // 3. costruttore regolare
}
```

**NEL Nome.cc** → METODI DI SERVIZIO
```cpp
void Nome::controllaB(double b)                // niente static nel .cc
{
    if (b <= 0.0)
    {
        throw std::invalid_argument("b deve essere > 0");
    }
}
```

**NEL app.cpp**
```cpp
        // Rk: named constructor
        std::cout << "=== Named constructor ===\n";
        Nome n1 = Nome::daA(3.0);                 // Classe::funzione, senza oggetto
        Nome n2 = Nome::daB(1.5);
        std::cout << n1 << "\n" << n2 << "   (atteso stesso a = 3)\n\n";
```

> **Per OBBLIGARE a usarli** (il testo dice "the only way to build"): sposta la riga del costruttore regolare dal `public:` al `private:` nel `.h`. Il main allora non può più scrivere `Nome x("x", 3.0);`.

---

# ATTRIBUTI

## T1 · AGGIUNGERE UN ATTRIBUTO NUMERICO
**Il testo dice:** l'oggetto ha una grandezza in più (es. massa **e** spin).  
**Scheletro:** A — esempio: `b` in [0, 1].

**Un attributo nuovo tocca 7 posti** (spunta la lista):
☐ costruttore nel `.h` ☐ costruttore nel `.cc` (lista + controllo) ☐ getter ☐ setter ☐ attributo ☐ controllo ☐ print — e nel main ogni oggetto costruito.

**SOSTITUISCI in Nome.h** → `Nome(const std::string& label, double a);`
```cpp
    Nome(const std::string& label, double a, double b);   // b [unita'] in [0, 1]
```

**NEL Nome.h** → GETTER
```cpp
    double b() const;
```

**NEL Nome.h** → SETTER
```cpp
    void setB(double b);
```

**NEL Nome.h** → ATTRIBUTI
```cpp
    double b_;                           // [unita'], in [0, 1]
```

**NEL Nome.h** → METODI DI SERVIZIO
```cpp
    void controllaB(double b) const;     // lancia se b non e' in [0, 1]
```

**SOSTITUISCI in Nome.cc** → `Nome::Nome(const std::string& label, double a)`
```cpp
Nome::Nome(const std::string& label, double a, double b)
    : label_(label)
    , a_(a)
    , b_(b)                      // nell'ordine degli attributi
{
    controlla(a_);
    controllaB(b_);
}
```

**NEL Nome.cc** → GETTER
```cpp
double Nome::b() const
{
    return b_;
}
```

**NEL Nome.cc** → SETTER
```cpp
void Nome::setB(double b)
{
    controllaB(b);
    b_ = b;
}
```

**NEL Nome.cc** → METODI DI SERVIZIO
```cpp
// b in [0, 1]: quadre = estremi validi -> nell'if < e > stretti
void Nome::controllaB(double b) const
{
    if (b < 0.0 || b > 1.0)
    {
        throw std::invalid_argument("b deve essere in [0, 1]");
    }
}
```

**SOSTITUISCI in Nome.cc** → `void Nome::print(std::ostream& os) const`
```cpp
void Nome::print(std::ostream& os) const
{
    os << label_ << ": a = " << a_ << ", b = " << b_;
}
```

**SOSTITUISCI in app.cpp** → `Nome x("x", 3.0);`
```cpp
        Nome x("x", 3.0, 0.5);
```

**SOSTITUISCI in app.cpp** → `Nome errato("errato", -1.0);`
```cpp
            Nome errato("errato", -1.0, 0.5);
```

**NEL app.cpp**
```cpp
        // Rk: la seconda grandezza
        std::cout << "=== Attributo b ===\n";
        x.setB(0.8);
        std::cout << "b = " << x.b() << "   (atteso 0.8)\n";
        try
        {
            x.setB(1.5);
            std::cout << "ERRORE: doveva lanciare un'eccezione\n";
        }
        catch (const std::invalid_argument& e)
        {
            std::cout << "Eccezione catturata: " << e.what() << "\n\n";
        }
```

---

## T2 · VETTORE 3D (`std::array`)
**Il testo dice:** "vector", "position", "spin vector", "three components", "|v| <= 1"  
**Scheletro:** A — esempio: v con modulo al massimo 1.

`std::array<double, 3>` = esattamente 3 numeri. Si legge con `v_[0]`, `v_[1]`, `v_[2]`. È un oggetto → getter con `const&`.

**NEL Nome.h** → INCLUDE
```cpp
#include <array>       // std::array -> vettore 3D
```

**SOSTITUISCI in Nome.h** → `Nome(const std::string& label, double a);`
```cpp
    Nome(const std::string& label, double a, const std::array<double, 3>& v);   // |v| <= 1
```

**NEL Nome.h** → GETTER
```cpp
    const std::array<double, 3>& v() const;   // oggetto: const&
```

**NEL Nome.h** → SETTER
```cpp
    void setV(const std::array<double, 3>& v);
```

**NEL Nome.h** → METODI
```cpp
    // modulo = std::sqrt(v_[0]*v_[0] + v_[1]*v_[1] + v_[2]*v_[2])
    double modulo() const;
```

**NEL Nome.h** → ATTRIBUTI
```cpp
    std::array<double, 3> v_;            // [unita']
```

**NEL Nome.h** → METODI DI SERVIZIO
```cpp
    void controllaV(const std::array<double, 3>& v) const;   // lancia se |v| > 1
```

**NEL Nome.cc** → INCLUDE
```cpp
#include <cmath>       // std::sqrt
```

**SOSTITUISCI in Nome.cc** → `Nome::Nome(const std::string& label, double a)`
```cpp
Nome::Nome(const std::string& label, double a, const std::array<double, 3>& v)
    : label_(label)
    , a_(a)
    , v_(v)
{
    controlla(a_);
    controllaV(v_);
}
```

**NEL Nome.cc** → GETTER
```cpp
const std::array<double, 3>& Nome::v() const
{
    return v_;
}
```

**NEL Nome.cc** → SETTER
```cpp
void Nome::setV(const std::array<double, 3>& v)
{
    controllaV(v);
    v_ = v;
}
```

**NEL Nome.cc** → METODI
```cpp
double Nome::modulo() const
{
    return std::sqrt(v_[0] * v_[0] + v_[1] * v_[1] + v_[2] * v_[2]);
}
```

**NEL Nome.cc** → METODI DI SERVIZIO
```cpp
void Nome::controllaV(const std::array<double, 3>& v) const
{
    if (std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]) > 1.0)
    {
        throw std::invalid_argument("|v| deve essere <= 1");
    }
}
```

**SOSTITUISCI in app.cpp** → `Nome x("x", 3.0);`
```cpp
        Nome x("x", 3.0, {0.0, 0.0, 0.5});       // il vettore si scrive tra graffe
```

**SOSTITUISCI in app.cpp** → `Nome errato("errato", -1.0);`
```cpp
            Nome errato("errato", -1.0, {0.0, 0.0, 0.0});
```

**NEL app.cpp**
```cpp
        // Rk: il vettore 3D
        std::cout << "=== Vettore ===\n";
        x.setV({0.1, 0.2, 0.3});
        std::cout << "v_z = " << x.v()[2] << ", |v| = " << x.modulo() << "   (atteso 0.3 e 0.374)\n\n";
```

---

## T3 · ELENCO DI NUMERI (`std::vector`)
**Il testo dice:** "a list of values", "N measurements", "add a value"  
**Scheletro:** A

**NEL Nome.h** → INCLUDE
```cpp
#include <vector>      // std::vector -> elenco di lunghezza variabile
#include <cstddef>     // std::size_t -> numeri e indici
```

**NEL Nome.h** → GETTER
```cpp
    const std::vector<double>& x() const;    // oggetto: const&
    std::size_t size() const;                // quanti valori
```

**NEL Nome.h** → SETTER
```cpp
    void aggiungi(double valore);            // in fondo
    void rimuovi(std::size_t i);             // l'i-esimo (da 0)
```

**NEL Nome.h** → METODI
```cpp
    // media = somma / numero
    double media() const;
```

**NEL Nome.h** → ATTRIBUTI
```cpp
    std::vector<double> x_;              // [unita']
```

**SOSTITUISCI in Nome.cc** → `Nome::Nome(const std::string& label, double a)`
```cpp
Nome::Nome(const std::string& label, double a)
    : label_(label)
    , a_(a)
    , x_()                       // elenco vuoto
{
    controlla(a_);
}
```

**NEL Nome.cc** → GETTER
```cpp
const std::vector<double>& Nome::x() const
{
    return x_;
}

std::size_t Nome::size() const
{
    return x_.size();
}
```

**NEL Nome.cc** → SETTER
```cpp
void Nome::aggiungi(double valore)
{
    x_.push_back(valore);
}

void Nome::rimuovi(std::size_t i)
{
    if (i >= x_.size())                      // prima il controllo
    {
        throw std::out_of_range("indice fuori range");
    }
    x_.erase(x_.begin() + i);                // poi la cancellazione
}
```

**NEL Nome.cc** → METODI
```cpp
double Nome::media() const
{
    if (x_.empty())
    {
        throw std::runtime_error("elenco vuoto");
    }
    double somma = 0.0;                      // 1. da zero
    for (double xi : x_)                     // 2. un valore alla volta
    {
        somma += xi;                         // 3. accumulo
    }
    return somma / x_.size();                // 4. divido
}
```

**NEL app.cpp**
```cpp
        // Rk: elenco di valori
        std::cout << "=== Elenco ===\n";
        x.aggiungi(1.0);
        x.aggiungi(2.0);
        x.aggiungi(6.0);
        std::cout << "N = " << x.size() << ", media = " << x.media() << "   (atteso 3 e 3)\n";
        x.rimuovi(2);
        std::cout << "dopo rimuovi(2): media = " << x.media() << "   (atteso 1.5)\n\n";
```

---

## T4 · GRANDEZZA MEMORIZZATA + `update()`
**Il testo dice:** "store ... as an attribute", "as a data member", "keep ... up to date"  
**Scheletro:** A — esempio: `risultato = a * a`.

Se il testo **non** dice "store/attribute", la grandezza calcolata NON si memorizza: basta un metodo `const` che la calcola al volo (come `calcolo()`).

**Regola:** `update()` si chiama alla **fine** di ogni costruttore e di ogni setter che cambia un dato da cui dipende.

**NEL Nome.h** → GETTER
```cpp
    double risultato() const;            // il valore memorizzato
```

**NEL Nome.h** → ATTRIBUTI
```cpp
    double risultato_;                   // = a_ * a_, calcolato da update(): DOPO i dati
```

**NEL Nome.h** → METODI DI SERVIZIO
```cpp
    void update();                       // ricalcola risultato_ (non const: lo modifica)
```

**SOSTITUISCI in Nome.cc** → `Nome::Nome(const std::string& label, double a)`
```cpp
Nome::Nome(const std::string& label, double a)
    : label_(label)
    , a_(a)
    , risultato_(0.0)            // provvisorio...
{
    controlla(a_);
    update();                    // ...il valore vero, per ULTIMO
}
```

**SOSTITUISCI in Nome.cc** → `void Nome::setA(double a)`
```cpp
void Nome::setA(double a)
{
    controlla(a);
    a_ = a;
    update();                    // a e' cambiato: ricalcolo
}
```

**NEL Nome.cc** → GETTER
```cpp
double Nome::risultato() const
{
    return risultato_;
}
```

**NEL Nome.cc** → METODI DI SERVIZIO
```cpp
// risultato = a * a
void Nome::update()
{
    risultato_ = a_ * a_;
}
```

**NEL app.cpp**
```cpp
        // Rk: grandezza memorizzata
        std::cout << "=== Memorizzata ===\n";
        std::cout << "risultato = " << x.risultato() << "   (atteso 4 * 4 = 16)\n";
        x.setA(3.0);
        std::cout << "dopo setA(3): " << x.risultato() << "   (atteso 9: aggiornato)\n\n";
        x.setA(4.0);                              // rimetto a = 4 per i blocchi successivi
```

---

## T5 · CONTATORE DI OGGETTI
**Il testo dice:** "number of instances", "how many objects exist at any time"  
**Scheletro:** A  
**Richiede:** K3

`static` = un solo valore condiviso da **tutti** gli oggetti. Si conta `+1` in **ogni** costruttore (anche la copia) e `-1` nel distruttore.

**NEL Nome.h** → COSTRUTTORI
```cpp
    // Distruttore: l'oggetto smette di esistere
    ~Nome();
```

**NEL Nome.h** → METODI
```cpp
    static int count();                  // oggetti esistenti adesso
```

**NEL Nome.h** → ATTRIBUTI
```cpp
    static int count_;                   // condiviso da tutti gli oggetti
```

**NEL Nome.cc** → COSTANTI
```cpp
int Nome::count_ = 0;                    // definizione: UNA volta, qui, senza "static"
```

**SOSTITUISCI in Nome.cc** → `Nome::Nome(const std::string& label, double a)`
```cpp
Nome::Nome(const std::string& label, double a)
    : label_(label)
    , a_(a)
{
    controlla(a_);
    ++count_;                    // DOPO i controlli: si contano solo oggetti validi
}
```

**SOSTITUISCI in Nome.cc** → `Nome::Nome(const Nome& other)`
```cpp
Nome::Nome(const Nome& other)
    : label_(other.label_)
    , a_(other.a_)
{
    ++count_;                    // anche la copia e' un oggetto nuovo
}
```

**NEL Nome.cc** → COSTRUTTORI
```cpp
// Distruttore
Nome::~Nome()
{
    --count_;
}
```

**NEL Nome.cc** → METODI
```cpp
int Nome::count()                        // niente static nel .cc
{
    return count_;
}
```

**NEL app.cpp**
```cpp
        // Rk: contatore
        std::cout << "=== Contatore ===\n";
        std::cout << "oggetti: " << Nome::count() << "\n";
        {
            Nome tmp("tmp", 1.0);
            Nome copia(tmp);
            std::cout << "dentro il blocco: " << Nome::count() << "   (2 in piu')\n";
        }                                         // qui tmp e copia vengono distrutti
        std::cout << "dopo il blocco: " << Nome::count() << "   (come all'inizio)\n\n";
```

---

## T6 · COSTANTI: π, ε₀, G, ...
**Il testo dice:** una costante fisica nella formula, "set M_sun = 1"  
**Scheletro:** A

Nel **.cc**, prima dei costruttori, con nome MAIUSCOLO e unità. Se il testo dice "set M_sun = 1": scrivilo nel commento della classe (`// UNITA': masse in masse solari`).

**NEL Nome.cc** → INCLUDE
```cpp
#include <cmath>       // std::acos
```

**NEL Nome.cc** → COSTANTI
```cpp
const double PI = std::acos(-1.0);       // pi greco (acos(-1) = pi)
const double EPS0 = 8.854e-12;           // permittivita' del vuoto [F/m]
const double G = 6.674e-11;              // costante di gravitazione [m^3 kg^-1 s^-2]
const double C_LUCE = 2.998e8;           // velocita' della luce [m/s]
```

**NEL Nome.h** → METODI
```cpp
    // area = PI * a_ * a_
    double area() const;
```

**NEL Nome.cc** → METODI
```cpp
double Nome::area() const
{
    return PI * a_ * a_;
}
```

**NEL app.cpp**
```cpp
        // Rk: formula con una costante
        std::cout << "area = " << x.area() << "   (atteso pi * 16 = 50.27)\n\n";
```

---

# METODI

## M1 · METODO CHE RESTITUISCE UN OGGETTO NUOVO
**Il testo dice:** "returns the inverse", "returns a new ...", "without modifying"  
**Scheletro:** A

Tipo di ritorno = la classe; `const` perché **questo** oggetto non cambia.

**NEL Nome.h** → METODI
```cpp
    // Un Nome nuovo con a inverso; questo resta invariato
    Nome inverso() const;
```

**NEL Nome.cc** → METODI
```cpp
Nome Nome::inverso() const
{
    return Nome(label_, 1.0 / a_);       // costruisco e restituisco un oggetto nuovo
}
```

**NEL app.cpp**
```cpp
        // Rk: oggetto nuovo
        std::cout << "=== Inverso ===\n";
        Nome inv = x.inverso();
        std::cout << inv << "   (atteso a = 0.25), x invariato: " << x << "\n\n";
```

---

## M2 · SCRIVERE SU FILE
**Il testo dice:** "write ... to a text file", "save", "store the values in ..."  
**Scheletro:** A

**NEL Nome.h** → METODI
```cpp
    // Scrive label e a su file, una riga
    void salva(const std::string& nomeFile) const;
```

**NEL Nome.cc** → INCLUDE
```cpp
#include <fstream>     // std::ofstream
```

**NEL Nome.cc** → METODI
```cpp
void Nome::salva(const std::string& nomeFile) const
{
    std::ofstream out(nomeFile);             // 1. apro in scrittura (cancella il contenuto vecchio)
    if (!out)                                // 2. controllo
    {
        throw std::runtime_error("impossibile scrivere " + nomeFile);
    }
    out << label_ << " " << a_ << "\n";      // 3. scrivo come con std::cout, colonne separate da spazi
}
```

**NEL app.cpp**
```cpp
        // Rk: file
        x.salva("nome.txt");
        std::cout << "scritto nome.txt (aprilo per controllare)\n\n";
```

> Il file viene creato nella cartella da cui lanci `./app`. Va messo nello zip.

---

## M3 · CONTROLLI CON INTERVALLI
**Il testo dice:** "must be in [1, 200]", "(0, 0.25]", "positive", "between ... and ..."  
**Scheletro:** A

Nell'`if` va il **contrario** del vincolo: quando il valore è **sbagliato**.

| Intervallo valido | Nell'`if` (sbagliato) |
|---|---|
| a > 0 | `a <= 0.0` |
| a ≥ 0 | `a < 0.0` |
| [x, y] | `a < x \|\| a > y` |
| (x, y] | `a <= x \|\| a > y` |
| [x, y) | `a < x \|\| a >= y` |
| (x, y) | `a <= x \|\| a >= y` |

**Quadra [ = estremo valido** → `<`, `>` stretti. **Tonda ( = estremo escluso** → `<=`, `>=`.

**SOSTITUISCI in Nome.cc** → `void Nome::controlla(double a) const`
```cpp
// a in (0, 10]: tonda a sinistra (0 escluso), quadra a destra (10 valido)
void Nome::controlla(double a) const
{
    if (a <= 0.0 || a > 10.0)
    {
        throw std::invalid_argument("a deve essere in (0, 10]");
    }
}
```

**NEL app.cpp**
```cpp
        // Rk: estremi dell'intervallo
        x.setA(10.0);
        std::cout << "a = 10 accettato   (estremo valido)\n";
        try
        {
            x.setA(10.5);
            std::cout << "ERRORE: doveva lanciare un'eccezione\n";
        }
        catch (const std::invalid_argument& e)
        {
            std::cout << "Eccezione catturata: " << e.what() << "\n\n";
        }
        x.setA(4.0);
```

---

## M4 · FORMULE: POTENZE, RADICI, ESPONENZIALI
**Il testo dice:** una formula con potenze, radici, esponenziali  
**Scheletro:** A

| Matematica | C++ | Attenzione |
|---|---|---|
| a³ | `std::pow(a_, 3.0)` oppure `a_ * a_ * a_` | **MAI** `a_^3` |
| a^(3/5) | `std::pow(a_, 3.0 / 5.0)` | **MAI** `3/5` (= 0, divisione intera) |
| √a | `std::sqrt(a_)` | controlla prima che a ≥ 0 |
| eˣ, ln x | `std::exp(x)`, `std::log(x)` | |
| \|x\| | `std::fabs(x)` | |
| sin, cos, arccos | `std::sin(x)`, `std::cos(x)`, `std::acos(x)` | angoli in **radianti** |

**NEL Nome.cc** → INCLUDE
```cpp
#include <cmath>       // std::pow, std::sqrt
```

**NEL Nome.h** → METODI
```cpp
    // formula = std::pow(a_, 3.0 / 5.0) * std::sqrt(a_)
    double formula() const;
```

**NEL Nome.cc** → METODI
```cpp
double Nome::formula() const
{
    return std::pow(a_, 3.0 / 5.0) * std::sqrt(a_);
}
```

**NEL app.cpp**
```cpp
        // Rk: formula
        std::cout << "formula = " << x.formula() << "   (atteso 4^0.6 * 2 = 4.595)\n\n";
```

> **Trucco anti-errore:** scrivi la formula **nel commento del .h** in sintassi C++, poi copiala nel `.cc`.

---

# COMPOSIZIONE (scheletro B)

Nello scheletro B: `Elemento` = il pezzo, `Contenitore` = tanti pezzi in un `std::vector`, `Coppia` = esattamente due pezzi. Nel main ci sono già `e` (un Elemento), `c` (Contenitore con a = 1, 2, 3) e `coppia`.

**Lo schema di quasi tutti i metodi del Contenitore** (il "ciclo sugli elementi"):
```
1. parto da zero                     double somma = 0.0;
2. prendo gli elementi uno alla volta for (const Elemento& e : elementi_)
3. chiedo all'elemento il SUO valore     e.a()   e.contributo(r)   ...
4. lo accumulo                           somma += ...;
5. restituisco                       return somma;
```

## B0 · SECONDA GRANDEZZA DELL'ELEMENTO
**Il testo dice:** "mass and spin", "charge and mass" — ogni pezzo ha due grandezze.  
**Scheletro:** B

Si **aggiunge** un costruttore con due argomenti; quello con uno resta (b = 0).

**NEL Elemento.h** → COSTRUTTORI
```cpp
    // Con anche b [unita'] in [0, 1]
    Elemento(double a, double b);
```

**NEL Elemento.h** → GETTER
```cpp
    double b() const;
```

**NEL Elemento.h** → ATTRIBUTI
```cpp
    double b_;                           // [unita'], in [0, 1]
```

**NEL Elemento.h** → METODI DI SERVIZIO
```cpp
    void controllaB(double b) const;
```

**SOSTITUISCI in Elemento.cc** → `Elemento::Elemento(double a)`
```cpp
Elemento::Elemento(double a)
    : a_(a)
    , b_(0.0)                    // anche qui: OGNI attributo in OGNI costruttore
{
    controlla(a_);
}
```

**NEL Elemento.cc** → COSTRUTTORI
```cpp
// Con anche b
Elemento::Elemento(double a, double b)
    : a_(a)
    , b_(b)
{
    controlla(a_);
    controllaB(b_);
}
```

**NEL Elemento.cc** → GETTER
```cpp
double Elemento::b() const
{
    return b_;
}
```

**NEL Elemento.cc** → METODI DI SERVIZIO
```cpp
void Elemento::controllaB(double b) const
{
    if (b < 0.0 || b > 1.0)
    {
        throw std::invalid_argument("b deve essere in [0, 1]");
    }
}
```

**SOSTITUISCI in Elemento.cc** → `void Elemento::print(std::ostream& os) const`
```cpp
void Elemento::print(std::ostream& os) const
{
    os << "a = " << a_ << ", b = " << b_;
}
```

**NEL app.cpp**
```cpp
        // Rk: elemento con due grandezze
        Elemento eb(2.0, 0.5);
        std::cout << eb << "   b = " << eb.b() << "   (atteso 0.5)\n\n";
```

---

## B1 · POSIZIONE DELL'ELEMENTO
**Il testo dice:** "located at (x, y, z)", "position", "point charge at r"  
**Scheletro:** B

**NEL Elemento.h** → INCLUDE
```cpp
#include <array>       // std::array -> posizione 3D
```

**NEL Elemento.h** → COSTRUTTORI
```cpp
    // Con la posizione pos [m]
    Elemento(double a, const std::array<double, 3>& pos);
```

**NEL Elemento.h** → GETTER
```cpp
    const std::array<double, 3>& pos() const;   // [m], oggetto: const&
```

**NEL Elemento.h** → ATTRIBUTI
```cpp
    std::array<double, 3> pos_;          // posizione [m]
```

**SOSTITUISCI in Elemento.cc** → `Elemento::Elemento(double a)`
```cpp
Elemento::Elemento(double a)
    : a_(a)
    , pos_({0.0, 0.0, 0.0})      // senza posizione: nell'origine
{
    controlla(a_);
}
```

**NEL Elemento.cc** → COSTRUTTORI
```cpp
// Con la posizione
Elemento::Elemento(double a, const std::array<double, 3>& pos)
    : a_(a)
    , pos_(pos)
{
    controlla(a_);
}
```

**NEL Elemento.cc** → GETTER
```cpp
const std::array<double, 3>& Elemento::pos() const
{
    return pos_;
}
```

**SOSTITUISCI in Elemento.cc** → `void Elemento::print(std::ostream& os) const`
```cpp
void Elemento::print(std::ostream& os) const
{
    os << "a = " << a_ << " in (" << pos_[0] << ", " << pos_[1] << ", " << pos_[2] << ") m";
}
```

**NEL app.cpp**
```cpp
        // Rk: elemento con posizione
        Elemento ep(1.0, {1.0, 2.0, 3.0});
        std::cout << ep << "   z = " << ep.pos()[2] << "   (atteso 3)\n\n";
```

> Se hai già fatto B0, nel costruttore con un argomento metti **entrambe** le righe `, b_(0.0)` e `, pos_({0.0, 0.0, 0.0})`, nell'ordine degli attributi.

---

## B2 · CONTRIBUTO VETTORIALE IN UN PUNTO + SOMMA (sovrapposizione)
**Il testo dice:** "electric field at r", "gravitational force", "total field", "superposition"  
**Scheletro:** B  
**Richiede:** B1

L'**Elemento** calcola il SUO contributo; il **Contenitore** parte da zero e li somma. Esempio: campo di Coulomb, `E = K a d / |d|^3` con `d = r - pos`.

**NEL Elemento.cc** → INCLUDE
```cpp
#include <cmath>       // std::sqrt, std::pow
```

**NEL Elemento.cc** → COSTANTI
```cpp
const double K = 8.9875517923e9;     // 1 / (4 pi eps0) [N m^2 / C^2]
```

**NEL Elemento.h** → METODI
```cpp
    // Contributo di QUESTO elemento nel punto r:
    //   d = r - pos_,   contributo = K * a_ * d / |d|^3
    std::array<double, 3> contributo(const std::array<double, 3>& r) const;
```

**NEL Elemento.cc** → METODI
```cpp
std::array<double, 3> Elemento::contributo(const std::array<double, 3>& r) const
{
    std::array<double, 3> d = {r[0] - pos_[0], r[1] - pos_[1], r[2] - pos_[2]};   // 1. vettore distanza
    double dist = std::sqrt(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]);           // 2. il suo modulo
    if (dist == 0.0)                                                             // 3. non definito sull'elemento
    {
        throw std::domain_error("non definito nella posizione dell'elemento");
    }
    double fattore = K * a_ / std::pow(dist, 3.0);                               // 4. la formula
    return {fattore * d[0], fattore * d[1], fattore * d[2]};                     // 5. componente per componente
}
```

**NEL Contenitore.h** → INCLUDE
```cpp
#include <array>       // std::array -> vettori 3D
```

**NEL Contenitore.h** → METODI
```cpp
    // Somma dei contributi di tutti gli elementi nel punto r
    std::array<double, 3> contributo(const std::array<double, 3>& r) const;
```

**NEL Contenitore.cc** → METODI
```cpp
std::array<double, 3> Contenitore::contributo(const std::array<double, 3>& r) const
{
    std::array<double, 3> V = {0.0, 0.0, 0.0};      // 1. parto da zero
    for (const Elemento& e : elementi_)              // 2. un elemento alla volta
    {
        std::array<double, 3> Vi = e.contributo(r);  // 3. il contributo di QUESTO elemento
        for (int k = 0; k < 3; ++k)                  // 4. sommato componente per componente
        {
            V[k] += Vi[k];
        }
    }
    return V;                                        // 5. il totale
}
```

**NEL app.cpp**
```cpp
        // Rk: sovrapposizione
        std::cout << "=== Sovrapposizione ===\n";
        Contenitore simm(std::vector<Elemento>{Elemento(1.0, {1.0, 0.0, 0.0}), Elemento(1.0, {-1.0, 0.0, 0.0})});
        std::array<double, 3> V0 = simm.contributo({0.0, 0.0, 0.0});
        std::array<double, 3> V1 = simm.contributo({0.0, 0.0, 1.0});
        std::cout << "in (0,0,0): (" << V0[0] << ", " << V0[1] << ", " << V0[2] << ")   (atteso 0: simmetria)\n";
        std::cout << "in (0,0,1): (" << V1[0] << ", " << V1[1] << ", " << V1[2] << ")   (atteso (0, 0, 2K/2^1.5 = 6.36e9))\n";
        try
        {
            simm.contributo({1.0, 0.0, 0.0});         // proprio sopra un elemento
            std::cout << "ERRORE: doveva lanciare un'eccezione\n";
        }
        catch (const std::domain_error& ex)
        {
            std::cout << "Eccezione catturata: " << ex.what() << "\n\n";
        }
```

**NEL app.cpp** → INCLUDE
```cpp
#include <array>
```

---

## B3 · CONTRIBUTO SCALARE IN UN PUNTO + SOMMA
**Il testo dice:** "potential at r", "potential energy"  
**Scheletro:** B  
**Richiede:** B1

**NEL Elemento.cc** → INCLUDE
```cpp
#include <cmath>       // std::sqrt
```

**NEL Elemento.cc** → COSTANTI
```cpp
const double K = 8.9875517923e9;     // [N m^2 / C^2]
```

**NEL Elemento.h** → METODI
```cpp
    // Contributo scalare nel punto r:  K * a_ / |r - pos_|
    double contributoScalare(const std::array<double, 3>& r) const;
```

**NEL Elemento.cc** → METODI
```cpp
double Elemento::contributoScalare(const std::array<double, 3>& r) const
{
    std::array<double, 3> d = {r[0] - pos_[0], r[1] - pos_[1], r[2] - pos_[2]};
    double dist = std::sqrt(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]);
    if (dist == 0.0)
    {
        throw std::domain_error("non definito nella posizione dell'elemento");
    }
    return K * a_ / dist;
}
```

**NEL Contenitore.h** → INCLUDE
```cpp
#include <array>
```

**NEL Contenitore.h** → METODI
```cpp
    // Somma dei contributi scalari nel punto r
    double contributoScalare(const std::array<double, 3>& r) const;
```

**NEL Contenitore.cc** → METODI
```cpp
double Contenitore::contributoScalare(const std::array<double, 3>& r) const
{
    double S = 0.0;
    for (const Elemento& e : elementi_)
    {
        S += e.contributoScalare(r);
    }
    return S;
}
```

**NEL app.cpp**
```cpp
        // Rk: contributo scalare
        Contenitore due(std::vector<Elemento>{Elemento(1.0, {1.0, 0.0, 0.0}), Elemento(1.0, {-1.0, 0.0, 0.0})});
        std::cout << "scalare in (0,0,0): " << due.contributoScalare({0.0, 0.0, 0.0}) << "   (atteso 2K = 1.8e10)\n\n";
```

---

## B4 · AGGIUNGERE E TOGLIERE ELEMENTI
**Il testo dice:** "add an element", "remove the i-th element"  
**Scheletro:** B

**NEL Contenitore.h** → SETTER
```cpp
    void aggiungi(const Elemento& e);        // in fondo
    void rimuovi(std::size_t i);             // l'i-esimo (da 0)
```

**NEL Contenitore.cc** → SETTER
```cpp
void Contenitore::aggiungi(const Elemento& e)
{
    elementi_.push_back(e);
}

void Contenitore::rimuovi(std::size_t i)
{
    if (i >= elementi_.size())               // prima il controllo
    {
        throw std::out_of_range("indice fuori range");
    }
    elementi_.erase(elementi_.begin() + i);  // poi la cancellazione
}
```

**NEL app.cpp**
```cpp
        // Rk: aggiungi e rimuovi
        std::cout << "=== Aggiungi e rimuovi ===\n";
        c.aggiungi(Elemento(10.0));
        std::cout << "dopo aggiungi(10): totale = " << c.totale() << "   (atteso 16)\n";
        c.rimuovi(0);                                // dopo, gli indici scalano di uno
        std::cout << "dopo rimuovi(0):   totale = " << c.totale() << "   (atteso 15)\n\n";
```

---

## B5 · COSTRUTTORE CHE LEGGE DA FILE
**Il testo dice:** "a constructor that reads the elements from a text file"  
**Scheletro:** B

Il file ha **una riga per elemento**. Il ciclo `while (file >> a)` legge finché ci sono numeri.
- Riga `a` → `while (file >> a)` e `Elemento(a)`
- Riga `a x y z` (con B1) → `while (file >> a >> x >> y >> z)` e `Elemento(a, {x, y, z})`
- Riga `a b` (con B0) → `while (file >> a >> b)` e `Elemento(a, b)`

**NUOVO FILE elementi.txt**
```text
1.0
2.0
```

**NEL Contenitore.h** → INCLUDE
```cpp
#include <string>      // std::string -> nome del file
```

**NEL Contenitore.h** → COSTRUTTORI
```cpp
    // Da file: una riga per elemento, formato "a"
    explicit Contenitore(const std::string& nomeFile);
```

**NEL Contenitore.cc** → INCLUDE
```cpp
#include <fstream>     // std::ifstream
```

**NEL Contenitore.cc** → COSTRUTTORI
```cpp
// Da file
Contenitore::Contenitore(const std::string& nomeFile)
    : elementi_()                                    // si parte senza elementi
{
    std::ifstream file(nomeFile);                    // 1. apro il file
    if (!file)                                       // 2. controllo che si sia aperto
    {
        throw std::runtime_error("impossibile aprire " + nomeFile);
    }
    double a;
    while (file >> a)                                // 3. finche' riesco a leggere una riga
    {
        elementi_.push_back(Elemento(a));            // 4. creo l'elemento e lo aggiungo
    }
}
```

**NEL app.cpp**
```cpp
        // Rk: costruttore da file (elementi.txt nella cartella da cui lanci ./app)
        std::cout << "=== Da file ===\n";
        Contenitore daFile("elementi.txt");
        std::cout << daFile << "totale = " << daFile.totale() << "   (atteso 3)\n";
        try
        {
            Contenitore nessuno("non_esiste.txt");
            std::cout << "ERRORE: doveva lanciare un'eccezione\n";
        }
        catch (const std::runtime_error& ex)
        {
            std::cout << "Eccezione catturata: " << ex.what() << "\n\n";
        }
```

> Il file di dati va **nello zip**. Se il programma dice "impossibile aprire": controlla con `ls` di essere nella cartella giusta.

---

## B6 · COSTRUTTORE CHE CHIEDE N ELEMENTI
**Il testo dice:** "asks the user for N elements", "from the keyboard"  
**Scheletro:** B

**NEL Contenitore.h** → COSTRUTTORI
```cpp
    // Chiede all'utente N elementi da tastiera
    explicit Contenitore(int N);
```

**NEL Contenitore.cc** → INCLUDE
```cpp
#include <iostream>    // std::cout, std::cin
```

**NEL Contenitore.cc** → COSTRUTTORI
```cpp
// Da tastiera: N elementi
Contenitore::Contenitore(int N)
    : elementi_()
{
    for (int i = 0; i < N; ++i)                      // per ogni elemento
    {
        double a;
        std::cout << "Elemento " << i << ", a = ";
        std::cin >> a;
        if (!std::cin)                               // non era un numero
        {
            throw std::runtime_error("input non valido");
        }
        elementi_.push_back(Elemento(a));
    }
}
```

**NEL app.cpp**
```cpp
        // Rk: da tastiera
        std::cout << "=== Da tastiera ===\n";
        Contenitore daTastiera(2);
        std::cout << "\n" << daTastiera << "\n";
```

---

## B7 · MEDIA, MASSIMO, MINIMO
**Il testo dice:** "mean", "average", "the largest", "the smallest", "which element has the maximum"  
**Scheletro:** B

**NEL Contenitore.h** → METODI
```cpp
    double media() const;                    // totale / numero
    std::size_t indiceMassimo() const;       // posizione dell'elemento con a piu' grande
    std::size_t indiceMinimo() const;        // posizione dell'elemento con a piu' piccola
```

**NEL Contenitore.cc** → METODI
```cpp
double Contenitore::media() const
{
    if (elementi_.empty())                   // dividere per zero non si puo'
    {
        throw std::runtime_error("nessun elemento");
    }
    return totale() / elementi_.size();      // riuso totale()
}

std::size_t Contenitore::indiceMassimo() const
{
    if (elementi_.empty())
    {
        throw std::runtime_error("nessun elemento");
    }
    std::size_t iMax = 0;                                  // 1. suppongo che il massimo sia il primo
    for (std::size_t i = 1; i < elementi_.size(); ++i)     // 2. guardo tutti gli altri
    {
        if (elementi_[i].a() > elementi_[iMax].a())        // 3. se ne trovo uno piu' grande...
        {
            iMax = i;                                      //    ...diventa lui il massimo
        }
    }
    return iMax;
}

std::size_t Contenitore::indiceMinimo() const
{
    if (elementi_.empty())
    {
        throw std::runtime_error("nessun elemento");
    }
    std::size_t iMin = 0;
    for (std::size_t i = 1; i < elementi_.size(); ++i)
    {
        if (elementi_[i].a() < elementi_[iMin].a())        // come sopra, con <
        {
            iMin = i;
        }
    }
    return iMin;
}
```

**NEL app.cpp**
```cpp
        // Rk: statistiche
        std::cout << "media = " << c.media() << "   (atteso 2)\n";
        std::cout << "massimo in [" << c.indiceMassimo() << "]   (atteso 2), minimo in [" << c.indiceMinimo() << "]   (atteso 0)\n\n";
```

---

## B8 · CONTARE CON UNA CONDIZIONE
**Il testo dice:** "how many elements have ... above ...", "count the ..."  
**Scheletro:** B

**NEL Contenitore.h** → METODI
```cpp
    // Quanti elementi hanno a > soglia
    std::size_t contaSopra(double soglia) const;
```

**NEL Contenitore.cc** → METODI
```cpp
std::size_t Contenitore::contaSopra(double soglia) const
{
    std::size_t conta = 0;
    for (const Elemento& e : elementi_)
    {
        if (e.a() > soglia)                  // la condizione del testo
        {
            ++conta;
        }
    }
    return conta;
}
```

**NEL app.cpp**
```cpp
        // Rk: conteggio
        std::cout << "quanti con a > 1.5: " << c.contaSopra(1.5) << "   (atteso 2)\n\n";
```

---

## B9 · ORDINARE
**Il testo dice:** "sort the elements", "in increasing order"  
**Scheletro:** B

`std::sort` vuole una **regola di confronto**: una piccola funzione senza nome (si chiama *lambda*) che riceve due elementi e risponde "il primo va PRIMA del secondo?".

**NEL Contenitore.h** → SETTER
```cpp
    void ordina();                           // dal piu' piccolo al piu' grande (per a)
```

**NEL Contenitore.cc** → INCLUDE
```cpp
#include <algorithm>   // std::sort
```

**NEL Contenitore.cc** → SETTER
```cpp
void Contenitore::ordina()
{
    std::sort(elementi_.begin(), elementi_.end(),
              [](const Elemento& x, const Elemento& y) { return x.a() < y.a(); });   // > per l'ordine inverso
}
```

**NEL app.cpp**
```cpp
        // Rk: ordinamento
        Contenitore dis(std::vector<Elemento>{Elemento(3.0), Elemento(1.0), Elemento(2.0)});
        dis.ordina();
        std::cout << dis << "   (atteso 1, 2, 3)\n\n";
```

---

## B10 · CENTRO PESATO (di massa, di carica)
**Il testo dice:** "center of mass", "center of charge", "weighted average position"  
**Scheletro:** B  
**Richiede:** B1

`R = somma(a_i * pos_i) / somma(a_i)`

**NEL Contenitore.h** → INCLUDE
```cpp
#include <array>
```

**NEL Contenitore.h** → METODI
```cpp
    // R = somma(a_i * pos_i) / somma(a_i)
    std::array<double, 3> centro() const;
```

**NEL Contenitore.cc** → METODI
```cpp
std::array<double, 3> Contenitore::centro() const
{
    std::array<double, 3> R = {0.0, 0.0, 0.0};   // 1. da zero
    double somma = 0.0;
    for (const Elemento& e : elementi_)          // 2. uno alla volta
    {
        for (int k = 0; k < 3; ++k)              // 3. componente per componente
        {
            R[k] += e.a() * e.pos()[k];
        }
        somma += e.a();
    }
    if (somma == 0.0)
    {
        throw std::runtime_error("somma dei pesi nulla");
    }
    for (int k = 0; k < 3; ++k)                  // 4. divido per la somma dei pesi
    {
        R[k] /= somma;
    }
    return R;
}
```

**NEL app.cpp**
```cpp
        // Rk: centro pesato
        Contenitore cm(std::vector<Elemento>{Elemento(1.0, {0.0, 0.0, 0.0}), Elemento(3.0, {4.0, 0.0, 0.0})});
        std::cout << "centro x = " << cm.centro()[0] << "   (atteso (1*0 + 3*4)/4 = 3)\n\n";
```

---

## B11 · TOTALE MEMORIZZATO NEL CONTENITORE
**Il testo dice:** "store the total ... as a data member"  
**Scheletro:** B

Come T4: attributo + `update()` alla fine di **ogni** costruttore e di **ogni** metodo che cambia gli elementi (anche `aggiungi`, `rimuovi`, se li hai).

**NEL Contenitore.h** → ATTRIBUTI
```cpp
    double totale_;                          // memorizzato: DOPO elementi_
```

**NEL Contenitore.h** → METODI DI SERVIZIO
```cpp
    void update();                           // ricalcola totale_
```

**SOSTITUISCI in Contenitore.cc** → `Contenitore::Contenitore()`
```cpp
Contenitore::Contenitore()
    : elementi_()
    , totale_(0.0)
{
    update();
}
```

**SOSTITUISCI in Contenitore.cc** → `Contenitore::Contenitore(const std::vector<Elemento>& elementi)`
```cpp
Contenitore::Contenitore(const std::vector<Elemento>& elementi)
    : elementi_(elementi)
    , totale_(0.0)
{
    update();
}
```

**SOSTITUISCI in Contenitore.cc** → `double Contenitore::totale() const`
```cpp
double Contenitore::totale() const
{
    return totale_;                          // gia' calcolato da update()
}
```

**NEL Contenitore.cc** → METODI DI SERVIZIO
```cpp
void Contenitore::update()
{
    totale_ = 0.0;
    for (const Elemento& e : elementi_)
    {
        totale_ += e.a();
    }
}
```

**NEL app.cpp**
```cpp
        // Rk: totale memorizzato
        std::cout << "totale memorizzato = " << c.totale() << "   (atteso 6)\n\n";
```

---

## B12 · SALVARE GLI ELEMENTI SU FILE
**Il testo dice:** "write the elements / results to a text file"  
**Scheletro:** B

**NEL Contenitore.h** → INCLUDE
```cpp
#include <string>
```

**NEL Contenitore.h** → METODI
```cpp
    // Una riga per elemento (stesso formato del file letto)
    void salva(const std::string& nomeFile) const;
```

**NEL Contenitore.cc** → INCLUDE
```cpp
#include <fstream>     // std::ofstream
```

**NEL Contenitore.cc** → METODI
```cpp
void Contenitore::salva(const std::string& nomeFile) const
{
    std::ofstream out(nomeFile);             // 1. apro in scrittura
    if (!out)                                // 2. controllo
    {
        throw std::runtime_error("impossibile scrivere " + nomeFile);
    }
    for (const Elemento& e : elementi_)      // 3. una riga per elemento
    {
        out << e.a() << "\n";                // con B1: << " " << e.pos()[0] << ...
    }
}
```

**NEL app.cpp**
```cpp
        // Rk: salvataggio
        c.salva("risultati.txt");
        std::cout << "scritto risultati.txt (aprilo per controllare)\n\n";
```

---

## B13 · GRANDEZZA DELLA COPPIA MEMORIZZATA (es. chi_eff)
**Il testo dice:** "store the effective spin chi_eff", "mass-weighted ...", BBH (06/2022)  
**Scheletro:** B  
**Richiede:** B0

Media pesata: `(a1 b1 + a2 b2) / (a1 + a2)` (per il BBH: `chi_eff = (m1 chi1 + m2 chi2)/(m1 + m2)`, con gli angoli se il testo li dà). Memorizzata → `update()` nel costruttore e nei setter.

**NEL Coppia.h** → GETTER
```cpp
    double mediaPesata() const;              // il valore memorizzato
```

**NEL Coppia.h** → ATTRIBUTI
```cpp
    double mediaPesata_;                     // (a1 b1 + a2 b2)/(a1 + a2): DOPO i due elementi
```

**NEL Coppia.h** → METODI DI SERVIZIO
```cpp
    void update();                           // ricalcola mediaPesata_
```

**SOSTITUISCI in Coppia.cc** → `Coppia::Coppia(const Elemento& primo, const Elemento& secondo)`
```cpp
Coppia::Coppia(const Elemento& primo, const Elemento& secondo)
    : primo_(primo)
    , secondo_(secondo)
    , mediaPesata_(0.0)
{
    update();
}
```

**SOSTITUISCI in Coppia.cc** → `void Coppia::setPrimo(const Elemento& e)`
```cpp
void Coppia::setPrimo(const Elemento& e)
{
    primo_ = e;
    update();                                // un dato e' cambiato: ricalcolo
}
```

**SOSTITUISCI in Coppia.cc** → `void Coppia::setSecondo(const Elemento& e)`
```cpp
void Coppia::setSecondo(const Elemento& e)
{
    secondo_ = e;
    update();
}
```

**NEL Coppia.cc** → GETTER
```cpp
double Coppia::mediaPesata() const
{
    return mediaPesata_;
}
```

**NEL Coppia.cc** → METODI DI SERVIZIO
```cpp
// (a1 b1 + a2 b2) / (a1 + a2)
void Coppia::update()
{
    double a1 = primo_.a();                  // nomi corti = formula leggibile
    double a2 = secondo_.a();
    mediaPesata_ = (a1 * primo_.b() + a2 * secondo_.b()) / (a1 + a2);
}
```

**NEL app.cpp**
```cpp
        // Rk: grandezza della coppia memorizzata
        Coppia cp(Elemento(1.0, 0.2), Elemento(3.0, 0.6));
        std::cout << "media pesata = " << cp.mediaPesata() << "   (atteso (0.2 + 1.8)/4 = 0.5)\n";
        cp.setPrimo(Elemento(1.0, 0.6));
        std::cout << "dopo setPrimo: " << cp.mediaPesata() << "   (atteso (0.6 + 1.8)/4 = 0.6)\n\n";
```

> Un dato in più della **coppia** (es. un angolo in [0, π]): segui la scheda T1 sulla classe `Coppia`.

---

# OPERATORI (scheletro C)

Nel main dello scheletro C ci sono già `a` (a = 2), `b` (a = 3) e `s`.

**La regola:** operatore **dentro** la classe → un argomento in meno (`rhs`, quello a destra). Dentro `operator+` non si usa `+`: si scrive **la formula** e si restituisce `Nome(formula)`. Nel main: `a + b`, come in matematica.

## O1 · `-` `*` `/` TRA DUE OGGETTI
**Il testo dice:** "overload the -, * and / operators"  
**Scheletro:** C

**NEL Nome.h** → OPERATORI
```cpp
    Nome operator-(const Nome& rhs) const;   // a - b
    Nome operator*(const Nome& rhs) const;   // a * b
    Nome operator/(const Nome& rhs) const;   // a / b (lancia se b = 0)
```

**NEL Nome.cc** → OPERATORI
```cpp
Nome Nome::operator-(const Nome& rhs) const
{
    return Nome(a_ - rhs.a_);
}

Nome Nome::operator*(const Nome& rhs) const
{
    return Nome(a_ * rhs.a_);
}

Nome Nome::operator/(const Nome& rhs) const
{
    if (rhs.a_ == 0.0)                       // prima il controllo
    {
        throw std::domain_error("divisione per zero");
    }
    return Nome(a_ / rhs.a_);
}
```

**NEL app.cpp**
```cpp
        // Rk: altri operatori aritmetici
        std::cout << "b - a = " << (b - a) << "   (atteso 1)\n";
        std::cout << "a * b = " << (a * b) << "   (atteso 6)\n";
        std::cout << "b / a = " << (b / a) << "   (atteso 1.5)\n\n";
```

---

## O2 · OGGETTO × NUMERO E NUMERO × OGGETTO
**Il testo dice:** "multiply by a number", "scale", "2 * a and a * 2"  
**Scheletro:** C

`a * 2.0` → metodo (l'oggetto è a sinistra). `2.0 * a` → **funzione libera** (a sinistra c'è un numero, non un Nome), fuori dalla classe, che riusa il metodo.

**NEL Nome.h** → OPERATORI
```cpp
    Nome operator*(double k) const;          // a * 2.0
```

**NEL Nome.h** → FUNZIONI LIBERE
```cpp
Nome operator*(double k, const Nome& x);     // 2.0 * a
```

**NEL Nome.cc** → OPERATORI
```cpp
Nome Nome::operator*(double k) const
{
    return Nome(a_ * k);
}
```

**NEL Nome.cc** → FUNZIONI LIBERE
```cpp
// 2.0 * a: niente Nome:: (non e' un metodo); riuso a * 2.0
Nome operator*(double k, const Nome& x)
{
    return x * k;
}
```

**NEL app.cpp**
```cpp
        // Rk: per un numero
        std::cout << "a * 10 = " << (a * 10.0) << ", 10 * a = " << (10.0 * a) << "   (atteso 20 e 20)\n\n";
```

---

## O3 · `+=` `-=` `*=`
**Il testo dice:** "+=", "-=", "compound assignment"  
**Scheletro:** C

Modificano **questo** oggetto: niente `const`; restituiscono `Nome&` con `return *this;` ("me stesso").

**NEL Nome.h** → OPERATORI
```cpp
    Nome& operator+=(const Nome& rhs);       // a += b  (non const: modifica a)
    Nome& operator-=(const Nome& rhs);       // a -= b
    Nome& operator*=(double k);              // a *= 2.0
```

**NEL Nome.cc** → OPERATORI
```cpp
Nome& Nome::operator+=(const Nome& rhs)
{
    setA(a_ + rhs.a_);                       // stessa formula di operator+, passando dal setter
    return *this;
}

Nome& Nome::operator-=(const Nome& rhs)
{
    setA(a_ - rhs.a_);
    return *this;
}

Nome& Nome::operator*=(double k)
{
    setA(a_ * k);
    return *this;
}
```

**NEL app.cpp**
```cpp
        // Rk: operatori composti
        Nome cc(1.0);
        cc += b;
        std::cout << "1 += 3 -> " << cc << "   (atteso 4)\n";
        cc *= 2.0;
        std::cout << "*= 2  -> " << cc << "   (atteso 8)\n\n";
```

---

## O4 · `==` `!=` `<`
**Il testo dice:** "compare", "equal", "less than", "sort"  
**Scheletro:** C

Restituiscono `bool`. Con i `double` **mai** `==` diretto: due numeri "uguali" possono differire nell'ultima cifra → tolleranza.

**NEL Nome.h** → OPERATORI
```cpp
    bool operator==(const Nome& rhs) const;
    bool operator!=(const Nome& rhs) const;
    bool operator<(const Nome& rhs) const;
```

**NEL Nome.cc** → INCLUDE
```cpp
#include <cmath>       // std::fabs
```

**NEL Nome.cc** → OPERATORI
```cpp
bool Nome::operator==(const Nome& rhs) const
{
    return std::fabs(a_ - rhs.a_) < 1e-9 * std::fabs(a_);   // differenza relativa piccola
}

bool Nome::operator!=(const Nome& rhs) const
{
    return !(*this == rhs);                  // riuso ==
}

bool Nome::operator<(const Nome& rhs) const
{
    return a_ < rhs.a_;
}
```

**NEL app.cpp**
```cpp
        // Rk: confronti (1 = vero, 0 = falso)
        std::cout << "a == a: " << (a == a) << ", a != b: " << (a != b) << ", a < b: " << (a < b) << "   (atteso 1 1 1)\n\n";
```

---

## O5 · MENO UNARIO `-a`
**Il testo dice:** "the opposite", "unary minus"  
**Scheletro:** C

Nessun argomento. Attenzione: se il vincolo è `a > 0`, `-a` lancia l'eccezione.

**NEL Nome.h** → OPERATORI
```cpp
    Nome operator-() const;                  // -a: NESSUN argomento
```

**NEL Nome.cc** → OPERATORI
```cpp
Nome Nome::operator-() const
{
    return Nome(-a_);
}
```

**NEL app.cpp**
```cpp
        // Rk: meno unario
        try
        {
            std::cout << "-a = " << (-a) << "\n";
        }
        catch (const std::invalid_argument& e)
        {
            std::cout << "Eccezione catturata (-a con vincolo a > 0): " << e.what() << "\n\n";
        }
```

---

## O6 · SERIE E PARALLELO (`+` e `||`)
**Il testo dice:** "series", "parallel", "overload + and ||" — Capacitor (02/2022), RLC (01/2025)  
**Scheletro:** C — esempio con i **condensatori**.

| | Serie | Parallelo |
|---|---|---|
| **Condensatori** | `1.0 / (1.0 / a_ + 1.0 / rhs.a_)` | `a_ + rhs.a_` |
| **Resistenze** | `a_ + rhs.a_` | `1.0 / (1.0 / a_ + 1.0 / rhs.a_)` |

Leggi nel testo **quale simbolo** è la serie: nel Capacitor `+` = serie e `||` = parallelo.

**SOSTITUISCI in Nome.cc** → `Nome Nome::operator+(const Nome& rhs) const`
```cpp
Nome Nome::operator+(const Nome& rhs) const
{
    return Nome(1.0 / (1.0 / a_ + 1.0 / rhs.a_));   // serie di condensatori
}
```

**NEL Nome.h** → OPERATORI
```cpp
    // a || b = a_ + rhs.a_       (parallelo di condensatori)
    Nome operator||(const Nome& rhs) const;
```

**NEL Nome.cc** → OPERATORI
```cpp
Nome Nome::operator||(const Nome& rhs) const
{
    return Nome(a_ + rhs.a_);                // parallelo: NON si usa || qui dentro
}
```

**SOSTITUISCI in app.cpp** → `std::cout << "a + b = " << (a + b) << "   (atteso 5: con la TUA formula, calcolalo a mano)\n\n";`
```cpp
        std::cout << "a + b (serie)     = " << (a + b) << "   (atteso 2*3/(2+3) = 1.2)\n";
        std::cout << "a || b (parallelo) = " << (a || b) << "   (atteso 5)\n\n";
```

**NEL app.cpp**
```cpp
        // Rk: un circuito si scrive come il disegno
        std::cout << "(a || b) + a = " << ((a || b) + a) << "   (atteso 5*2/(5+2) = 1.43)\n\n";
```

---

## O7 · INDICE `a[i]`
**Il testo dice:** "access the i-th coefficient", "p[i]", "operator[]"  
**Scheletro:** A  
**Richiede:** T3

Ha senso quando la classe contiene un **elenco** (es. i coefficienti di un Polinomio).

**NEL Nome.h** → OPERATORI
```cpp
    double operator[](std::size_t i) const;  // a[i]: l'i-esimo valore (lancia se non esiste)
```

**NEL Nome.cc** → OPERATORI
```cpp
double Nome::operator[](std::size_t i) const
{
    return x_.at(i);                         // .at() lancia std::out_of_range da solo
}
```

**NEL app.cpp**
```cpp
        // Rk: indice
        x.aggiungi(7.0);
        std::cout << "x[0] = " << x[0] << "   (atteso 7)\n\n";
```

---

## O8 · MEMORIA DINAMICA (regola del tre)
**Il testo dice:** "allocate with new", "dynamic array", "manage the memory"  
**Scheletro:** C

Chi usa `new` deve scrivere **tre** cose: distruttore (`delete[]`), costruttore di copia (copia **profonda**: memoria nuova) e assegnazione. Se ne manca una, due oggetti condividono la stessa memoria e il programma si rompe.

**NEL Nome.h** → INCLUDE
```cpp
#include <cstddef>     // std::size_t
```

**NEL Nome.h** → COSTRUTTORI
```cpp
    Nome(const Nome& other);                 // 1. copia profonda
    Nome& operator=(const Nome& other);      // 2. assegnazione profonda
    ~Nome();                                 // 3. distruttore: delete[]
```

**NEL Nome.h** → ATTRIBUTI
```cpp
    std::size_t n_;                          // quanti valori
    double* dati_;                           // memoria presa con new[]
```

**SOSTITUISCI in Nome.cc** → `Nome::Nome(double a)`
```cpp
Nome::Nome(double a)
    : a_(0.0)
    , n_(3)
    , dati_(nullptr)                         // ancora nessuna memoria
{
    setA(a);                                 // i controlli PRIMA...
    dati_ = new double[n_]();                // ...poi la memoria (se il controllo fallisce, niente memoria persa)
}
```

**NEL Nome.cc** → COSTRUTTORI
```cpp
// 1. Copia profonda: memoria NUOVA, poi copio i valori
Nome::Nome(const Nome& other)
    : a_(other.a_)
    , n_(other.n_)
    , dati_(new double[other.n_])
{
    for (std::size_t i = 0; i < n_; ++i)
    {
        dati_[i] = other.dati_[i];
    }
}

// 2. Assegnazione: libero la vecchia, prendo la nuova, copio
Nome& Nome::operator=(const Nome& other)
{
    if (this != &other)                      // a = a: niente da fare
    {
        delete[] dati_;
        a_ = other.a_;
        n_ = other.n_;
        dati_ = new double[n_];
        for (std::size_t i = 0; i < n_; ++i)
        {
            dati_[i] = other.dati_[i];
        }
    }
    return *this;
}

// 3. Distruttore
Nome::~Nome()
{
    delete[] dati_;                          // new[] -> delete[]
}
```

**NEL app.cpp**
```cpp
        // Rk: copia e assegnazione con memoria dinamica
        Nome copia(a);
        copia = b;
        std::cout << "copia = b: " << copia << "   (atteso 3)\n\n";
```

---

## O9 · CLASSI CON PIÙ ATTRIBUTI (Frac, complessi, vettori 2D)
**Il testo dice:** "fraction with numerator and denominator", "complex number"

Stessi operatori, stesse firme: cambia **solo la formula** dentro `return Nome(...)`, che usa tutti gli attributi.

```cpp
// Frac (num_, den_ interi, den_ != 0)
Frac Frac::operator+(const Frac& rhs) const
{
    return Frac(num_ * rhs.den_ + rhs.num_ * den_, den_ * rhs.den_);   // a/b + c/d = (ad + cb)/bd
}

Frac Frac::operator*(const Frac& rhs) const
{
    return Frac(num_ * rhs.num_, den_ * rhs.den_);
}

bool Frac::operator==(const Frac& rhs) const        // interi: == va bene
{
    return num_ * rhs.den_ == rhs.num_ * den_;      // a/b == c/d  <=>  ad == cb
}

// Semplificazione (nel costruttore, dopo il controllo den != 0): #include <numeric>
//     int m = std::gcd(num_, den_);   num_ /= m;   den_ /= m;
//     if (den_ < 0) { num_ = -num_; den_ = -den_; }      // segno sempre al numeratore

// Complesso (re_, im_)
Complesso Complesso::operator+(const Complesso& rhs) const
{
    return Complesso(re_ + rhs.re_, im_ + rhs.im_);
}
```

---

# PROGRAMMA (il main)

## P1 · NUMERI CASUALI E FILE `.dat`
**Il testo dice:** "draw 10^3 random values from a uniform distribution in [1, 100]", "store the pairs in true.dat"  
**Scheletro:** C

Sempre le stesse due righe: un **generatore** (uno solo, seme fisso = risultati ripetibili) e una **distribuzione**.

| Distribuzione | Si scrive |
|---|---|
| uniforme in [x, y] | `std::uniform_real_distribution<double> unif(x, y);` |
| gaussiana (μ, σ) | `std::normal_distribution<double> gauss(mu, sigma);` |
| intero tra 1 e 6 | `std::uniform_int_distribution<int> dado(1, 6);` |
| vero con probabilità p | `std::bernoulli_distribution moneta(p);` |
| esponenziale di media τ | `std::exponential_distribution<double> espo(1.0 / tau);` |

Si estrae un valore con `unif(gen)`.

**NEL app.cpp** → INCLUDE
```cpp
#include <random>      // std::mt19937, distribuzioni
#include <fstream>     // std::ofstream
```

**NEL app.cpp**
```cpp
        // Rk: valori casuali e file di uscita
        std::cout << "=== Numeri casuali ===\n";
        std::mt19937 gen(42);                                     // generatore, seme fisso
        std::uniform_real_distribution<double> unif(1.0, 100.0);  // uniforme in [1, 100]
        std::ofstream out("true.dat");                            // file di uscita
        if (!out)
        {
            throw std::runtime_error("impossibile creare true.dat");
        }
        out << "# x   risultato\n";                               // intestazione (Python la salta)
        const int N = 1000;
        for (int i = 0; i < N; ++i)
        {
            double xi = unif(gen);                                // 1. un valore casuale
            Nome ri = Nome(xi) + b;                               // 2. il calcolo con la classe
            out << xi << " " << ri.a() << "\n";                   // 3. una riga: due colonne
        }
        std::cout << "scritte " << N << " righe in true.dat\n\n";
```

> Il `.dat` va **nello zip**. Unità comode (pF): vedi P3.

---

## P2 · LEGGERE UN FILE DI DATI NEL MAIN
**Il testo dice:** "read the values from data.txt", "use the values in the file"  
**Scheletro:** C

**NUOVO FILE dati.txt**
```text
1.0 2.0
2.0 4.5
3.0 6.0
```

**NEL app.cpp** → INCLUDE
```cpp
#include <fstream>     // std::ifstream
#include <vector>
```

**NEL app.cpp**
```cpp
        // Rk: lettura di un file a due colonne
        std::ifstream in("dati.txt");                             // 1. apro
        if (!in)                                                  // 2. controllo
        {
            throw std::runtime_error("impossibile aprire dati.txt");
        }
        std::vector<double> colonna1, colonna2;
        double u, w;
        while (in >> u >> w)                                      // 3. due numeri per riga, finche' ci sono
        {
            colonna1.push_back(u);
            colonna2.push_back(w);
        }
        std::cout << "lette " << colonna1.size() << " righe   (atteso 3)\n\n";
```

---

## P3 · UNITÀ COMODE (pF, nC, km, ...)
**Il testo dice:** valori in pF, nC, μm ...

Dentro la classe si lavora in **unità SI**. Le unità comode si convertono solo **in entrata** (× pF) e **in uscita** (÷ pF):

```cpp
        const double pF = 1e-12;                      // 1 picofarad in farad
        Nome c2(10.0 * pF);                           // entrata: moltiplico
        out << c2.a() / pF << "\n";                   // uscita: divido -> il file e' in pF
```

---

# GERARCHIE (scheletro D) — gruppi D, E, F

Nello scheletro D: `Base` = la base astratta (es. Shape), `Derivata` = una derivata concreta (es. Square). Nel main c'è già `d` (Derivata con x = 4 dopo R2).

**Le regole in una riga ciascuna:**
- base: distruttore `virtual`; metodi da specializzare `virtual ... = 0`; un solo `operator<<`, sulla base
- derivata: `class X : public Base`; nel costruttore `: Base(label)` per **primo**; ogni virtuale puro riscritto con la **stessa firma** + `override` (solo nel `.h`)
- `virtual`, `override`, `= 0` **non** si ripetono nel `.cc`
- attributi privati della base → nella derivata si usano i **getter** (`label()`), oppure diventano `protected` (D5)

## D1 · UN'ALTRA DERIVATA (sorella)
**Il testo dice:** "Triangle and Rectangle derive from Shape", "implement the classes A, B, C deriving from ..."
**Scheletro:** D

Si copiano `Derivata.h` e `Derivata.cc`, si rinominano (Match Whole Word) e si cambiano attributi e formula. Esempio con **due** attributi.

**NUOVO FILE Derivata2.h**
```cpp
// =====================================================================
// Derivata2.h  -  seconda derivata concreta (sorella di Derivata)
// es. Rectangle : public Shape
// =====================================================================

#ifndef DERIVATA2_H
#define DERIVATA2_H

#include "Base.h"

// Commento: che cosa rappresenta, unita', formula di calcolo().
class Derivata2 : public Base
{
public:
    // ================= COSTRUTTORI =================

    // Regolare: y [unita'] > 0, z [unita'] > 0
    Derivata2(const std::string& label, double y, double z);

    // ================= GETTER =================
    double y() const;
    double z() const;

    // ================= METODI =================
    // calcolo = y_ * z_         (es. area del rettangolo)
    double calcolo() const override;

    // ================= STAMPA =================
    void print(std::ostream& os) const override;

private:
    // ================= ATTRIBUTI =================
    double y_;                           // [unita']
    double z_;                           // [unita']

    // ================= METODI DI SERVIZIO =================
    void controlla(double v) const;      // lancia se v <= 0
};

#endif
```

**NUOVO FILE Derivata2.cc**
```cpp
// =====================================================================
// Derivata2.cc  -  IMPLEMENTAZIONE di Derivata2
// =====================================================================

#include "Derivata2.h"

#include <stdexcept>

// ===================== COSTRUTTORI =====================

Derivata2::Derivata2(const std::string& label, double y, double z)
    : Base(label)                        // la base per PRIMA
    , y_(y)
    , z_(z)
{
    controlla(y_);
    controlla(z_);
}

// ===================== GETTER =====================

double Derivata2::y() const
{
    return y_;
}

double Derivata2::z() const
{
    return z_;
}

// ===================== METODI =====================

// calcolo = y_ * z_
double Derivata2::calcolo() const
{
    return y_ * z_;
}

// ===================== STAMPA =====================

void Derivata2::print(std::ostream& os) const
{
    os << "Derivata2 '" << label() << "': y = " << y_ << ", z = " << z_ << ", calcolo = " << calcolo();
}

// ===================== METODI DI SERVIZIO =====================

void Derivata2::controlla(double v) const
{
    if (v <= 0.0)
    {
        throw std::invalid_argument("le dimensioni devono essere > 0");
    }
}
```

**NEL app.cpp** → INCLUDE
```cpp
#include "Derivata2.h"
```

**NEL app.cpp**
```cpp
        // Rk: la seconda derivata, anche lei usata tramite la base
        std::cout << "=== Derivata2 ===\n";
        Derivata2 d2("rett", 2.0, 5.0);
        const Base& rif2 = d2;
        std::cout << rif2 << "   (atteso calcolo = 10)\n\n";
```

> Compilazione: aggiungi `Derivata2.cc` al comando: `g++ ... Base.cc Derivata.cc Derivata2.cc app.cpp -o app`.

---

## D2 · DERIVATA DI SECONDO LIVELLO
**Il testo dice:** "Cube derives from Square", "a Cone is a ... with ...", "extends the class ..."
**Scheletro:** D

Eredita da `Derivata` (non da `Base`): riceve tutto quello che ha Derivata e può **riusarne** i metodi chiamando la versione del genitore con `Derivata::calcolo()`.

**NUOVO FILE Livello2.h**
```cpp
// =====================================================================
// Livello2.h  -  derivata di SECONDO livello: Base -> Derivata -> Livello2
// es. Cube : public Square
// =====================================================================

#ifndef LIVELLO2_H
#define LIVELLO2_H

#include "Derivata.h"  // eredita da Derivata

// Commento: che cosa aggiunge rispetto a Derivata.
class Livello2 : public Derivata
{
public:
    // ================= COSTRUTTORI =================

    // Regolare: x va a Derivata, h [unita'] > 0 e' nuovo
    Livello2(const std::string& label, double x, double h);

    // ================= GETTER =================
    double h() const;

    // ================= METODI =================
    // calcolo = (calcolo di Derivata) * h_      (es. volume = area di base * altezza)
    double calcolo() const override;

    // ================= STAMPA =================
    void print(std::ostream& os) const override;

private:
    // ================= ATTRIBUTI =================
    double h_;                           // [unita']
};

#endif
```

**NUOVO FILE Livello2.cc**
```cpp
// =====================================================================
// Livello2.cc  -  IMPLEMENTAZIONE di Livello2
// =====================================================================

#include "Livello2.h"

#include <stdexcept>

// ===================== COSTRUTTORI =====================

Livello2::Livello2(const std::string& label, double x, double h)
    : Derivata(label, x)                 // il GENITORE diretto per primo (che costruisce la Base)
    , h_(h)
{
    if (h_ <= 0.0)
    {
        throw std::invalid_argument("h deve essere > 0");
    }
}

// ===================== GETTER =====================

double Livello2::h() const
{
    return h_;
}

// ===================== METODI =====================

double Livello2::calcolo() const
{
    return Derivata::calcolo() * h_;     // Derivata::calcolo() = la versione del GENITORE
}

// ===================== STAMPA =====================

void Livello2::print(std::ostream& os) const
{
    os << "Livello2 '" << label() << "': x = " << x() << ", h = " << h_ << ", calcolo = " << calcolo();
}
```

**NEL app.cpp** → INCLUDE
```cpp
#include "Livello2.h"
```

**NEL app.cpp**
```cpp
        // Rk: secondo livello
        std::cout << "=== Livello 2 ===\n";
        Livello2 cubo("cubo", 2.0, 3.0);
        const Base& rif3 = cubo;
        std::cout << rif3 << "   (atteso calcolo = 2*2*3 = 12)\n\n";
```

> Senza `Derivata::` davanti, `calcolo()` dentro `Livello2::calcolo()` chiamerebbe **se stesso** all'infinito.

---

## D3 · METODO VIRTUALE CON UNA VERSIONE DI DEFAULT
**Il testo dice:** "a method that returns the name of the shape", "can be redefined by the derived classes"
**Scheletro:** D

`virtual` **senza** `= 0`: la base lo scrive; ogni derivata **può** ridefinirlo (con `override`), oppure tenersi quello della base.

**NEL Base.h** → METODI
```cpp
    // Virtuale CON versione di default: le derivate POSSONO ridefinirlo
    virtual std::string tipo() const;
```

**NEL Base.cc** → METODI
```cpp
std::string Base::tipo() const           // niente virtual nel .cc
{
    return "forma generica";
}
```

**NEL Derivata.h** → METODI
```cpp
    std::string tipo() const override;
```

**NEL Derivata.cc** → METODI
```cpp
std::string Derivata::tipo() const       // niente override nel .cc
{
    return "quadrato";
}
```

**NEL app.cpp**
```cpp
        // Rk: virtuale con default
        std::cout << "tipo tramite la base: " << rif.tipo() << "   (atteso quadrato)\n\n";
```

---

## D4 · METODO COMUNE NELLA BASE (usa un virtuale)
**Il testo dice:** "the energy is Q^2/(2C) for every capacitor", "volume = area * height for all prisms"
**Scheletro:** D

Scritto **una volta** nella base, **non** virtuale; dentro chiama un virtuale (`calcolo()`), quindi funziona per ogni derivata.

**NEL Base.h** → METODI
```cpp
    // Uguale per tutte le derivate: doppio = 2 * calcolo()
    double doppio() const;
```

**NEL Base.cc** → METODI
```cpp
double Base::doppio() const
{
    return 2.0 * calcolo();              // chiamata virtuale: usa il calcolo della derivata vera
}
```

**NEL app.cpp**
```cpp
        // Rk: metodo comune
        std::cout << "doppio = " << d.doppio() << "   (atteso 32)\n\n";
```

---

## D5 · ATTRIBUTO PROTECTED
**Il testo dice:** "accessible by the derived classes", "protected"
**Scheletro:** D

`protected` = lo vedono le derivate (senza getter), **non** il main. Si inizializza nel costruttore della **base**.

**NEL Base.h** → ATTRIBUTI PROTECTED
```cpp
    double scala_;                       // es. un fattore comune [unita']
```

**SOSTITUISCI in Base.cc** → `Base::Base(const std::string& label)`
```cpp
Base::Base(const std::string& label)
    : scala_(1.0)                        // nel .h protected viene PRIMA di private:
    , label_(label)                      // quindi scala_ e' dichiarato prima di label_
{
}
```

**SOSTITUISCI in Derivata.cc** → `double Derivata::calcolo() const`
```cpp
double Derivata::calcolo() const
{
    return x_ * x_ * scala_;             // scala_ della base, usato direttamente
}
```

**NEL app.cpp**
```cpp
        // Rk: protected
        std::cout << "calcolo con scala = " << d.calcolo() << "   (atteso 16)\n\n";
```

---

## D6 · ELENCO POLIMORFICO (`std::vector<Base*>`)
**Il testo dice:** "store different shapes in a vector", "loop over all the objects", "polymorphism"
**Scheletro:** D
**Richiede:** D1

Un `vector` di **puntatori alla base**: ogni elemento può essere una derivata diversa. Creati con `new`, vanno distrutti con `delete` (il distruttore virtuale fa partire quello giusto).

**NEL app.cpp** → INCLUDE
```cpp
#include <vector>
```

**NEL app.cpp**
```cpp
        // Rk: elenco polimorfico
        std::cout << "=== Elenco polimorfico ===\n";
        std::vector<Base*> forme;
        forme.push_back(new Derivata("q", 2.0));
        forme.push_back(new Derivata2("r", 2.0, 3.0));
        double somma = 0.0;
        for (const Base* f : forme)               // ogni f "vede" una Base
        {
            std::cout << *f << "\n";              // *f = l'oggetto puntato; print della derivata vera
            somma += f->calcolo();                // -> per chiamare un metodo con un puntatore
        }
        std::cout << "somma = " << somma << "   (atteso 4 + 6 = 10)\n";
        for (Base* f : forme)
        {
            delete f;                             // ogni new ha il suo delete
        }
        std::cout << "\n";
```

---

## D7 · CONTATORE NELLA GERARCHIA (gruppo F)
**Il testo dice:** "number of objects existing at any time", "count the instances"
**Scheletro:** D

Il contatore sta nella **base**: ogni derivata, quando nasce, costruisce la base → conta da sola. Va contata anche la **copia** (costruttore di copia della base).

**NEL Base.h** → COSTRUTTORI
```cpp
    // Copia: anche le copie vanno contate
    Base(const Base& other);
    Base& operator=(const Base& other) = default;
```

**NEL Base.h** → METODI
```cpp
    static int count();                  // oggetti esistenti adesso (di qualunque derivata)
```

**NEL Base.h** → ATTRIBUTI
```cpp
    static int count_;                   // condiviso da tutti
```

**NEL Base.cc** → COSTANTI
```cpp
int Base::count_ = 0;                    // definizione: UNA volta, senza static
```

**SOSTITUISCI in Base.cc** → `Base::Base(const std::string& label)`
```cpp
Base::Base(const std::string& label)
    : label_(label)
{
    ++count_;
}

// Copia
Base::Base(const Base& other)
    : label_(other.label_)
{
    ++count_;
}
```

**SOSTITUISCI in Base.cc** → `Base::~Base()`
```cpp
Base::~Base()
{
    --count_;
}
```

**NEL Base.cc** → METODI
```cpp
int Base::count()                        // niente static nel .cc
{
    return count_;
}
```

**NEL app.cpp**
```cpp
        // Rk: contatore
        std::cout << "oggetti: " << Base::count() << "\n";
        {
            Derivata t1("t1", 1.0);
            Derivata t2(t1);                      // copia
            std::cout << "nel blocco: " << Base::count() << "   (2 in piu')\n";
        }
        std::cout << "dopo il blocco: " << Base::count() << "   (come all'inizio)\n\n";
```

> **Attenzione:** se una derivata ha un controllo che fallisce, la base è già stata costruita (e contata) e poi viene distrutta (e scontata): il conto torna giusto da solo.

---

## D8 · DATI NELLA BASE, CALCOLI DIVERSI NELLE DERIVATE (gruppo E)
**Il testo dice:** "DataSet reads the data from a file; ArithmeticMean, GeometricMean, HarmonicMean derive from it"
**Scheletro:** D

La **base** legge e tiene i dati; ogni **derivata** dice solo come calcolare (media aritmetica, geometrica, armonica...). Qui una nuova derivata `Aritmetica`.

**NUOVO FILE dati.txt**
```text
1.0
2.0
6.0
```

**NEL Base.h** → INCLUDE
```cpp
#include <vector>      // std::vector -> i dati
#include <cstddef>     // std::size_t
```

**NEL Base.h** → COSTRUTTORI
```cpp
    // Da file: legge tutti i numeri del file
    Base(const std::string& label, const std::string& nomeFile);
```

**NEL Base.h** → GETTER
```cpp
    const std::vector<double>& dati() const;   // le derivate leggono i dati con questo
    std::size_t size() const;
```

**NEL Base.h** → ATTRIBUTI
```cpp
    std::vector<double> dati_;
```

**SOSTITUISCI in Base.cc** → `Base::Base(const std::string& label)`
```cpp
Base::Base(const std::string& label)
    : label_(label)
    , dati_()
{
}

// Da file
Base::Base(const std::string& label, const std::string& nomeFile)
    : label_(label)
    , dati_()
{
    std::ifstream file(nomeFile);                // 1. apro
    if (!file)                                   // 2. controllo
    {
        throw std::runtime_error("impossibile aprire " + nomeFile);
    }
    double v;
    while (file >> v)                            // 3. tutti i numeri del file
    {
        dati_.push_back(v);
    }
    if (dati_.empty())
    {
        throw std::runtime_error("nessun dato in " + nomeFile);
    }
}
```

**NEL Base.cc** → INCLUDE
```cpp
#include <fstream>     // std::ifstream
```

**NEL Base.cc** → GETTER
```cpp
const std::vector<double>& Base::dati() const
{
    return dati_;
}

std::size_t Base::size() const
{
    return dati_.size();
}
```

**NUOVO FILE Aritmetica.h**
```cpp
// =====================================================================
// Aritmetica.h  -  media aritmetica dei dati della base
// =====================================================================

#ifndef ARITMETICA_H
#define ARITMETICA_H

#include "Base.h"

class Aritmetica : public Base
{
public:
    // ================= COSTRUTTORI =================
    Aritmetica(const std::string& label, const std::string& nomeFile);

    // ================= METODI =================
    // calcolo = somma(dati) / N
    double calcolo() const override;

    // ================= STAMPA =================
    void print(std::ostream& os) const override;
};

#endif
```

**NUOVO FILE Aritmetica.cc**
```cpp
// =====================================================================
// Aritmetica.cc  -  IMPLEMENTAZIONE di Aritmetica
// =====================================================================

#include "Aritmetica.h"

// ===================== COSTRUTTORI =====================

Aritmetica::Aritmetica(const std::string& label, const std::string& nomeFile)
    : Base(label, nomeFile)              // e' la BASE a leggere il file
{
}

// ===================== METODI =====================

// somma / N        (geometrica: prodotto^(1/N);  armonica: N / somma(1/x))
double Aritmetica::calcolo() const
{
    double somma = 0.0;
    for (double v : dati())              // dati() = getter della base
    {
        somma += v;
    }
    return somma / size();
}

// ===================== STAMPA =====================

void Aritmetica::print(std::ostream& os) const
{
    os << "Media aritmetica di " << size() << " dati: " << calcolo();
}
```

**NEL app.cpp** → INCLUDE
```cpp
#include "Aritmetica.h"
```

**NEL app.cpp**
```cpp
        // Rk: dati nella base
        std::cout << "=== Dati ===\n";
        Aritmetica media("media", "dati.txt");
        std::cout << media << "   (atteso (1 + 2 + 6)/3 = 3)\n\n";
```

---

## D9 · "Q OPPURE ΔV": `enum class`
**Il testo dice:** "the capacitor can be given either the charge Q or the voltage ΔV", "the user specifies which"
**Scheletro:** D

Due modi di dare un dato, con **lo stesso tipo** (`double`): un `enum class` dice quale è stato dato. (Alternativa: named constructor, scheda K7.)

L'`enum class` va **sotto gli `#include`**, prima della classe.

**NEL Derivata.h** → INCLUDE
```cpp
// Quale dato e' stato fornito (es. { Carica, Tensione })
enum class Dato { Diretto, Inverso };
```

**NEL Derivata.h** → COSTRUTTORI
```cpp
    // Con un dato in piu': se Diretto, valore e' v; se Inverso, v = calcolo() * valore
    Derivata(const std::string& label, double x, Dato dato, double valore);
```

**NEL Derivata.h** → GETTER
```cpp
    double v() const;
```

**NEL Derivata.h** → ATTRIBUTI
```cpp
    double v_;                           // es. la carica Q [C]
```

**SOSTITUISCI in Derivata.cc** → `Derivata::Derivata(const std::string& label, double x)`
```cpp
Derivata::Derivata(const std::string& label, double x)
    : Base(label)
    , x_(x)
    , v_(0.0)                            // anche qui: ogni attributo
{
    controlla(x_);
}

// Con il dato: Diretto -> v = valore;  Inverso -> v = calcolo() * valore  (es. Q = C * DeltaV)
Derivata::Derivata(const std::string& label, double x, Dato dato, double valore)
    : Base(label)
    , x_(x)
    , v_(0.0)
{
    controlla(x_);
    if (dato == Dato::Diretto)           // si scrive NomeEnum::Valore
    {
        v_ = valore;
    }
    else
    {
        v_ = calcolo() * valore;         // calcolo() qui funziona: siamo nella derivata
    }
}
```

**NEL Derivata.cc** → GETTER
```cpp
double Derivata::v() const
{
    return v_;
}
```

**NEL app.cpp**
```cpp
        // Rk: enum class
        Derivata dq("dq", 2.0, Dato::Diretto, 5.0);
        Derivata dv("dv", 2.0, Dato::Inverso, 5.0);
        std::cout << "diretto: v = " << dq.v() << "   (atteso 5),  inverso: v = " << dv.v() << "   (atteso 4 * 5 = 20)\n\n";
```

---

## D10 · REGOLE DA RICORDARE (senza codice)

| Situazione | Cosa scrivere |
|---|---|
| la base deve obbligare le derivate a scrivere un metodo | `virtual double f() const = 0;` |
| la base ha una versione, le derivate possono cambiarla | `virtual double f() const;` (D3) |
| metodo uguale per tutte | metodo normale nella base (D4) |
| la derivata riscrive un virtuale | stessa firma **+ `override`** (se la firma è diversa, `override` dà errore: ti salva) |
| chiamare la versione del genitore | `Genitore::f()` (D2) |
| costruttore della derivata | `: Base(label), x_(x)` — la base **per prima** |
| errore `allocating an object of abstract class type` | stai creando la base, o una derivata non ha scritto tutti gli `= 0` |
| errore `marked 'override' but does not override` | firma diversa dalla base (spesso manca `const`) |
| grandezza memorizzata nella derivata | come T4, nella derivata |
| attributo in più nella derivata | come T1, nella derivata |

---

## D11 · FUNZIONE DEL MAIN CHE LAVORA CON QUALUNQUE DERIVATA
**Il testo dice:** "write a function that prints ... for any shape"
**Scheletro:** D

Riceve un `const Base&`: le si può passare qualunque derivata.

**NEL app.cpp** → FUNZIONI DEL MAIN
```cpp
// Stampa nome e calcolo di qualunque oggetto della famiglia
void descrivi(const Base& b)
{
    std::cout << b.label() << " -> " << b.calcolo() << "\n";
}
```

**NEL app.cpp**
```cpp
        // Rk: funzione polimorfica
        descrivi(d);
        std::cout << "   (atteso quadrato -> 16)\n\n";
```

---

# FUNCTION DATA DAL TESTO (scheletro G)

Nello scheletro G: `Function.h` = la classe **del testo**, da incollare identica; `Derivata` = la tua derivata (nell'esempio sin x). Nel main c'è già `f` (il seno) e `g` (riferimento alla base).

**La regola d'oro:** le firme dei virtuali puri si **copiano dal testo**, carattere per carattere (`const` compreso), e si aggiunge `override`.

## G1 · DISTRUTTORE VIRTUALE NELLA CLASSE DEL TESTO
**Il testo dice:** la classe data non ha il distruttore virtuale (capita spesso)
**Scheletro:** G

È l'**unica** modifica permessa alla classe del testo; va dichiarata in un commento.

**SOSTITUISCI in Function.h** → `Function(const std::string& name) { name_ = name; };`
```cpp
  Function(const std::string& name) { name_ = name; };
  // AGGIUNTO (non c'era nel testo): distruttore virtuale, necessario per
  // distruggere correttamente le derivate tramite un Function*
  virtual ~Function() = default;
```

---

## G2 · POLINOMIO (coefficienti in un `vector`, grado memorizzato)
**Il testo dice:** "Polynomial with coefficients a0, a1, ...", "store the degree"
**Scheletro:** G

Una **nuova derivata**. `value` con lo schema di Horner: `(((a_n x + a_{n-1}) x + ...) x + a_0)`.

**NUOVO FILE Polinomio.h**
```cpp
// =====================================================================
// Polinomio.h  -  p(x) = a0 + a1 x + a2 x^2 + ...   (derivata di Function)
// =====================================================================

#ifndef POLINOMIO_H
#define POLINOMIO_H

#include "Function.h"
#include <vector>

class Polinomio : public Function
{
public:
    // ================= COSTRUTTORI =================

    // coeff = {a0, a1, a2, ...} (potenze crescenti), non vuoto
    Polinomio(const std::string& name, const std::vector<double>& coeff);

    // ================= GETTER =================
    int grado() const;                       // memorizzato
    const std::vector<double>& coeff() const;

    // ================= SETTER =================
    void setCoeff(const std::vector<double>& coeff);

    // ================= METODI =================
    double value(double x) const override;
    double deriv(double x) const override;

private:
    // ================= ATTRIBUTI =================
    std::vector<double> coeff_;              // a0, a1, a2, ...
    int grado_;                              // memorizzato: DOPO coeff_

    // ================= METODI DI SERVIZIO =================
    void update();                           // ricalcola grado_
};

#endif
```

**NUOVO FILE Polinomio.cc**
```cpp
// =====================================================================
// Polinomio.cc  -  IMPLEMENTAZIONE di Polinomio
// =====================================================================

#include "Polinomio.h"

#include <stdexcept>

// ===================== COSTRUTTORI =====================

Polinomio::Polinomio(const std::string& name, const std::vector<double>& coeff)
    : Function(name)
    , coeff_(coeff)
    , grado_(0)
{
    if (coeff_.empty())
    {
        throw std::invalid_argument("servono dei coefficienti");
    }
    update();
}

// ===================== GETTER =====================

int Polinomio::grado() const
{
    return grado_;
}

const std::vector<double>& Polinomio::coeff() const
{
    return coeff_;
}

// ===================== SETTER =====================

void Polinomio::setCoeff(const std::vector<double>& coeff)
{
    if (coeff.empty())                       // prima il controllo
    {
        throw std::invalid_argument("servono dei coefficienti");
    }
    coeff_ = coeff;                          // poi l'assegnazione
    update();                                // poi il ricalcolo
}

// ===================== METODI =====================

// Horner: r = a_n;  r = r*x + a_{n-1};  ...  r = r*x + a_0
double Polinomio::value(double x) const
{
    double r = 0.0;
    for (int k = grado_; k >= 0; --k)        // dal coefficiente piu' alto al piu' basso
    {
        r = r * x + coeff_[k];
    }
    return r;
}

// p'(x) = a1 + 2 a2 x + 3 a3 x^2 + ...  (Horner sui coefficienti k * a_k)
double Polinomio::deriv(double x) const
{
    double r = 0.0;
    for (int k = grado_; k >= 1; --k)
    {
        r = r * x + k * coeff_[k];
    }
    return r;
}

// ===================== METODI DI SERVIZIO =====================

// grado = indice dell'ultimo coefficiente NON nullo
void Polinomio::update()
{
    grado_ = static_cast<int>(coeff_.size()) - 1;   // size() e' size_t: lo converto in int
    while (grado_ > 0 && coeff_[grado_] == 0.0)
    {
        --grado_;
    }
}
```

**NEL app.cpp** → INCLUDE
```cpp
#include "Polinomio.h"
```

**NEL app.cpp**
```cpp
        // Rk: polinomio
        std::cout << "=== Polinomio ===\n";
        Polinomio p("p", {1.0, -3.0, 2.0});       // 1 - 3x + 2x^2
        std::cout << "p(2) = " << p.value(2.0) << ", p'(2) = " << p.deriv(2.0) << ", grado " << p.grado()
                  << "   (atteso 1 - 6 + 8 = 3,  -3 + 8 = 5,  2)\n";
        p.setCoeff({4.0, 0.0, 0.0});
        std::cout << "dopo setCoeff({4, 0, 0}): grado " << p.grado() << "   (atteso 0: gli zeri in fondo non contano)\n\n";
```

---

## G3 · FUNZIONE FATTA DI ALTRE FUNZIONI (rapporto, somma, composizione)
**Il testo dice:** "Rational = N(x)/D(x)", "a function built from two functions"
**Scheletro:** G

Eredita da `Function` **e** contiene due `Function` (composizione + ereditarietà). Le tiene come **riferimenti** `const Function&`: così accetta qualunque derivata. Gli oggetti originali devono esistere finché si usa il rapporto.

**NUOVO FILE Rapporto.h**
```cpp
// =====================================================================
// Rapporto.h  -  r(x) = n(x) / d(x), con n e d funzioni qualsiasi
// =====================================================================

#ifndef RAPPORTO_H
#define RAPPORTO_H

#include "Function.h"

class Rapporto : public Function
{
public:
    // ================= COSTRUTTORI =================
    Rapporto(const std::string& name, const Function& num, const Function& den);

    // ================= METODI =================
    // value = n / d        deriv = (n' d - n d') / d^2      (lancia se d = 0)
    double value(double x) const override;
    double deriv(double x) const override;

private:
    // ================= ATTRIBUTI =================
    const Function& num_;                    // riferimenti: vanno inizializzati nella LISTA
    const Function& den_;
};

#endif
```

**NUOVO FILE Rapporto.cc**
```cpp
// =====================================================================
// Rapporto.cc  -  IMPLEMENTAZIONE di Rapporto
// =====================================================================

#include "Rapporto.h"

#include <stdexcept>

// ===================== COSTRUTTORI =====================

Rapporto::Rapporto(const std::string& name, const Function& num, const Function& den)
    : Function(name)
    , num_(num)
    , den_(den)
{
}

// ===================== METODI =====================

double Rapporto::value(double x) const
{
    double d = den_.value(x);                // chiamata virtuale: la funzione vera
    if (d == 0.0)
    {
        throw std::domain_error("denominatore nullo");
    }
    return num_.value(x) / d;
}

double Rapporto::deriv(double x) const
{
    double d = den_.value(x);
    if (d == 0.0)
    {
        throw std::domain_error("denominatore nullo");
    }
    return (num_.deriv(x) * d - num_.value(x) * den_.deriv(x)) / (d * d);
}
```

**NEL app.cpp** → INCLUDE
```cpp
#include "Rapporto.h"
```

**NEL app.cpp**
```cpp
        // Rk: rapporto di funzioni
        Rapporto r("f/f", f, f);
        std::cout << "r(1) = " << r.value(1.0) << ", r'(1) = " << r.deriv(1.0) << "   (atteso 1 e 0)\n";
        try
        {
            r.value(0.0);                         // sin(0) = 0 al denominatore
            std::cout << "ERRORE: doveva lanciare un'eccezione\n";
        }
        catch (const std::domain_error& e)
        {
            std::cout << "Eccezione catturata: " << e.what() << "\n\n";
        }
```

> Per una **somma**: `value = num_.value(x) + den_.value(x)`, `deriv` = somma delle derivate. Per una **composizione** f(g(x)): `value = f_.value(g_.value(x))`, `deriv = f_.deriv(g_.value(x)) * g_.deriv(x)`.

---

## G4 · METODO IN PIÙ: PRIMITIVA
**Il testo dice:** "add a method that returns the primitive", "integral"
**Scheletro:** G

Un metodo **non** virtuale in più nella derivata (la base non lo conosce: si chiama sull'oggetto `Derivata`, non su `Function&`).

**NEL Derivata.h** → METODI
```cpp
    // primitiva = -std::cos(x)
    double primitive(double x) const;
```

**NEL Derivata.cc** → METODI
```cpp
double Derivata::primitive(double x) const
{
    return -std::cos(x);
}
```

**NEL app.cpp**
```cpp
        // Rk: primitiva e integrale definito
        const double PI = std::acos(-1.0);
        std::cout << "integrale da 0 a pi = " << f.primitive(PI) - f.primitive(0.0) << "   (atteso 2)\n\n";
```

---

## G5 · CONTROLLO: DERIVATA NUMERICA
**Il testo dice:** niente, ma è il modo più veloce per **verificare** che `deriv` sia giusta (controllo n. 3).
**Scheletro:** G

Differenza centrata: `f'(x) ≈ [f(x + h) - f(x - h)] / (2h)`. Funziona con qualunque derivata.

**NEL app.cpp** → FUNZIONI DEL MAIN
```cpp
// Derivata numerica (differenza centrata) di una Function qualsiasi
double derivataNumerica(const Function& fun, double x, double h = 1e-5)
{
    return (fun.value(x + h) - fun.value(x - h)) / (2.0 * h);
}
```

**NEL app.cpp**
```cpp
        // Rk: verifica di deriv con la derivata numerica
        for (double xv : {0.0, 0.5, 1.0, 2.0})
        {
            std::cout << "x = " << xv << ": deriv = " << f.deriv(xv)
                      << ", numerica = " << derivataNumerica(f, xv) << "   (devono coincidere)\n";
        }
        std::cout << "\n";
```

---

## G6 · TABELLA SU FILE (per il grafico in Python)
**Il testo dice:** "write x, f(x), f'(x) for N points in [a, b] to a file", "plot with Python"
**Scheletro:** G

**NEL app.cpp** → INCLUDE
```cpp
#include <fstream>     // std::ofstream
```

**NEL app.cpp**
```cpp
        // Rk: tabella su file
        {
            const double xa = 0.0, xb = 6.0;      // intervallo [a, b]
            const int N = 100;                    // numero di intervalli -> N + 1 punti
            std::ofstream out("funzione.dat");
            if (!out)
            {
                throw std::runtime_error("impossibile creare funzione.dat");
            }
            out << "# x  f(x)  f'(x)\n";
            for (int i = 0; i <= N; ++i)
            {
                double xi = xa + i * (xb - xa) / N;   // passo costante, senza accumulare errori
                out << xi << " " << f.value(xi) << " " << f.deriv(xi) << "\n";
            }
            std::cout << "scritto funzione.dat (" << N + 1 << " righe)\n\n";
        }
```

> In Python: `x, y, dy = np.loadtxt("funzione.dat", unpack=True)` (le righe con `#` vengono saltate).

---

## G7 · PARAMETRI DELLA FUNZIONE (con setter)
**Il testo dice:** "f(x) = A sin(ωx)", "amplitude and frequency", "setters for the parameters"
**Scheletro:** G

**SOSTITUISCI in Derivata.h** → `explicit Derivata(const std::string& name);`
```cpp
    // A [unita'] qualsiasi, omega [rad/unita'] > 0
    Derivata(const std::string& name, double A, double omega);
```

**NEL Derivata.h** → GETTER
```cpp
    double A() const;
    double omega() const;
```

**NEL Derivata.h** → SETTER
```cpp
    void setA(double A);
    void setOmega(double omega);
```

**NEL Derivata.h** → ATTRIBUTI
```cpp
    double A_;                           // ampiezza
    double omega_;                       // pulsazione, > 0
```

**SOSTITUISCI in Derivata.cc** → `Derivata::Derivata(const std::string& name)`
```cpp
Derivata::Derivata(const std::string& name, double A, double omega)
    : Function(name)
    , A_(A)
    , omega_(0.0)
{
    setOmega(omega);                     // il setter controlla
}
```

**NEL Derivata.cc** → GETTER
```cpp
double Derivata::A() const
{
    return A_;
}

double Derivata::omega() const
{
    return omega_;
}
```

**NEL Derivata.cc** → SETTER
```cpp
void Derivata::setA(double A)
{
    A_ = A;
}

void Derivata::setOmega(double omega)
{
    if (omega <= 0.0)
    {
        throw std::invalid_argument("omega deve essere > 0");
    }
    omega_ = omega;
}
```

**SOSTITUISCI in Derivata.cc** → `double Derivata::value(double x) const`
```cpp
double Derivata::value(double x) const
{
    return A_ * std::sin(omega_ * x);
}
```

**SOSTITUISCI in Derivata.cc** → `double Derivata::deriv(double x) const`
```cpp
double Derivata::deriv(double x) const
{
    return A_ * omega_ * std::cos(omega_ * x);   // regola della catena
}
```

**SOSTITUISCI in app.cpp** → `Derivata f("seno");`
```cpp
        Derivata f("seno", 1.0, 1.0);             // A = 1, omega = 1: stessi valori attesi di prima
```

**NEL app.cpp**
```cpp
        // Rk: parametri
        f.setA(2.0);
        f.setOmega(3.0);
        std::cout << "2 sin(3x) in 0: f' = " << f.deriv(0.0) << "   (atteso A * omega = 6)\n\n";
```

---

## G8 · FUNZIONE DI PIÙ VARIABILI (es. 02/2025)
**Il testo dice:** "value(x, y, z)", "deriv(x, y, z, var) with var = 0, 1, 2"

Stesse regole, firme copiate dal testo. La derivata parziale si sceglie con un `if` (o `switch`) sulla variabile, e un valore non valido lancia:

```cpp
// f(x, y, z) = x^2 y + z
double Campo::deriv(double x, double y, double /*z*/, int var) const   // z non serve qui: nome commentato
{
    if (var == 0)
    {
        return 2.0 * x * y;                  // df/dx
    }
    if (var == 1)
    {
        return x * x;                        // df/dy
    }
    if (var == 2)
    {
        return 1.0;                          // df/dz
    }
    throw std::invalid_argument("var deve essere 0, 1 o 2");
}
```
Se un parametro non serve in una formula (qui `z` in `deriv`), il compilatore avvisa "unused parameter": togli il nome e lascialo commentato, `double /*z*/`.

---

# STRATEGY (scheletro H)

Nello scheletro H: `Sistema` (la fisica, base astratta) → `Modello` (l'oscillatore armonico); `Integratore` (l'algoritmo, base astratta) → `Euler`; `Simulazione` ripete i passi e scrive `sim.dat`. Nel main ci sono già `sistema`, `u0`, `dt`, `N`, `euler`, `scelto`.

**L'idea:** per cambiare algoritmo non si tocca la Simulazione: si crea un'altra derivata di `Integratore` e la si passa al posto di Euler.

## S1 · RUNGE-KUTTA DEL SECONDO ORDINE
**Il testo dice:** "implement also the RK2 integrator", "second-order Runge-Kutta"
**Scheletro:** H

`K1 = f(t, u)`;  `K2 = f(t + dt/2, u + K1 dt/2)`;  `u_{i+1} = u_i + K2 dt`

**NEL Integratore.h** → ALTRE STRATEGIE
```cpp
// RK2:  K1 = f(t, u);  K2 = f(t + dt/2, u + K1 dt/2);  u_{i+1} = u_i + K2 dt
class RungeKutta2 : public Integratore
{
public:
    Stato passo(const Sistema& s, double t, const Stato& u, double dt) const override;
    std::string nome() const override;
};
```

**NEL Integratore.cc** → EULER (in fondo al file)
```cpp
// ===================== RUNGE-KUTTA 2 =====================

Stato RungeKutta2::passo(const Sistema& s, double t, const Stato& u, double dt) const
{
    Stato K1 = s.derivate(t, u);                                       // 1. pendenza all'inizio
    Stato K2 = s.derivate(t + dt / 2.0, combina(u, dt / 2.0, K1));     // 2. pendenza a meta' passo
    return combina(u, dt, K2);                                         // 3. passo intero con K2
}

std::string RungeKutta2::nome() const
{
    return "RungeKutta2";
}
```

**NEL app.cpp**
```cpp
        // Rk: Runge-Kutta 2
        RungeKutta2 rk2;
        Stato fineRK = Simulazione(sistema, rk2).esegui(u0, dt, N, "sim_rk2.dat");
        std::cout << rk2.nome() << ": x(T) = " << fineRK[0] << "   (piu' vicino all'esatto di Euler)\n\n";
```

> **Per sceglierlo per nome** (es. da `params.ini`): nel blocco R3 del main aggiungi `RungeKutta2 rk2;` sotto `Euler euler;` e, prima dell'`else`, il ramo
> `else if (metodo == "RK2") { scelto = &rk2; }`

---

## S2 · PARAMETRI DA `params.ini`
**Il testo dice:** "read the parameters from params.ini", "the integrator is chosen in the parameter file"
**Scheletro:** H

Il file ha righe `chiave valore`. Si leggono tutte in un **dizionario** (`std::map`: chiave → valore, entrambi testo), poi si convertono: `std::stod` (testo → double), `std::stoi` (testo → int). `.at("chiave")` lancia un'eccezione se la chiave manca.

**NUOVO FILE params.ini**
```ini
metodo Euler
dt 0.001
N 1000
omega 1.0
x0 1.0
v0 0.0
```

**NEL app.cpp** → INCLUDE
```cpp
#include <fstream>     // std::ifstream
#include <map>         // std::map: dizionario chiave -> valore
```

**SOSTITUISCI in app.cpp** → `std::string metodo = "Euler";`
```cpp
        std::ifstream in("params.ini");
        if (!in)
        {
            throw std::runtime_error("manca params.ini");
        }
        std::map<std::string, std::string> par;    // es. par["dt"] = "0.001"
        std::string chiave, valore;
        while (in >> chiave >> valore)             // due parole per riga
        {
            par[chiave] = valore;
        }
        std::string metodo = par.at("metodo");
```

**SOSTITUISCI in app.cpp** → `double dt = 0.001;`
```cpp
        double dt = std::stod(par.at("dt"));       // stod: testo -> double
```

**SOSTITUISCI in app.cpp** → `int N = 1000;`
```cpp
        int N = std::stoi(par.at("N"));            // stoi: testo -> int
```

**SOSTITUISCI in app.cpp** → `double omega = 1.0, x0 = 1.0, v0 = 0.0;`
```cpp
        double omega = std::stod(par.at("omega"));
        double x0 = std::stod(par.at("x0"));
        double v0 = std::stod(par.at("v0"));
```

**NEL app.cpp**
```cpp
        // Rk: parametri letti
        std::cout << "da params.ini: metodo " << metodo << ", dt = " << dt << ", N = " << N << "\n\n";
```

> `params.ini` va **nello zip**, nella stessa cartella del programma.

---

## S3 · CONFRONTO TRA INTEGRATORI
**Il testo dice:** "compare the two integrators", "one output file per integrator"
**Scheletro:** H
**Richiede:** S1

Un ciclo sugli integratori **tramite la base**: la stessa riga di codice usa algoritmi diversi.

**NEL app.cpp**
```cpp
        // Rk: confronto, un file per integratore
        std::cout << "=== Confronto ===\n";
        Euler e1;
        RungeKutta2 r1;
        const Integratore* tutti[] = {&e1, &r1};   // array di puntatori alla base
        double esatto = x0 * std::cos(omega * N * dt);
        for (const Integratore* integ : tutti)
        {
            Stato f = Simulazione(sistema, *integ).esegui(u0, dt, N, "sim_" + integ->nome() + ".dat");
            std::cout << integ->nome() << ": x(T) = " << f[0] << ", errore = " << std::fabs(f[0] - esatto) << "\n";
        }
        std::cout << "(RK2 deve avere l'errore piu' piccolo)\n\n";
```

---

## S4 · UN ALTRO SISTEMA FISICO (es. orbita, satellite)
**Il testo dice:** "a satellite in the gravitational field of the Earth", "equations of motion ..."
**Scheletro:** H

Una nuova derivata di `Sistema`: cambiano **solo** lo stato e `derivate`. Esempio: gravità in 2D, `u = (x, y, vx, vy)`, `a = -GM r / |r|^3`.

**NUOVO FILE Gravita.h**
```cpp
// =====================================================================
// Gravita.h  -  moto in 2D nel campo di una massa M nell'origine
// u = (x, y, vx, vy):   dx/dt = vx,  dy/dt = vy,  dv/dt = -GM r / |r|^3
// =====================================================================

#ifndef GRAVITA_H
#define GRAVITA_H

#include "Integratore.h"

class Gravita : public Sistema
{
public:
    // GM [m^3/s^2] > 0
    explicit Gravita(double GM);

    Stato derivate(double t, const Stato& u) const override;

private:
    double GM_;
};

#endif
```

**NUOVO FILE Gravita.cc**
```cpp
// =====================================================================
// Gravita.cc  -  IMPLEMENTAZIONE di Gravita
// =====================================================================

#include "Gravita.h"

#include <stdexcept>
#include <cmath>

Gravita::Gravita(double GM)
    : GM_(GM)
{
    if (GM_ <= 0.0)
    {
        throw std::invalid_argument("GM deve essere > 0");
    }
}

Stato Gravita::derivate(double /*t*/, const Stato& u) const
{
    double x = u[0], y = u[1], vx = u[2], vy = u[3];   // nomi leggibili
    double r = std::sqrt(x * x + y * y);
    if (r == 0.0)
    {
        throw std::domain_error("r = 0: il corpo e' nel centro");
    }
    double k = -GM_ / (r * r * r);
    return {vx, vy, k * x, k * y};                     // (dx/dt, dy/dt, dvx/dt, dvy/dt)
}
```

**NEL app.cpp** → INCLUDE
```cpp
#include "Gravita.h"
```

**NEL app.cpp**
```cpp
        // Rk: orbita circolare (GM = 1, r = 1, v = 1): dopo un periodo T = 2 pi torna al punto di partenza
        Gravita terra(1.0);
        const double PI = std::acos(-1.0);
        int passi = 10000;
        Stato fineOrbita = Simulazione(terra, *scelto).esegui({1.0, 0.0, 0.0, 1.0}, 2.0 * PI / passi, passi, "orbita.dat");
        double rFin = std::sqrt(fineOrbita[0] * fineOrbita[0] + fineOrbita[1] * fineOrbita[1]);
        std::cout << "raggio dopo un giro: " << rFin << "   (atteso circa 1; con Euler cresce un po')\n\n";
```

> Compilazione: aggiungi `Gravita.cc` al comando.

---

## S5 · CONTROLLO: ENERGIA CONSERVATA
**Il testo dice:** niente, ma è il controllo fisico più forte (controllo n. 3): in un sistema conservativo l'energia all'inizio e alla fine deve essere quasi uguale.
**Scheletro:** H

**NEL app.cpp** → FUNZIONI DEL MAIN
```cpp
// Energia per unita' di massa dell'oscillatore: E = v^2/2 + omega^2 x^2/2
double energia(const Stato& u, double omega)
{
    return 0.5 * u[1] * u[1] + 0.5 * omega * omega * u[0] * u[0];
}
```

**NEL app.cpp**
```cpp
        // Rk: conservazione dell'energia
        std::cout << "energia iniziale = " << energia(u0, omega) << ", finale = " << energia(fine, omega)
                  << "   (quasi uguali; con Euler cresce leggermente)\n\n";
```
