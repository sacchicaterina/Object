#ifndef MASSA_H
#define MASSA_H

#include <iostream>

class massa{
 private:
  double X1x_;
  double X1y_;
  double X1z_;

  double X2x_;
  double X2y_;
  double X2z_;

  double m1_;
  double m2_;

  double Mc_;
  double v_;

  static const double M_solare=1.0;

 public:

  //constructor vuoto per i valori in condizione di default
  massa();

  //constructor
  massa(double X1x, double X1y, double X1z, double X2x, double X2y, double X2z, double Mc, double v);

  //getters 
  double Mcvalue() const;
  double vvalue() const;
  double X1xvalue() const;
  double X1yvalue() const;
  double X1zvalue() const;
  double X2xvalue() const;
  double X2yvalue() const;
  double X2zvalue() const;
  double m1value() const;
  double m2value() const;

  //setters
  void set_Mc(double Mcvalue);
  void set_v(double vvalue);
  void set_X1x(double X1xvalue);
  void set_X1y(double X1yvalue);
  void set_X1z(double X1zvalue);
  void set_X2x(double X2xvalue);
  void set_X2y(double X2yvalue);
  void set_X2z(double X2zvalue);
  void set_m1(double m1value);
  void set_m2(double m2value);
  
  // Metodi di calcolo parametri
  double calcolo_mass_ratio() const; // q = m2/m1

  double calcolo_spin_effettivo() const; // chi_eff

  double calcolo_spin_precession() const; // chi_p

  //output
  void print() const;
  friend std::ostream& operator<<(std::ostream& os, const massa& obj);

};
#endif
