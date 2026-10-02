#include "Polynomial.h"

#include <iostream>
#include <cmath>
#include <string>
#include <vector>

using namespace std;

Polynomial::Polynomial(const std::string& name, const std::vector<double>& coefficienti) : Function(name) {
  //Il testo chiede un vettore di 10 dimensioni
  if(coefficienti.size() == 10)
    {
      coeffs_ = coefficienti;
    }
  else
    {
      //stampo il messaggio di errore per la dimensione del vettore
      cerr << "errore nella dimensione del vettore" << endl;
    }
}

Polynomial::~Polynomial(){}

//impostiamo il getter e il setter

std::vector<double> Polynomial::getCoeff() const {return coeffs_;}
  
void Polynomial::setCoeff(const std::vector<double>& nuovi_coeff) {
  if(nuovi_coeff.size() == 10)
    {
      coeffs_ = nuovi_coeff;
    }
  else	
    {
      //stampo il messaggio di errore per la dimensione del vettore               
      cerr << "errore nella dimensione del vettore" << endl;
    }
}

double Polynomial::value(double x, double y, double z) const {

  // coeff_[0] = a_0;
  //coeff_[1] = a_1;
  //coeff_[2] = a_2;
  //coeff_[3] = a_3;
  //coeff_[4] = a_4;
  //coeff_[5] = a_5;
  //coeff_[6] = a_6;
  //coeff_[7] = a_7;
  //coeff_[8] = a_8;
  //coeff_[9] = a_9;

  //ricordarsi per il futoro che pow bello ma x*x in c++ e' piu veloce
  return coeffs_[0] + coeffs_[1] * x +  coeffs_[2] * y + coeffs_[3] * z + coeffs_[4] * pow(x,2) + coeffs_[5] * pow(y,2) + coeffs_[6] * pow(z,2) +  coeffs_[7] * x * y + coeffs_[8] * x * z + coeffs_[9] * y * z;
}

double Polynomial::deriv(double x, double y, double z, char var) const {

  // o uso switch come fatto qui sotto, altrimenti avrei dovuto usare if, else if, else: scritto
  // if (var == x) { return...}
  //else if (var == y) {} ect.

  switch (var) {
        case 'x':
            // Ritorna la derivata rispetto a x: a1 + 2*a4*x + a7*y + a8*z
            return coeffs_[1] + 2.0 * coeffs_[4] * x + coeffs_[7] * y + coeffs_[8] * z;

        case 'y':
            // Ritorna la derivata rispetto a y: a2 + 2*a5*y + a7*x + a9*z
            return coeffs_[2] + 2.0 * coeffs_[5] * y + coeffs_[7] * x + coeffs_[9] * z;

        case 'z':
            // Ritorna la derivata rispetto a z: a3 + 2*a6*z + a8*x + a9*y
            return coeffs_[3] + 2.0 * coeffs_[6] * z + coeffs_[8] * x + coeffs_[9] * y;

        default:
            // Caso in cui venga passata una lettera diversa da x, y, z
            cerr << "Errore: variabile non valida per la derivata!" << endl;
            return 0.0;
    }
}

void Polynomial::print(double x, double y, double z) const {
  cout << "Funzione: " << name() << endl;
  cout << "Coefficienti: ";
  for (double c : coeffs_) {
    cout <<c << " ";
  }
  cout << endl;

  cout << "Valori: (" << x << ", " << y << ", " << z << ")" << endl;
  
  cout << "valore: " << value(x,y,z) << endl;
  cout << "derivata: " << deriv(x,y,z, 'x') << endl;
  cout << "derivata: " << deriv(x,y,z, 'y') << endl;
  cout << "derivata: " << deriv(x,y,z, 'z') << endl;

  cout << "Esecuzione completata con successo" << endl;
}
    
	
  
