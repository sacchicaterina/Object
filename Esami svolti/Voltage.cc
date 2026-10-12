#include "Voltage.h"
#include <cmath>
#include <fstream>  // per std::ofstream
#include <iostream>  // per std::cerr

Voltage::Voltage(Mesh g, Source ch) : grid(g), charges(ch) { computePotential(); }

void Voltage::computePotential() {
  // Dichiara 5 variabili temporanee, che verranno riusate a ogni
  // iterazione dei cicli sottostanti per fare i calcoli intermedi.
  double diff_x, diff_y, diff_z, dist_j, charge;

  // Prealloca memoria nel vettore potentials, per contenere quanti
  // elementi ci sono in grid (uno per ogni punto dove calcolerai V).
  potentials.reserve(grid.gridSize());

  // CICLO ESTERNO: scorre su ogni punto j della mesh "grid"
  // (i punti dove vuoi calcolare il potenziale).
  for (size_t j = 0; j < grid.gridSize(); j++) {

    // Variabile che accumulerà il potenziale per QUESTO punto j.
    // Riparte da 0 a ogni nuova iterazione del ciclo esterno,
    // perché ogni punto ha il suo proprio V_j da calcolare da zero.
    double V_j = 0.;

    // CICLO INTERNO: scorre su ogni carica i dentro "charges"
    // (tutte le cariche che contribuiscono al potenziale nel punto j).
    for (size_t i = 0; i < charges.sourceSize(); i++) {

      // Calcola la differenza tra la coordinata x della carica i-esima
      // (presa dalla mesh dentro charges) e la coordinata x del punto j
      // (presa da grid). Stessa cosa per y e z nelle due righe seguenti.
      diff_x = charges.sourceGrid().x(i) - grid.x(j);
      diff_y = charges.sourceGrid().y(i) - grid.y(j);
      diff_z = charges.sourceGrid().z(i) - grid.z(j);

      // Calcola la distanza euclidea tra la carica i e il punto j,
      // usando il teorema di Pitagora in 3D (radice della somma dei
      // quadrati delle tre differenze appena calcolate).
      dist_j = std::sqrt(std::pow(diff_x, 2) + std::pow(diff_y, 2) + std::pow(diff_z, 2));

      // Prende il valore numerico (q_i) della carica i-esima.
      charge = charges.value(i);

      // Applica la formula V_j += q_i / distanza, e ACCUMULA questo
      // contributo su V_j (che alla fine del ciclo interno conterrà
      // la somma di TUTTI i contributi di TUTTE le cariche).
      V_j += charge / dist_j;
    }
    // Il ciclo interno è finito: V_j ora contiene il valore finale
    // del potenziale per il punto j (somma di tutti i contributi).
    // Lo aggiungi al vettore potentials, in coda.
    potentials.push_back(V_j);
  }
  // Il ciclo esterno è finito: potentials ora ha un valore V_j
  // per ciascuno dei punti di grid, nello stesso ordine.
}

const Mesh& Voltage::voltageGrid() const { return grid; }
size_t Voltage::potentialSize() const { return potentials.size(); }
double Voltage::potential(int j) const { return potentials.at(j); }
const std::vector<double>& Voltage::potentialsVec() const { return potentials; }

void Voltage::printToFile(const std::string& filename) const {
  std::ofstream out(filename);
  if (!out) {
    std::cerr << "printToFile: could not open file \"" << filename << "\" for writing.\n";
    return;
  }
  out << "# x y z V\n";
  for (size_t j = 0; j < grid.gridSize(); j++) {
    out << grid.x(j) << " "
        << grid.y(j) << " "
        << grid.z(j) << " "
        << potential(j) << "\n";
  }
  out.close();
}
