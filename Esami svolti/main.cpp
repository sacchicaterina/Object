#include <iostream>
#include "Mesh.h"
#include "Source.h"
#include "Voltage.h"

int main() {
  // Mesh regolare 3x3x3=27 punti: dove calcolerò il potenziale
  Mesh fieldGrid = Mesh::Regular(3, 2.0);
  std::cout << "Mesh regolare creata con " << fieldGrid.gridSize() << " punti.\n";

  // Mesh casuale, 5 punti sparsi in un cubo di lato 2 (solo dimostrativa)
  Mesh randomGrid = Mesh::Random(5, 2.0);
  std::cout << "Mesh random creata con " << randomGrid.gridSize() << " punti.\n";

  // Mesh interattiva, 2 punti: posizioni delle cariche
  Mesh chargeGrid(2);
  std::cout << "Mesh interattiva creata con " << chargeGrid.gridSize() << " punti.\n";

  // Chiede il valore di ciascuna delle 2 cariche
  Source charges(chargeGrid, "scalar");
  std::cout << "Source creato con " << charges.sourceSize() << " cariche.\n";

  // Calcola il potenziale di "charges" su tutti i punti di "fieldGrid"
  Voltage V(fieldGrid, charges);
  std::cout << "Voltage calcolato su " << V.potentialSize() << " punti.\n";

  // Scrive il risultato su file
  V.printToFile("voltage.txt");

  std::cout << "Potenziale salvato in voltage.txt\n";

  return 0;
}
