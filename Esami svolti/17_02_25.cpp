#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>   // <-- necessario per std::invalid_argument


class Function {
  
public:
  
    Function(const std::string& name) : name_(name) {}

    virtual ~Function();

    virtual double value(double x, double y, double z) const = 0;
    virtual double deriv(double x, double y, double z, char var) const = 0;

   
    virtual std::string name() const { return name_; }

private:
  
    std::string name_;
};


class Polynomial : public Function {
  
private:
  
    std::vector<double> coefficients;

public:
  
    static constexpr int SIZE = 10;

    // Costruttore secondo la consegna (solo i coefficienti, nome fissato internamente)
    Polynomial(const std::vector<double>& coeffs) : Function("Polynomial") {
        setCoefficients(coeffs);
    }

    void setCoefficients(const std::vector<double>& coeffs) {   // passato per const&, evita copie inutili
        if (coeffs.size() != SIZE) {
            throw std::invalid_argument("Polynomial: servono esattamente 10 coefficienti");
        }
        coefficients = coeffs;
    }

    
    std::vector<double> getCoefficients() const { return coefficients; }   //getter

    double value(double x, double y, double z) const {
      
        return coefficients[0]
             + coefficients[1] * x
             + coefficients[2] * y
             + coefficients[3] * z
             + coefficients[4] * x * x
             + coefficients[5] * y * y
             + coefficients[6] * z * z
             + coefficients[7] * x * y
             + coefficients[8] * x * z
             + coefficients[9] * y * z;
    }

    double deriv(double x, double y, double z, char var) const {
        
        if (var == 'x') {
            return coefficients[1] + 2.0 * coefficients[4] * x + coefficients[7] * y + coefficients[8] * z;
        }
        else if (var == 'y') {
            return coefficients[2] + 2.0 * coefficients[5] * y + coefficients[7] * x + coefficients[9] * z;
        }
        else if (var == 'z') {
            return coefficients[3] + 2.0 * coefficients[6] * z + coefficients[8] * x + coefficients[9] * y;
        }
        else {
           
            throw std::invalid_argument("Polynomial::deriv: var deve essere 'x', 'y' o 'z'");
        }
    }   
};      



class VectorField {
  
private:
  
    std::vector<const Function*> functions;

public:
  
    static constexpr int SIZE = 3;

    VectorField(const std::vector<const Function*>& funcs) {
        setPointers(funcs);
    }

    void setPointers(const std::vector<const Function*>& funcs) {
      
        if (funcs.size() != SIZE) {
            throw std::invalid_argument("VectorField: servono esattamente 3 puntatori a Function");
        }
        functions = funcs;
    }

    
    std::vector<const Function*> getFunctions() const { //getter
       return functions;
    }

    std::vector<double> evaluate(double x, double y, double z) const {
      
        std::vector<double> values;
        values.reserve(SIZE);
	
        for (const auto& func : functions) {
            values.push_back(func->value(x, y, z));
        }
        return values;
    }

    double divergence(double x, double y, double z) const {
      
        double sum = 0.;
        int pos = 0;
        char variabili[3] = {'x', 'y', 'z'};
	
        for (const auto& func : functions) {
            sum += func->deriv(x, y, z, variabili[pos]);
            pos++;
        }
        return sum;
    }

   std::vector<double> curl(double x, double y, double z) const {
     
    std::vector<double> values;
    values.reserve(SIZE);   

    values.push_back(functions[2]->deriv(x, y, z, 'y') - functions[1]->deriv(x, y, z, 'z'));  
    values.push_back(functions[0]->deriv(x, y, z, 'z') - functions[2]->deriv(x, y, z, 'x'));  
    values.push_back(functions[1]->deriv(x, y, z, 'x') - functions[0]->deriv(x, y, z, 'y'));  

    return values;
}

  
};


void printVector(const std::vector<double>& v) {
  
    std::cout << "(";

    for (double x : v) {              // <-- range-based for: scorre ogni elemento di v
        std::cout << x;
    }
    std::cout << ")";
}



int main() {

  std::cout << "=== TEST 1: Polynomial ===\n";

  Polynomial p({1., 2., 3., 4., 5., 6., 7., 8., 9., 10.});

  std::cout << "Nome: " << p.name() << std::endl;

  std::vector<double> coeffs = p.getCoefficients();
  std::cout << "Polynomial coefficients: " << std::endl;
  printVector(p);
  std::cout << std::endl;
 

  std::cout << "p(1,1,1) : " << p.value(1., 1., 1.) << std::endl;

  std::cout << "dp/dx(1.,1.,1.) :" << p.deriv(1., 1., 1.,'x') << std::endl;

  std::cout << "dp/dy(1.,1.,1.) :" << p.deriv(1., 1., 1.,'y') << std::endl;

  std::cout << "dp/dz(1.,1.,1.) :" << p.deriv(1., 1., 1.,'z') << std::endl;
  
  p.setCoefficients({0, 1, 0, 0, 0, 0, 0, 0, 0, 0});   // ora p(x,y,z) = x
  std::cout << "Dopo setCoefficients, p(5,0,0) = " << p.value(5.0, 0.0, 0.0) << std::endl;


  
  std::cout << "\n=== TEST 2: VectorField F(x,y,z) = (y, -x, 0) ===\n";

   Polynomial fx({0, 0, 1, 0, 0, 0, 0, 0, 0, 0});    // Fx = y
   Polynomial fy({0, -1, 0, 0, 0, 0, 0, 0, 0, 0});   // Fy = -x
   Polynomial fz({0, 0, 0, 0, 0, 0, 0, 0, 0, 0});    // Fz = 0
 
   VectorField f({&gx, &gy, &gz});


   std::vector<double> val = f.evaluate(x, y, z);
   std::cout << "F(" << x << "," << y << "," << z << ") = ";
   printVector(val);
   std::cout << std::endl;


   std::cout << "div F = " << f.divergence(x, y, z) << std::endl;


   std::vector<double> c = f.curl(x, y, z);
   std::cout << "curl F = ";
   printVector(c);
   std::cout << std::endl;
  







  
}
