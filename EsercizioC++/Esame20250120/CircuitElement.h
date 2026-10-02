#ifndef CIRCUITELEMENT_H
#define CIRCUITELEMENT_H

class CircuitElement {
 public:

  virtual double getValue() const = 0; // Restituisce R, L o C a seconda della classe

  //distruttore
  virtual ~CircuitElement() {}

  virtual void print() const = 0;

};

#endif
