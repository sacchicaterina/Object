#ifndef RESiSTOR_H
#define RESiSTOR_H

#include "CircuitElement.h"

class Resistor : public CircuitElement {
 private:
  double Resistance_;
  static int count_;

 public:

  //constructor
  Resistor(double R);

  //getters
  double Rvalue() const;

  //setters
  void setRvalue(double Rvalue);

  //per contare
  static int getCount() { return count_; }


  // 3. Override dei metodi virtuali di CircuitElement
  double getValue() const override;
  void print() const override;

  // 2. Overload Operatori (richiesti dalla traccia)
  Resistor operator+(const Resistor& other) const;  // Serie
  Resistor operator||(const Resistor& other) const; // Parallelo

  Resistor(const Resistor& other); //serve per conta ache valore uguali
  ~Resistor() override;

};

// 4. Overload operatore << (globale)
std::ostream& operator<<(std::ostream& os, const Resistor& r);

#endif
