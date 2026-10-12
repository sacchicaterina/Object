// Frac.cpp
// Computing Methods for Physics 1 - Exam 11/09/2024 - Part 1 (C++)
//
// Class Frac: represents a fraction n/d.
//
// Sign convention: the sign of the fraction is always carried by the
// numerator (n). The denominator (d) is kept strictly positive.
// This choice is enforced inside simplify(), which is called after
// every construction/modification that could break the convention.

#include <iostream>
#include <numeric>   // std::gcd
#include <stdexcept> // std::invalid_argument

class Frac {
private:
    int n; // numerator
    int d; // denominator

public:
    // 1. Regular constructor
    Frac(int num, int den) : n(num), d(den) {
        if (d == 0) {
            throw std::invalid_argument("Frac: denominator cannot be zero.");
        }
        simplify();
    }

    // 2. Default constructor: asks the user for numerator and denominator
    Frac() {
        int num, den;
        std::cout << "Enter numerator: ";
        std::cin >> num;
        do {
            std::cout << "Enter denominator (non-zero): ";
            std::cin >> den;
        } while (den == 0);

        n = num;
        d = den;
        simplify();
    }

    // 3. Copy constructor
    Frac(const Frac& other) : n(other.n), d(other.d) {
        // already simplified/normalized, nothing more to do
    }

    // 4. Constructor from a single integer (denominator assumed to be 1)
    Frac(int num) : n(num), d(1) {
        // already simplified
    }

    // --- Getters ---
    int getNumerator() const { return n; }
    int getDenominator() const { return d; }

    // --- Setters ---
    void setNumerator(int num) { n = num; simplify(); }

    void setDenominator(int den) {
        if (den == 0) {
            throw std::invalid_argument("Frac: denominator cannot be zero.");
        }
        d = den;
        simplify();
    }

    // Returns the inverse of the fraction (does not modify *this)
    Frac inverse() const {
        if (n == 0) {
            throw std::invalid_argument("Frac: cannot invert a fraction with numerator 0.");
        }
        return Frac(d, n);
    }

    // Simplifies the fraction in place: reduces n/d to lowest terms and
    // enforces the sign convention (sign on the numerator, d > 0).
    void simplify() {
        if (d < 0) {
            n = -n;
            d = -d;
        }
        if (n == 0) {
            // Without this, two mathematically identical fractions like
            // 0/5 and 0/17 would end up with different internal
            // representations, breaking comparisons, printing, and
            // subsequent calculations.
            d = 1;
            return;
        }
        int g = std::gcd(n, d);
        if (g != 0) {
            n /= g;
            d /= g;
        }
    }

    // --- Operator overloads ---

    Frac operator+(const Frac& other) const {
        return Frac(n * other.d + other.n * d, d * other.d);
    }

    Frac operator-(const Frac& other) const {
        return Frac(n * other.d - other.n * d, d * other.d);
    }

    Frac operator*(const Frac& other) const {
        return Frac(n * other.n, d * other.d);
    }

    Frac operator/(const Frac& other) const {
        if (other.n == 0) {
            throw std::invalid_argument("Frac: division by a fraction with numerator 0.");
        }
        return Frac(n * other.d, d * other.n);
    }

    // Stream insertion operator: must be a free function (friend) since the
    // left-hand operand is an ostream, not a Frac.
    friend std::ostream& operator<<(std::ostream& os, const Frac& f);
};

std::ostream& operator<<(std::ostream& os, const Frac& f) {
    os << f.n << "/" << f.d;
    return os;
}

// --- Demo / manual test of the class ---
int main() {
    Frac a(3, 4);          // regular constructor
    Frac b(-2, 6);         // regular constructor, will simplify to -1/3
    Frac c(5);             // constructor from a single integer -> 5/1
    Frac d_copy(a);        // copy constructor

    std::cout << "a = " << a << '\n';
    std::cout << "b = " << b << '\n';
    std::cout << "c = " << c << '\n';
    std::cout << "copy of a = " << d_copy << '\n';

    std::cout << "a + b = " << (a + b) << '\n';
    std::cout << "a - b = " << (a - b) << '\n';
    std::cout << "a * b = " << (a * b) << '\n';
    std::cout << "a / b = " << (a / b) << '\n';

    std::cout << "inverse of a = " << a.inverse() << '\n';

    a.setNumerator(10);
    a.setDenominator(4);
    std::cout << "a after setters (10/4 simplified) = " << a << '\n';

    // Uncomment to test the interactive default constructor:
    // Frac user_frac;
    // std::cout << "your fraction: " << user_frac << '\n';

}
