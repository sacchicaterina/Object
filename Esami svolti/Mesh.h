#ifndef MESH_H
#define MESH_H

#include <vector>

// Rappresenta un insieme di punti in 3D tramite le loro coordinate cartesiane.
// Costruzione tramite named constructor idiom (Regular, Random) oppure
// tramite il costruttore pubblico diretto Mesh(int N) per input interattivo.
class Mesh {

private:

  std::vector<double> coords_x;
  std::vector<double> coords_y;
  std::vector<double> coords_z;

  // Costruttore privato: prende N e L, e centralizza qui i controlli
  // comuni a Regular e Random, evitando di ripeterli in entrambi i build*.
  Mesh(int N, double L);

  void buildRegular(int N, double L);
  void buildRandom(int N, double L);

public:

  static Mesh Regular(int N, double L);
  static Mesh Random(int N, double L);

  // Costruttore pubblico diretto: legge N punti interattivamente da input.
  Mesh(int N);

  size_t gridSize() const;

  double x(int i) const;
  double y(int i) const;
  double z(int i) const;

  const std::vector<double>& xVec() const;
  const std::vector<double>& yVec() const;
  const std::vector<double>& zVec() const;
};

#endif // MESH_H
