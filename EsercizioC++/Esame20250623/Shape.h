#ifndef SHAPE_H
#define SHAPE_H

#include <string> // Necessario per l'attributo label

class Shape{
 protected:
  std::string label; // L'attributo richiesto
 public:
  // Un costruttore per inizializzare la label
  Shape(const std::string& l) : label(l) {}
  virtual void print() const=0;

  virtual ~Shape() {}

};
#endif
