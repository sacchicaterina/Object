#ifndef CONE_H
#define CONE_H

#include "Shape.h"

class Cone : public Shape{
private:
  double raggio_;
  double altezza_;

  //memorizzare
  double base_;
  double volume_;
  double superficie_;

public:
  ~Cone() override;

  Cone(const std::string& lbl, double rval, double hval);

  double rvalue() const;
  double hvalue() const;

  void set_r(double rvalue);
  void set_h(double hvalue);

  void get_base();
  void get_superficie();
  void get_volume();

  void print() const override;
};
#endif
  
  
