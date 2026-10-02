#include "Particle.h"
#include <cmath>
#include <iostream>

using namespace std;

//costruttore
Particle::Particle(double energy, double px, double py, double pz) {
  energy_ = energy;
  momentum_  = Vector(px,py,pz);

  //imposto la condizione E > ||p||
  if (energy_  < momentum_.norm())
    {
      cerr << "Attenzione valore inseriti non accettabili, violazione fisica (E < ||p||)" << endl;

      energy_ = 0;
      momentum_ = Vector(0,0,0);
    }
  else {
    cout << "Rispettata richiesta fisica (E > ||P||)" << endl;
  }
}

Particle::Particle(double energy, const Vector& p) {
  energy_ = energy;
  momentum_ = Vector(p);

  //imposto la condizione E > ||p||
  if (energy_ < momentum_.norm())
    {
      cerr << "Attenzione valore inseriti non accettabili, violazione fisica (E < ||p||)" << endl;

      energy_ =	0;
      momentum_	= Vector(0,0,0);
    }
  else {
    cout << "Rispettata richiesta fisica (E > ||P||)" << endl;
  }
}

//getters e setters
double Particle::get_energy() const { return energy_;}
Vector Particle::get_momentum() const { return momentum_;}

void Particle::set_energy(double energyvalue) {
  if (energyvalue < momentum_.norm()) {
        cerr << "Attenzione: valore energia non accettabile, violazione fisica (E < ||p||)" << endl;
        energy_ = 0;
        momentum_ = Vector(0, 0, 0);
    } else {
        energy_ = energyvalue;
        cout << "Energia aggiornata: rispettata richiesta fisica (E > ||P||)" << endl;
    }
}

void Particle::set_momentum(const Vector& p) {
  if (energy_ < p.norm()) {
        cerr << "Attenzione: valore momento non accettabile, violazione fisica (E < ||p||)" << endl;
        energy_ = 0;
        momentum_ = Vector(0, 0, 0);
    } else {
        momentum_ = p;
        cout << "Momento aggiornato: rispettata richiesta fisica (E > ||P||)" << endl;
    }
}

//eseguoi il calcolo della massa invariante
double calculate_invariant_mass(const Particle& A, const Particle& B) {
  double total_energy = A.get_energy() + B.get_energy();
  Vector total_momentum = A.get_momentum() + B.get_momentum();

  double Invariant_mass = sqrt ( pow(total_energy,2) - pow(total_momentum.norm(),2));

  return Invariant_mass;
}
