#ifndef VOLTAGE_H
#define VOLTAGE_H

#include <string>
#include <vector>
#include "Mesh.h"
#include "Source.h"

// Rappresenta discretamente il potenziale elettrostatico V generato da un
// insieme di cariche puntiformi (Source), calcolato su ogni punto di una
// mesh separata (grid).
class Voltage {

private:

  Mesh grid;
  Source charges;
  std::vector<double> potentials;

  void computePotential();

public:

  Voltage(Mesh g, Source ch);

  const Mesh& voltageGrid() const;   // punti della mesh dove ho calcolato il potenziale
  size_t potentialSize() const;
  double potential(int j) const;
  const std::vector<double>& potentialsVec() const;   // getter per l'intero vettore dei potenziali

  void printToFile(const std::string& filename) const;
};

#endif // VOLTAGE_H
