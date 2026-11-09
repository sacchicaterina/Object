// =====================================================================
// Function.h  -  CLASSE BASE DATA DAL TESTO                          [GRUPPO G]
// Esempi: Function 1D (07/2025), Function 3D con deriv(x, y, z, var) (02/2025).
//
// >>> INCOLLA QUI LA CLASSE DEL TESTO, IDENTICA (nomi, firme, const). <<<
//     Sotto c'e' quella del 07/2025 come esempio.
// Non si cambia nulla. Unica aggiunta permessa (e consigliata): il distruttore
// virtuale, se manca, DICHIARANDOLO nel commento -> CATALOGO G1.
// =====================================================================

#ifndef FUNCTION_H
#define FUNCTION_H

#include <string>

class Function {
 public:
  Function(const std::string& name) { name_ = name; };
  // Value of the function at a point along the x-axis
  virtual double value(double x) const = 0;
  // Derivative of the function calculated at a point along the x-axis
  virtual double deriv(double x) const = 0;
  virtual std::string name() const { return name_; };
 private:
  std::string name_;
};

#endif
