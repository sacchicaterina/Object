#include "Triangle.h"
#include <iostream>
#include <cmath>

using namespace std;

//constructor
Triangle::Triangle(const std::string& lbl, double aval, double bval, double cval) :Shape( lbl) {
  a_ = aval;
  b_ = bval;
  c_ = cval;

  // Chiamiamo il tuo metodo per memorizzare gli angoli subito 
  get_angle();
}

double Triangle::avalue() const { return a_; }
double Triangle::bvalue() const { return b_; }
double Triangle::cvalue() const { return c_; }
void Triangle::set_a(double avalue) { a_ = avalue; get_angle(); }
void Triangle::set_b(double bvalue) { b_ = bvalue; get_angle(); }
void Triangle::set_c(double cvalue) { c_ = cvalue; get_angle(); }

// Implementazione del distruttore (necessaria se dichiarata nel .h)
Triangle::~Triangle() {}

void Triangle::get_angle() {
  gamma_ = acos((pow(a_,2) + pow(b_,2) - pow(c_,2) )/(2*a_*b_));
  beta_ = acos((pow(a_,2) + pow(c_,2) - pow(b_,2) )/(2*a_*c_));
  alpha_ = acos((pow(b_,2) + pow(c_,2) - pow(a_,2) )/(2*b_*c_));
}

double Triangle::get_perimeter() const {
  return a_ + b_ + c_;
}

void Triangle::print() const {
  cout << "figura geometrica: " << label << endl;
  cout <<"lato 1: " << a_ << " ; lato 2: "<< b_ << " ; lato 3: "<< c_ << endl;
  cout <<"alpha: " << alpha_ << " ; beta: "<< beta_ << " ; gamma: "<< gamma_ << endl;
  cout <<"perimetro: " << get_perimeter() << endl;
}

  
