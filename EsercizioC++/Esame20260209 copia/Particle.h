#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vector.h"

#include <iostream>

class Particle{
 private:
  double energy_;
  Vector momentum_;

 public:

  //costruttori
  Particle(double energy, double px, double py, double pz);

  Particle(double energy, const Vector& p);

  //getters e setters
  double get_energy() const;
  Vector get_momentum() const;

  void set_energy(double energyvalue);
  void set_momentum(const Vector& p);

};

//iserisco fuori la classe un metodo per trovare la massa invariante di un sistema a due particelle
double calculate_invariant_mass(const Particle& A, const Particle& B);

#endif
