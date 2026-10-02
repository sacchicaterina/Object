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

    Mass[i] = Particle(energy_sum, momentum_sum);

    cout << "Evento " << i + 1 << ":" << endl;
    cout << "  > Energia Totale (E_A + E_B):  " << energy_sum << " GeV" << endl;
    cout << "  > Massa Invariante Calcolata:   " << m << " GeV/c^2" << endl;
    cout << "----------------------------------------------------------------" << endl;
    }

  cout << "TEST 2: VIOLAZIONE NEL COSTRUTTORE" << endl;
  Particle p_invalid(1.0, 10.0, 10.0, 10.0);
  cout << "Particella creata con E=1, p=10. Risultato -> E: " << p_invalid.get_energy() << " GeV" << endl;

  cout << "TEST 3: VIOLAZIONE TRAMITE SETTER " << endl;
  Particle p_setter(50.0, 1.0, 1.0, 1.0);
  cout << "Particella iniziale valida: E = " << p_setter.get_energy() << endl;
  cout << "Tentativo: set_energy(0.1)..." << endl;
  p_setter.set_energy(0.1);
  
  cout << "FINE SIMULAZIONE" << endl; 
  
  return 0;
}
