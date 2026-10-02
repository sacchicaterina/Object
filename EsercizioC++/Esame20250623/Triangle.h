#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Shape.h"

class Triangle : public Shape {
 private:
  double a_;
  double b_;
  double c_;

  //angoli da memorizzare
  double alpha_;
  double beta_;
  double gamma_;

 public:
  //constructor, deve accettare anche la label per la classe base
  Triangle(const std::string& lbl, double aval, double bval, double cval);

  //getters
  double avalue() const;
  double bvalue() const;
  double cvalue() const;

  //setters
  void set_a(double avalue);
  void set_b(double bvalue);
  void set_c(double cvalue);

  //metodo calcolo angoli
  void get_angle();

  //metodo perimetro
  double get_perimeter() const;

  void print() const override;

  ~Triangle() override;

};
#endif
