#include "Source.h"
#include <iostream>  // per std::cin e std::cerr
#include <cstdlib>  // per std::exit() e EXIT_FAILURE

Source::Source(Mesh g, std::string field) : grid(g) {
  if (field == "scalar") {
    values.reserve(g.gridSize());
    double q;
    for (size_t i = 0; i < g.gridSize(); i++) {
      std::cin >> q;
      values.push_back(q);
    }
  } else {
    std::cerr << "Source: unsupported field type \"" << field
              << "\". Only \"scalar\" is currently supported. Exiting.\n";
    std::exit(EXIT_FAILURE);
  }
}

const Mesh& Source::sourceGrid() const { return grid; }
size_t Source::sourceSize() const { return values.size(); }
const std::vector<double>& Source::valuesVec() const { return values; }
double Source::value(int i) const { return values.at(i); }
