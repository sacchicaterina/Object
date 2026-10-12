"""
Computing Methods for Physics 1 - Exam 11/09/2024 - Part 2 (Python)

Class Frac: represents a fraction n/d.

Sign convention: same as the C++ part -> the sign is always carried by
the numerator, the denominator is kept strictly positive.
"""

from math import gcd


class Frac:
    """A simple fraction n/d, always kept in lowest terms."""

    def __init__(self, n, d=1):
        """Constructor: denominator defaults to 1 if not provided."""
        if d == 0:
            raise ValueError("Frac: denominator cannot be zero.")
        self._n = n
        self._d = d
        self.simplify()

    # --- Getters / setters as properties (Pythonic getters/setters) ---

    @property
    def n(self):
        """Numerator getter."""
        return self._n

    @n.setter
    def n(self, value):
        """Numerator setter."""
        self._n = value
        self.simplify()

    @property
    def d(self):
        """Denominator getter."""
        return self._d

    @d.setter
    def d(self, value):
        """Denominator setter."""
        if value == 0:
            raise ValueError("Frac: denominator cannot be zero.")
        self._d = value
        self.simplify()

    def inverse(self):
        """Return a new Frac representing the inverse of this fraction."""
        if self._n == 0:
            raise ValueError("Frac: cannot invert a fraction with numerator 0.")
        return Frac(self._d, self._n)

    def simplify(self):
        """Reduce the fraction to lowest terms and normalize the sign
        (sign on the numerator, denominator kept positive)."""
        if self._d < 0:
            self._n = -self._n
            self._d = -self._d

        if self._n == 0:
            # So we normalize: zero is always stored as 0/1.
            self._d = 1
            return

        g = gcd(self._n, self._d)
        if g:
            # Using // (floor/integer division) instead of / here is
            # essential: n and d must stay integers, since a fraction is
            # only meaningful with an integer numerator and denominator.
            # `/` in Python always returns a float (e.g. 6 / 2 -> 3.0),
            # even when the division is exact, which would silently turn
            # self._n / self._d into floats and break the class (a Frac
            # is supposed to store two ints, not two floats).
            # `//` instead performs integer division and returns an int
            # when both operands are ints (6 // 2 -> 3). Since g is the
            # gcd of self._n and self._d, it always divides them exactly,
            # so // here is not "rounding down" anything - it's just the
            # correct, type-safe way to divide two integers by their gcd.
            self._n //= g
            self._d //= g

    # --- Operator overloads ---
    #
    # What are "dunder methods"?
    # "Dunder" is short for "double underscore": methods whose name starts
    # and ends with two underscores, like __init__ or __add__. Python
    # reserves these names for a specific purpose: they let a class hook
    # into the language's built-in syntax and behavior.
    #
    # When you write `a + b` for two Frac objects, Python does NOT know
    # natively how to add two fractions - it simply looks for a method
    # called __add__ on the object and calls a.__add__(b). If __add__ is
    # not defined, `+` raises a TypeError. The same idea applies to every
    # dunder used below:
    #   a + b        -> a.__add__(b)
    #   a - b        -> a.__sub__(b)
    #   a * b        -> a.__mul__(b)
    #   a / b        -> a.__truediv__(b)
    #   str(a) / print(a) -> a.__str__()
    #   repr(a)      -> a.__repr__()
    #   Frac(...)    -> __init__ is called automatically on construction
    #
    # This mechanism is called "operator overloading": giving a new,
    # custom meaning to an operator that already exists in the language,
    # for a type that you defined yourself. It is the direct Python
    # equivalent of overloading operator+, operator-, etc. in the C++
    # part of this exam - only the naming convention differs (__add__
    # instead of operator+).

    def __add__(self, other):
        return Frac(self._n * other.d + other.n * self._d, self._d * other.d)

    def __sub__(self, other):
        return Frac(self._n * other.d - other.n * self._d, self._d * other.d)

    def __mul__(self, other):
        return Frac(self._n * other.n, self._d * other.d)

    def __truediv__(self, other):
        if other.n == 0:
            raise ZeroDivisionError("Frac: division by a fraction with numerator 0.")
        return Frac(self._n * other.d, self._d * other.n)

    def __str__(self):
        return f"{self._n}/{self._d}"

    def __repr__(self):
        return f"Frac({self._n}, {self._d})"


if __name__ == "__main__":
    a = Frac(3, 4)
    b = Frac(-2, 6)      # will simplify to -1/3
    c = Frac(5)          # denominator defaults to 1 -> 5/1

    print(f"a = {a}")
    print(f"b = {b}")
    print(f"c = {c}")

    print(f"a + b = {a + b}")
    print(f"a - b = {a - b}")
    print(f"a * b = {a * b}")
    print(f"a / b = {a / b}")

    print(f"inverse of a = {a.inverse()}")

    a.n = 10
    a.d = 4
    print(f"a after setters (10/4 simplified) = {a}")
