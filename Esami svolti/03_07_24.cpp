#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <cmath>
#include <stdexcept>
#include <cstdlib>

class Mesh {

private:

  std::vector<double> coords_x;
  std::vector<double> coords_y;
  std::vector<double> coords_z;

  // Costruttore privato: controlli comuni su N e L
  Mesh(int N, double L) {
    if (N <= 0) {
      throw std::invalid_argument("N must be positive.");
    }
    if (L <= 0.0) {
      throw std::invalid_argument("L must be positive.");
    }
  }

  void buildRegular(int N, double L) {
    int points = N * N * N;
    coords_x.reserve(points);
    coords_y.reserve(points);
    coords_z.reserve(points);

    std::vector<double> values;
    values.reserve(N);
    for (int i = 0; i < N; i++) {
      double v = (N == 1) ? 0.0 : -L / 2.0 + i * (L / (N - 1));
      values.push_back(v);
    }
    for (int i = 0; i < N; ++i) {
      for (int j = 0; j < N; ++j) {
        for (int k = 0; k < N; ++k) {
          coords_x.push_back(values[i]);
          coords_y.push_back(values[j]);
          coords_z.push_back(values[k]);
        }
      }
    }
  }

  void buildRandom(int N, double L) {
    coords_x.reserve(N);
    coords_y.reserve(N);
    coords_z.reserve(N);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(-L / 2.0, L / 2.0);
    for (int i = 0; i < N; i++) {
      coords_x.push_back(dist(gen));
      coords_y.push_back(dist(gen));
      coords_z.push_back(dist(gen));
    }
  }

public:

  static Mesh Regular(int N, double L) {
    Mesh m(N, L);
    m.buildRegular(N, L);
    std::cout << "[Mesh] Griglia Regolare creata con successo (" 
              << m.gridSize() << " punti totali).\n";
    return m;
  }

  static Mesh Random(int N, double L) {
    Mesh m(N, L);
    m.buildRandom(N, L);
    std::cout << "[Mesh] Griglia Random creata con successo (" 
              << m.gridSize() << " punti totali).\n";
    return m;
  }

  // Costruttore per input manuale da tastiera
  Mesh(int N) {
    if (N <= 0) {
      throw std::invalid_argument("N must be positive.");
    }
    coords_x.reserve(N);
    coords_y.reserve(N);
    coords_z.reserve(N);

    std::cout << "\n=== Inserimento Coordinate delle Cariche ===\n";
    double x, y, z;
    for (int i = 0; i < N; i++) {
      std::cout << "Inserisci le coordinate x y z per la carica " << (i + 1) << ": ";
      std::cin >> x >> y >> z;
      coords_x.push_back(x);
      coords_y.push_back(y);
      coords_z.push_back(z);
    }
    std::cout << "[Mesh] Griglia interattiva delle cariche creata con " 
              << gridSize() << " punti.\n";
  }

  size_t gridSize() const { return coords_x.size(); }

  double x(int i) const { return coords_x.at(i); }
  double y(int i) const { return coords_y.at(i); }
  double z(int i) const { return coords_z.at(i); }

  const std::vector<double>& xVec() const { return coords_x; }
  const std::vector<double>& yVec() const { return coords_y; }
  const std::vector<double>& zVec() const { return coords_z; }
};


class Source {
  
private:
  Mesh grid;
  std::vector<double> values;

public:
  Source(Mesh g, std::string field) : grid(g) {
    if (field == "scalar") {
      values.reserve(g.gridSize());
      double q;
      std::cout << "\n=== Inserimento Valori delle Cariche ===\n";
      for (size_t i = 0; i < g.gridSize(); i++) {
        std::cout << "Inserisci il valore scalare q per la carica " << (i + 1) << ": ";
        std::cin >> q;
        values.push_back(q);
      }
      std::cout << "[Source] Sorgenti scalari impostate correttamente per tutte le " 
                << values.size() << " cariche.\n";
    } else {
      std::cerr << "Source: unsupported field type \"" << field
                << "\". Only \"scalar\" is currently supported. Exiting.\n";
      std::exit(EXIT_FAILURE);
    }
  }

  const Mesh& sourceGrid() const { return grid; }
  size_t sourceSize() const { return values.size(); }
  const std::vector<double>& valuesVec() const { return values; }
  double value(int i) const { return values.at(i); }
  
};


class Voltage {
  
private:
  
  Mesh grid;
  Source charges;
  std::vector<double> potentials;

  void computePotential() {
    double diff_x, diff_y, diff_z, dist_j, charge;
    potentials.reserve(grid.gridSize());

    for (size_t j = 0; j < grid.gridSize(); j++) {
      double V_j = 0.;
      for (size_t i = 0; i < charges.sourceSize(); i++) {
        diff_x = charges.sourceGrid().x(i) - grid.x(j);
        diff_y = charges.sourceGrid().y(i) - grid.y(j);
        diff_z = charges.sourceGrid().z(i) - grid.z(j);

        dist_j = std::sqrt(std::pow(diff_x, 2) + std::pow(diff_y, 2) + std::pow(diff_z, 2));
        charge = charges.value(i);

        V_j += charge / dist_j;
      }
      potentials.push_back(V_j);
    }
    std::cout << "\n[Voltage] Calcolo del potenziale completato.\n";
  }

public:
  
  Voltage(Mesh g, Source ch) : grid(g), charges(ch) { computePotential(); }

  const Mesh& voltageGrid() const { return grid; }
  size_t potentialSize() const { return potentials.size(); }
  double potential(int j) const { return potentials.at(j); }

  void printToFile(const std::string& filename) const {
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
    std::cout << "[Voltage] Potenziale salvato nel file: " << filename << "\n";
  }
};

int main() {
  try {
    std::cout << "--- Inizio Esecuzione Programma ---\n\n";

    // Mesh regolare 3x3x3=27 punti: dove calcolerò il potenziale
    Mesh fieldGrid = Mesh::Regular(3, 2.0);

    // Mesh interattiva, 3 punti: posizioni delle cariche
    Mesh chargeGrid(3);

    // Chiede il valore di ciascuna delle 3 cariche
    Source charges(chargeGrid, "scalar");

    // Calcola il potenziale di "charges" su tutti i punti di "fieldGrid"
    Voltage V(fieldGrid, charges);

    // Scrive il risultato su file
    V.printToFile("voltage.txt");

  } catch (const std::exception& e) {
    std::cerr << "Errore: " << e.what() << "\n";
    return EXIT_FAILURE;
  }

  return 0;
}
