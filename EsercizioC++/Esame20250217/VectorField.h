#ifndef VECTORFIELD_H
#define VECTORFIELD_H

#include "Function.h"
#include <vector>
#include <iostream>
#include <string>

// questa non e' una classe derivata
class VectorField {
 private:
  // Tre puntatori alle componenti del campo
  // Nota: usiamo la classe base Function per il polimorfismo
  const Function* f_x_;
  const Function* f_y_;
  const Function* f_z_;

 public:
  //costruttore: riceve i 3 putatori alla funzioni componenti
  VectorField(const Function* fx, const Function* fy, const Function* fz);

  // Valutazione del campo in un punto (restituisce un vettore di 3 double)
  std::vector<double> value(double x, double y, double z) const;

  double divergence(double x, double y, double z) const;
  std::vector<double> curl(double x, double y, double z) const;

  void printVector(double x, double y, double z)const;

  ~VectorField();

};
#endif
