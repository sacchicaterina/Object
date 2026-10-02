#include <iostream>
#include <vector>

#include "Particle.h"
#include "Vector.h"

using namespace std;

int main() {

  //definiamo std::vector                                                                          
  Particle p_default(0,0,0,0);
  vector<Particle> A(10, p_default);
  vector<Particle> B(10, p_default);
  vector<Particle> Mass(10, p_default);

  for (int i = 0; i < 10; i++)
    {
      A[i].set_energy(15.0 + i);
      A[i].set_momentum(Vector(2.0, 1.0,3.0));

      B[i].set_energy(20.0 + i);
      B[i].set_momentum(Vector(-1.0, 0.0, 2.0));
    }

  cout << "Risultati Decadimenti" << endl;

  for(int i=0; i<10; i++) {

    double m = calculate_invariant_mass(A[i], B[i]);

    double energy_sum = A[i].get_energy() + B[i].get_energy();
    Vector momentum_sum = A[i].get_momentum() + B[i].get_momentum();

    Mass[i] = Particle(m, Vector(0,0,0));
  }

  cout << "\n--- RISULTATI MEMORIZZATI NEL VETTORE MASS ---" << endl;
  for(int i=0; i<10; i++) {
    cout << "Decadimento " << i + 1 << " - Massa Invariante: " 
         << Mass[i].get_energy() << " GeV/c^2" << endl;
  }
  return 0;
}
