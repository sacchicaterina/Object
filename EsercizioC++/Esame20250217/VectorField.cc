#include "VectorField.h"

#include <iostream>
#include <cmath>
#include <string>
#include <vector>

using namespace std;

//costruttore
VectorField::VectorField(const Function* fx, const Function* fy, const Function* fz) : f_x_(fx), f_y_(fy), f_z_(fz) {}

//distruttore:
VectorField::~VectorField() {}

std::vector<double> VectorField::value(double x, double y, double z) const {
  // Valutiamo ogni componente usando il polimorfismo (->value)
  // operatore freccia si usa quando usiamo i puntatori, va a guardare cosa c'e all'indirizzo di memoria indicaro dal putatore, accede a u metodo o una variabile di quell'oggetto sostitusce (*f_x_).value(x,y,z)
  //ha seso farlo perche i miei mebri privati non soo oggetti ma indirizzi che puntano ad oggetti di tipo fuction, usando la freccia dici al computer "vai all'indirizzo momorizzato in f_x_, li troverai un oggetto Fuction e di quell'oggetto chiama il metodo value.
    return { f_x_->value(x, y, z), f_y_->value(x, y, z), f_z_->value(x, y, z) };
}

double VectorField::divergence(double x, double y, double z) const {
  return f_x_->deriv(x, y, z, 'x') + f_y_->deriv(x, y, z, 'y') + f_z_->deriv(x, y, z, 'z');
}

std::vector<double> VectorField::curl(double x, double y, double z) const {
    double cx = f_z_->deriv(x, y, z, 'y') - f_y_->deriv(x, y, z, 'z');
    double cy = f_x_->deriv(x, y, z, 'z') - f_z_->deriv(x, y, z, 'x');
    double cz = f_y_->deriv(x, y, z, 'x') - f_x_->deriv(x, y, z, 'y');
    
    return {cx, cy, cz};
}

void VectorField::printVector(double x, double y, double z) const {
    std::vector<double> v = value(x, y, z);
    std::vector<double> c = curl(x, y, z);
    
    std::cout << "--- Campo Vettoriale nel punto (" << x << "," << y << "," << z << ") ---" << std::endl;
    std::cout << "Valore F: (" << v[0] << ", " << v[1] << ", " << v[2] << ")" << std::endl;
    std::cout << "Divergenza: " << divergence(x, y, z) << std::endl;
    std::cout << "Rotore: (" << c[0] << ", " << c[1] << ", " << c[2] << ")" << std::endl;
}
