#include "Particle.h"
#include <iostream>
#include <cmath>

using namespace std;

//constructor
Particle::Particle(double mval, double zval, double pval){
  mvalue_ = mval;
  zvalue_ = zval;
  pvalue_ = pval;
}

//getters
double Particle::mvalue() const {return mvalue_;}
double Particle::zvalue() const {return zvalue_;}
double Particle::pvalue() const {return pvalue_;}

//setters
void Particle::setmvalue(double mvalue) {mvalue_=mvalue;}
void Particle::setzvalue(double zvalue) {zvalue_=zvalue;}
void Particle::setpvalue(double pvalue) {pvalue_=pvalue;}

double Particle::beta () const {
  return pvalue_ / sqrt( pow(pvalue_, 2) + pow(mvalue_, 2) );
}

void Particle::print() const {
  cout << "Particle:" << mvalue_ << " MeV, " << zvalue_ << " , " << pvalue_ << " MeV, " << endl;
  cout << "valore di beta:" <<  beta() << endl;
}
