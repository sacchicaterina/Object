#include <iostream>
#include "Particle.h"
#include "Barrier.h"

using namespace std;

int main() {
  //creiamo la particella (elettrone)
  // m = 0.511 MeV, z = 1, p = 5.5 MeV/c

  Particle electron(0.511, 1.0, 5.5);

  // 2. Creiamo la barriera (Carbonio)
  // A = 12.011, Z = 6.0, rho = 2.265, I = 81.0 eV
  Barrier carbon(12.011, 6.0, 2.265, 81.0);

  // 3. Verifichiamo i dati della particella
  cout << "--- TEST PARTICELLA ---" << endl;
  electron.print();
  cout << endl;

  // 4. Calcoliamo e stampiamo il potere d'arresto
  // Passiamo l'elettrone alla funzione print della barriera
  carbon.print(electron);

  return 0;
}
    
