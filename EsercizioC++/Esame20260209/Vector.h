#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>

class Vector{
 private:
  double px_;
  double py_;
  double pz_;

 public:
  //costruttore di default
  Vector();

  //costruttore
  Vector(double pxval, double pyval, double pzval);

  //getters e setters

  double get_px() const;
  double get_py() const;
  double get_pz() const;

  void set_px(double pxvalue);
  void set_py(double pyvalue);
  void set_pz(double pzvalue);

  //metodo per calolare il modulo
  double norm() const;

  //metodo per eseguire la somma tra vettori
  Vector operator+(const Vector& other) const;

};
#endif
