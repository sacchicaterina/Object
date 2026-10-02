#include "Polyomial.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main() {

  vector<double> test_coeff = {1.0, 4.0, 5.9, 12.0, 4.0, 0.0, 2.0};
  Polynomial p1("PolyTest", test_coeff);

  cout<< "--- TEST CLASSE POLYNOMIAL ---" << endl;

  double x = 5.7;

  p1.print(x);

  cout << "\nModifica coefficienti in corso..." << endl;
  vector<double> nuovi_coeff = {0, 0, 0, 0, 0, 0, 0, 0, 0}; // Solo a9 * y * z
  p1.setCoeff(nuovi_coeff);

  p1.print(x);

  return 0;
}
