#include "Polyomial.h"

#include <iostream>
#include <cmath>
#include <vector>
#include <string>

using namespace std;

Polynomial::Polynomial(const std::string& name, const std::vector<double>& coefficienti) : Function(name) {
      coeffs_ = coefficienti;
      //empty() serve a verificare se il conteitore e' vuoto, restituisce un  valore boleano "True" se il vettore non contiene alcun elemento, "False" se contiene almeno un elemento.
      // Piu carino che usare if(coeffs_.size() == 0)
      if (coeffs_.empty()) {
	degree_ = 0;
      }
      else {
	//conversione di tipo esplicita, size() non restituisce un int ma un tipo chiamto size_t (un unsigned), allora essendo che int e' invece signed. Si usa static_cast serve proprio per dire "convertilo in un int" 
	degree_ = static_cast<int>(coefficienti.size()) - 1;
      }
}

Polynomial::~Polynomial(){}

//getter e setter
std::vector<double> Polynomial::getCoeff() const {return coeffs_;}

void Polynomial::setCoeff(const std::vector<double>& nuovi_coeff) {
  coeffs_ = nuovi_coeff;
  if (coeffs_.empty()) {
        degree_ = 0;
      }
      else {
	degree_ = static_cast<int>(nuovi_coeff.size()) - 1;
      }
  
}

double Polynomial::value(double x) const {
  double result = 0.0; //metto a zero il risulato

  //creo il ciclo for
  for(int i = 0 ; i <=degree_; ++i)
    {
      result += coeffs_[i] * pow(x, i);
    }
  
  return result;
}

double Polynomial::deriv(double x) const {
  double derivata = 0.0;

  for (int i = 1 ; i <=degree_; ++i)
    {
      derivata += i * coeffs_[i] * pow(x, i-1);
    }
  return derivata;
}

double Polynomial::primitive(double x) const {
  double primitiva = 0.0;

  for (int i = 0 ; i <=degree_; ++i)
    {
      primitiva += coeffs_[i]/(i+1) * pow(x, i+1);
    }
  return primitiva;
}

void Polynomial::print(double x) const {
  cout << "Funzione: " << name() << endl;
  cout << "Coefficienti: ";
  for (double c : coeffs_) {
    cout <<c << " ";
  }
  cout << endl;

  cout << "Valore di x: " << x << endl;
  cout << "Valore: " << value(x) << endl;
  cout << "Derivata: " << deriv(x) << endl;
  cout << "Primitiva: " << primitive(x) << endl;

  cout << "Esecuzione completata con successo" << endl;
}


    
  
