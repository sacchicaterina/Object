#include "Mesh.h"
#include <iostream>
#include <random>
#include <stdexcept>

Mesh::Mesh(int N, double L) {
  if (N <= 0) {
    throw std::invalid_argument("N must be positive.");
  }
  if (L <= 0.0) {
    throw std::invalid_argument("L must be positive.");
  }
}

void Mesh::buildRegular(int N, double L) {
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

void Mesh::buildRandom(int N, double L) {
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

Mesh Mesh::Regular(int N, double L) {
  Mesh m(N, L);        // il costruttore privato fa già i controlli su N, L
  m.buildRegular(N, L);
  return m;
}

Mesh Mesh::Random(int N, double L) {
  Mesh m(N, L);        // stesso controllo comune riusato qui
  m.buildRandom(N, L);
  return m;
}

Mesh::Mesh(int N) {
  if (N <= 0) {
    throw std::invalid_argument("N must be positive.");
  }
  coords_x.reserve(N);
  coords_y.reserve(N);
  coords_z.reserve(N);

  double x, y, z;
  for (int i = 0; i < N; i++) {
    std::cin >> x >> y >> z;
    coords_x.push_back(x);
    coords_y.push_back(y);
    coords_z.push_back(z);
  }
}

size_t Mesh::gridSize() const { return coords_x.size(); }

double Mesh::x(int i) const { return coords_x.at(i); }  // .at() controlla il fuori range
double Mesh::y(int i) const { return coords_y.at(i); }
double Mesh::z(int i) const { return coords_z.at(i); }

const std::vector<double>& Mesh::xVec() const { return coords_x; }
const std::vector<double>& Mesh::yVec() const { return coords_y; }
const std::vector<double>& Mesh::zVec() const { return coords_z; }
