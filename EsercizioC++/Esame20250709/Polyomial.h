#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include "Function.h"
#include <iostream>
#include <string>
#include <vector>

class Polynomial : public Function {
 private:
  std::vector<double> coeffs_;
  int degree_; // <--- Aggiungi questo membro dato

 public:
  //costruttore
  Polynomial(const std::string& name, const std::vector<double>& coefficienti);

  //getter e setter
  std::vector<double> getCoeff() const;
  void setCoeff(const std::vector<double>& nuovi_coeff);

  double value(double x) const override;
  double primitive(double x) const;
  double deriv(double x) const override;

  void print(double x) const;

  ~Polynomial() override;

};
#endif
