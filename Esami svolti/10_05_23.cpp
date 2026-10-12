#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cmath>
#include <vector>
#include <array>



class PointCharge {

private:

  std::array<double,3> position;
  double charge;

public:

  PointCharge(const std::array<double,3>& pos, double ch) : position(pos), charge(ch) {}

  double getCharge() const { return charge; }
  std::array<double,3> getPosition() const { return position; }

  void setCharge(double ch) { charge = ch; }
  void setPosition(const std::array<double,3>& pos) { position = pos; }

  void printCharge() const {

    std::cout << "Charge: " << charge << " C, Position: (";

    for (size_t i = 0; i < position.size(); ++i) {
        std::cout << position[i];
	//stampare l'ultimo numero senza virgola
	if (i < position.size() - 1) std::cout << ", ";
        }
        std::cout << ")\n";
    }


};




class PointChargeDist {
  
private:
  
    std::vector<PointCharge> charges;
    static constexpr double EPSILON_0 = 8.854e-12; // C^2 N^-1 m^-2
    static constexpr double K = 1.0 / (4.0 * 3.141592653589793 * EPSILON_0);

  
public:
   
    explicit PointChargeDist(const std::string& filename) {
        //ifstream per lettura, ofstream per scrittura
        std::ifstream file(filename);
	
        if (!file.is_open()) {
            throw std::runtime_error("PointChargeDist: impossibile aprire il file '" + filename + "'");
        }
	
        double q, x, y, z;
        // Legge le righe nel formato: q x y z
        while (file >> q >> x >> y >> z) {
	  //devo passare {x,y,z} e non pos perchè qui non ho la variabile, dovrei dichairarla con uno std::array
            charges.push_back(PointCharge({x, y, z}, q));
	    //charges.emplace_back(pos, charge);   // <-- ordine: prima pos, poi charge
        }
	
        file.close();
    }

  
  void addCharge(const PointCharge& a) {
    charges.push_back(a);
  }


  void removeCharge(size_t index) {
        if (index >= charges.size()) {
            throw std::out_of_range("Indice fuori dai limiti del sistema di cariche.");
        }
        charges.erase(charges.begin() + index);
    }


  size_t getNumberCharges() const { return charges.size(); }    //getter

  void printCharges() const {

    for(const auto& c : charges) {          //for(size_t i = 0, i < charges.size(); i++) { charges[i].print(); } 
       //applico il metodo della classe precedente all'oggetto
       c.printCharge();
     }
      
  }

  //r punto dello spazio dove voglio calcolare il campo elettrico
  std::array<double,3> electricField(const std::array<double,3>& r) const {

    std::array<double,3> electfield = {0., 0., 0.}; //inizializzato perchè dopo c'è +=, serve partire da un valore
    std::array<double,3> diff;

    for(const auto& c : charges) {

       double sum = 0.;
       double ch = c.getCharge();
       std::array<double,3> p = c.getPosition();
      
      for(size_t i = 0; i < p.size(); i++) {
	  
	  diff[i] = r[i] - p[i];	  
	  sum += std::pow(diff[i], 2);
	 
	  }
      
          double dist = std::sqrt(sum);

	  
      for(size_t i = 0; i < p.size(); i++) {
	  electfield[i]  += K * (ch / (std::pow(dist, 3)) * diff[i]);
     }
   }

       return electfield;
  }

  
};



int main() {

  PointCharge point({5., 3., 2.}, 1.5);
  point.printCharge();

  point.setCharge(1.3);
  point.setPosition({3., 2., 2.3});
  point.printCharge();


   PointChargeDist system("charges.txt");
 
    std::cout << "--- Initial system ---" << std::endl;
    system.printCharges();
    std::cout << "Number of charges: " << system.getNumberCharges() << std::endl;
    std::cout << std::endl;
 
    // 2. Add a new charge to the system.
    PointCharge extraCharge({0.5, 0.5, 0.5}, 2.);
    system.addCharge(extraCharge);
 
    std::cout << "--- After adding a charge ---" << std::endl;
    system.printCharges();
    std::cout << "Number of charges: " << system.getNumberCharges() << std::endl;
    std::cout << std::endl;
 
    // 3. Remove the first charge (index 0) from the system.
    system.removeCharge(0);
 
    std::cout << "--- After removing charge at index 0 ---" << std::endl;
    system.printCharges();
    std::cout << "Number of charges: " << system.getNumberCharges() << std::endl;
    std::cout << std::endl;

    
    // 4. Evaluate the electric field at a given point r = (x, y, z).
    std::array<double, 3> r = {1.0, 1.0, 1.0};
    std::array<double, 3> E = system.electricField(r);

    std::cout << "E = (";
    for (size_t i = 0; i < E.size(); ++i) {
         std::cout << E[i];
       if (i < E.size() - 1) std::cout << ", ";
       }
     std::cout << ") V/m" << std::endl;


   // 4. Evaluate the electric field at a given point r = (x, y, z).
   //std::array<double, 3> r = {1.0, 1.0, 1.0};
   //std::array<double, 3> E = system.electricField(r);

   //const std::array<char, 3> labels = {'x', 'y', 'z'};
   //for (size_t i = 0; i < E.size(); ++i) {
   //      std::cout << "E" << labels[i] << " = " << E[i] << " V/m" << std::endl;
   //}
 
}



  

