#include "Vector.h"

#include <iostream>
#include <cmath>

using namespace std;

//costruttori
Vector::Vector() : px_(0.0), py_(0.0), pz_(0.0) {}
Vector::Vector(double pxval, double pyval, double pzval) {
  px_ = pxval;
  py_ = pyval;
  pz_ = pzval;
}

//setters e getters

double Vector::get_px() const {return px_;}
double Vector::get_py() const {return py_;}
double Vector::get_pz() const {return pz_;}

void Vector::set_px(double pxvalue) {px_ = pxvalue;}
void Vector::set_py(double pyvalue) {py_ = pyvalue;}
void Vector::set_pz(double pzvalue) {pz_ = pzvalue;}

//implementazione per calcolare il modulo del vettore
double Vector::norm() const {
  return sqrt(px_*px_ + py_*py_ + pz_*pz_);
}

//eseguo la somma tra vettori, somma in componenti 
Vector Vector::operator+(const Vector& other) const {
    return Vector(px_ + other.px_, 
                  py_ + other.py_, 
                  pz_ + other.pz_);
}


    
