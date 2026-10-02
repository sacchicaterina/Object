#ifndef PARTICLE_H
#define PARTICLE_H

class Particle{
 public:
  //constructor
  Particle(double m, double z, double p);

  //getters
  double mvalue() const;
  double zvalue() const;
  double pvalue() const;

  //setters
  void setmvalue(double mvalue);
  void setzvalue(double zvalue);
  void setpvalue(double pvalue);

  double beta () const;

  void print() const;

 private:
  double mvalue_;
  double zvalue_;
  double pvalue_;
};

#endif
		     
    
