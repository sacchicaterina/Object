// functions.cpp
//
// OOP for Data Processing - Compito del 9 luglio 2025 - Parte 1 (C++)
//
// Implementazione in un unico file (senza .h/.cc separati) che contiene:
//   - la classe astratta Function (data dal testo del compito)
//   - la classe concreta Polynomial (polinomi di grado qualsiasi)
//   - la classe concreta Rational   (rapporto di due Polynomial)
//   - una breve applicazione dimostrativa in main()
//
// Compilazione: g++ -std=c++17 -Wall -o functions functions.cpp
// Esecuzione:   ./functions

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <cmath>
#include <utility>

// ---------------------------------------------------------------------
// Classe base astratta (data dal testo del compito)
// ---------------------------------------------------------------------
class Function {
public:
    Function(const std::string& name) { name_ = name; };

    // Valore della funzione in un punto lungo l'asse x
    virtual double value(double x) const = 0;

    // Derivata della funzione calcolata in un punto lungo l'asse x
    virtual double deriv(double x) const = 0;

    virtual std::string name() const { return name_; };

    // Una classe base con funzioni virtuali usata in modo polimorfico
    // dovrebbe avere un distruttore virtuale.
    virtual ~Function() = default;

private:
    std::string name_;
};

// ---------------------------------------------------------------------
// Polynomial: a0 + a1*x + a2*x^2 + ... + an*x^n
// ---------------------------------------------------------------------
class Polynomial : public Function {
public:
    // Costruttore: prende il vettore dei coefficienti (a0, a1, a2, ...)
    Polynomial(const std::vector<double>& coeff, const std::string& name)
        : Function(name), coefficients(coeff) {
        // coefficients.size() restituisce un tipo senza segno (size_t);
        // faccio il cast esplicito a int perché degree è un int con segno
        degree = static_cast<int>(coefficients.size()) - 1;
    }

    // Getter per il grado
    int getDegree() const { return degree; }

    // Getter per i coefficienti
    std::vector<double> getCoefficients() const { return coefficients; }

    // Getter per un singolo coefficiente (usa .at(i), che lancia
    // un'eccezione std::out_of_range se i non è un indice valido)
    double getCoefficient(int i) const { return coefficients.at(i); }

    // Setter: sostituisce i coefficienti (e aggiorna il grado)
    void setCoefficients(const std::vector<double>& coeff) {
        coefficients = coeff;
        degree = static_cast<int>(coefficients.size()) - 1;
    }

    // Valore del polinomio in x (metodo di Horner).
    //
    // Esempio: coefficients = [1, 2, 3, 4] (P(x) = 1 + 2x + 3x^2 + 4x^3),
    // degree = 3, x = 2.
    //
    // Il metodo di Horner riscrive P(x) raccogliendo la x a fattor comune,
    // annidando le parentesi a partire dal coefficiente di grado più alto:
    //   P(x) = 1 + 2x + 3x^2 + 4x^3
    //        = x*(4x^2 + 3x + 2) + 1
    //        = x*(x*(4x + 3) + 2) + 1
    //        = x*(x*(x*(4) + 3) + 2) + 1
    //
    // Il ciclo va da i = degree fino a i = 0. Ogni iterazione esegue:
    //   result = result * x + coefficients[i]
    // che calcola l'espressione annidata qui sopra un livello alla volta,
    // partendo dalla parentesi più interna (il coefficiente di grado più
    // alto) e procedendo verso l'esterno:
    //
    //   i=3: result = 0  * 2 + coefficients[3] = 0  * 2 + 4 = 4
    //   i=2: result = 4  * 2 + coefficients[2] = 4  * 2 + 3 = 11
    //   i=1: result = 11 * 2 + coefficients[1] = 11 * 2 + 2 = 24
    //   i=0: result = 24 * 2 + coefficients[0] = 24 * 2 + 1 = 49
    //
    // result = 49, che corrisponde a 1 + 2*2 + 3*4 + 4*8 = 49.
    // Ad ogni passo prendo il valore ereditato dal passo precedente,
    // lo moltiplico per x e sommo il prossimo coefficiente (procedendo
    // dal coefficiente di grado più alto fino ad a0).
    double value(double x) const override {
        double result = 0.0;
        for (int i = degree; i >= 0; --i) {
            result = result * x + coefficients[i];
        }
        return result;
    }

    // Derivata del polinomio in x (metodo di Horner sulla derivata).
    //
    // Esempio: coefficients = [1, 2, 3, 4] (P(x) = 1 + 2x + 3x^2 + 4x^3),
    // degree = 3, x = 2.
    //
    // La derivata è P'(x) = 2 + 6x + 12x^2 (ogni coefficiente a_i, per
    // i >= 1, diventa i*a_i e il grado si abbassa di uno). Raccogliendo
    // la x a fattor comune, come fatto per value():
    //   P'(x) = 2 + 6x + 12x^2
    //         = x*(12x + 6) + 2
    //         = x*(x*(12) + 6) + 2
    //
    // Il ciclo va da i = degree fino a i = 1 (a0 non compare nella
    // derivata). Ogni iterazione esegue:
    //   result = result * x + i * coefficients[i]
    //
    //   i=3: result = 0 * 2 + 3*coefficients[3] = 0 * 2 + 3*4 = 12
    //   i=2: result = 12 * 2 + 2*coefficients[2] = 12 * 2 + 2*3 = 30
    //   i=1: result = 30 * 2 + 1*coefficients[1] = 30 * 2 + 1*2 = 62
    //
    // result = 62, che corrisponde a 2 + 6*2 + 12*4 = 2+12+48 = 62.
    double deriv(double x) const override {
        if (degree == 0) return 0.0;
        double result = 0.0;
        for (int i = degree; i >= 1; --i) {
            result = result * x + i * coefficients[i];
        }
        return result;
    }

    // Valore della primitiva (antiderivata) in x, con costante di
    // integrazione posta a zero.
    //
    // Esempio: coefficients = [1, 2, 3, 4] (P(x) = 1 + 2x + 3x^2 + 4x^3),
    // degree = 3, x = 2.
    //
    // La primitiva è:
    //   Integrale(P(x)) = a0*x + (a1/2)*x^2 + (a2/3)*x^3 + (a3/4)*x^4
    //                   = 1*x + 1*x^2 + 1*x^3 + 1*x^4
    // (qui a1/2 = 2/2 = 1, a2/3 = 3/3 = 1, a3/4 = 4/4 = 1, per come sono
    // stati scelti i coefficienti in questo esempio)
    //
    // A differenza di value() e deriv(), qui NON uso il metodo di Horner:
    // ogni termine ha un denominatore diverso (i+1), quindi non si presta
    // a un raccoglimento a fattor comune così semplice. Calcolo invece
    // ogni termine a_i * x^(i+1) / (i+1) direttamente e li sommo, questa
    // volta procedendo in avanti da i=0 a i=degree (dal termine costante
    // verso il termine di grado più alto, l'ordine "naturale" con cui si
    // scrive il polinomio):
    //
    //   i=0: result = 0  + coefficients[0]*x^1/1 = 0  + 1*2/1  = 2
    //   i=1: result = 2  + coefficients[1]*x^2/2 = 2  + 2*4/2  = 6
    //   i=2: result = 6  + coefficients[2]*x^3/3 = 6  + 3*8/3  = 14
    //   i=3: result = 14 + coefficients[3]*x^4/4 = 14 + 4*16/4 = 30
    //
    // result = 30, che corrisponde a 1*2 + 1*4 + 1*8 + 1*16 = 2+4+8+16 = 30.
    // Stesso risultato di prima: l'ordine in cui si sommano i termini non
    // cambia il totale, l'addizione è commutativa.
    double primitive(double x) const {
        double result = 0.0;
        for (int i = 0; i <= degree; ++i) {
            result += coefficients[i] * std::pow(x, i + 1) / (i + 1);
        }
        return result;
    }

private:
    std::vector<double> coefficients;
    int degree;
};

// ---------------------------------------------------------------------
// Rational: rapporto di due Polynomial, num(x) / den(x)
// ---------------------------------------------------------------------
class Rational : public Function {
public:
    Rational(const Polynomial& num, const Polynomial& den, const std::string& name)
        : Function(name), numerator(num), denominator(den) {}

    // Getter per il numeratore
    Polynomial getNumerator() const { return numerator; }

    // Getter per il denominatore
    Polynomial getDenominator() const { return denominator; }

    // Getter per l'intera frazione: restituisce numeratore e denominatore
    // insieme, come coppia (numeratore, denominatore)
    std::pair<Polynomial, Polynomial> getFraction() const {
        return std::make_pair(numerator, denominator);
    }

    // Setter per il numeratore
    void setNumerator(const Polynomial& num) { numerator = num; }

    // Setter per il denominatore
    void setDenominator(const Polynomial& den) { denominator = den; }

    // Setter per l'intera frazione: sostituisce numeratore e denominatore
    // insieme in un'unica chiamata
    void setFraction(const Polynomial& num, const Polynomial& den) {
        numerator = num;
        denominator = den;
    }

    // Valore in x: num(x) / den(x)
    double value(double x) const override {
        double d = denominator.value(x);
        if (d == 0.0) {
            throw std::domain_error("Rational::value - divisione per zero a x = "
                                     + std::to_string(x));
        }
        return numerator.value(x) / d;
    }

    // Derivata in x tramite la regola del quoziente:
    // (num/den)' = (num' * den - num * den') / den^2
    double deriv(double x) const override {
        double d = denominator.value(x);
        if (d == 0.0) {
            throw std::domain_error("Rational::deriv - divisione per zero a x = "
                                     + std::to_string(x));
        }
        double num_val   = numerator.value(x);
        double num_deriv = numerator.deriv(x);
        double den_deriv = denominator.deriv(x);
        return (num_deriv * d - num_val * den_deriv) / (d * d);
    }

private:
    Polynomial numerator;
    Polynomial denominator;
};

// ---------------------------------------------------------------------
// Applicazione dimostrativa
// ---------------------------------------------------------------------
int main() {
    // P(x) = 1 + 2x + 3x^2   (a0=1, a1=2, a2=3)
    Polynomial p({1.0, 2.0, 3.0}, "P");

    std::cout << "== Polynomial " << p.name() << "(x) = 1 + 2x + 3x^2 ==\n";
    std::cout << "degree      = " << p.getDegree() << "\n";
    for (double x : {0.0, 1.0, 2.0}) {
        std::cout << "value(" << x << ")     = " << p.value(x) << "\n";
        std::cout << "deriv(" << x << ")     = " << p.deriv(x) << "\n";
        std::cout << "primitive(" << x << ") = " << p.primitive(x) << "\n";
    }

    // Uso il setter per cambiare i coefficienti:
    // Q(x) = 2 - x + x^3
    p.setCoefficients({2.0, -1.0, 0.0, 1.0});
    std::cout << "\nDopo setCoefficients -> nuovo grado = " << p.getDegree() << "\n";
    std::cout << "value(2) = " << p.value(2.0) << "\n";

    // Funzione razionale: R(x) = (1 + 2x + 3x^2) / (2 - x)
    Polynomial num({1.0, 2.0, 3.0}, "Num");
    Polynomial den({2.0, -1.0}, "Den");
    Rational r(num, den, "R");

    std::cout << "\n== Rational " << r.name() << "(x) = (1 + 2x + 3x^2) / (2 - x) ==\n";
    for (double x : {0.0, 1.0, -1.0}) {
        std::cout << "value(" << x << ") = " << r.value(x) << "\n";
        std::cout << "deriv(" << x << ") = " << r.deriv(x) << "\n";
    }

    // Dimostro il polimorfismo: entrambe le classi in un vector di Function*
    //
    // Function e' una classe astratta (ha metodi virtuali puri), quindi non
    // si puo' mai creare un oggetto Function direttamente: serve un
    // puntatore Function* che punti a un oggetto di una classe derivata
    // concreta (Polynomial o Rational). Ci sono due modi per ottenere quel
    // puntatore:
    //
    // MODO 1 (quello usato qui sotto): prendere l'indirizzo di un oggetto
    // gia' esistente sullo stack, con &p e &r. p e r sono variabili
    // normali, la cui vita e' legata al blocco main(): nessuna allocazione
    // dinamica, nessun new/delete da bilanciare, nessun rischio di memory
    // leak. E' la scelta giusta quando l'oggetto non deve sopravvivere
    // oltre lo scope in cui e' creato, come in questo programma.
    //
    // MODO 2 (alternativa, NON usata qui): allocazione dinamica con new,
    // ad esempio:
    //   functions.push_back(new Polynomial({1.0, 2.0, 3.0}, "P"));
    //   functions.push_back(new Rational(num, den, "R"));
    // In questo caso gli oggetti verrebbero creati sullo heap, e sarebbe
    // responsabilita' del programmatore fare delete su ciascuno di essi
    // (o usare uno smart pointer come std::unique_ptr) per evitare memory
    // leak. Serve solo quando l'oggetto deve vivere oltre lo scope in cui
    // e' creato, cosa che qui non ci serve.
    std::vector<Function*> functions;
    functions.push_back(&p);
    functions.push_back(&r);

    std::cout << "\n== Accesso polimorfico tramite Function* ==\n";
    for (const Function* f : functions) {
        std::cout << f->name() << "(1.5) = " << f->value(1.5)
                   << ", deriv = " << f->deriv(1.5) << "\n";
    }

    // Esempio dell'eccezione lanciata (divisione per zero)
    try {
        std::cout << "\nr.value(2) (den(2)=0) -> ";
        std::cout << r.value(2.0) << "\n";
    } catch (const std::domain_error& e) {
        std::cout << "eccezione catturata: " << e.what() << "\n";
    }

}
