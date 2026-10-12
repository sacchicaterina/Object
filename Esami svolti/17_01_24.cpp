#include <iostream>
#include <string>
#include <cmath>



class Shape {

 protected:
   std::string label;


 public:

  Shape(const std::string& lbl) : label(lbl) {}
  virtual ~Shape() {}  //destructor

  std::string getLabel() const { return label; }
  
  void setLabel(const std::string& lbl) { label = lbl; }

  virtual void print() const = 0;   //pure virtual method  
  
  
};



class Rectangle : public Shape {

  protected:
   double base;
   double height;
   double perimeter;
   double area;

  void updateAttributes() {                    //eager method --> private method + public method --> storage
    perimeter = ( base + height ) * 2.;
    area = base * height;
  }

  public:

   Rectangle(const std::string lbl, double b, double h) : Shape(lbl), base(b), height(h) { updateAttributes(); }
 
   double getBase() const { return base; }
   double getHeight() const { return height; }
   double getPerimeter() const { return perimeter; }
   double getArea() const { return area; }

   void setDimensions(double b, double h) {
     base = b;
     height = h;
     updateAttributes();
    
    }


  void print() const {
    
        std::cout << "Rectangle [" << label << "]: "
                  << "base = " << base << ", height = " << height
                  << ", area = " << area << ", perimeter = " << perimeter
                  << std::endl;
    }

   

};


class Sphere : public Shape {

 private:
  double radius;
  double surface_area;
  double volume;

  static constexpr double PI = 3.14159265358979323846;

  void updateAttributes() {                             //eager method --> private method + public method --> storage
    surface_area = 4 * PI * std::pow(radius, 2); 
    volume = 4./3. * PI * std::pow(radius, 3);
  }

 public:

  Sphere(const std::string& lbl, double rad) : Shape(lbl), radius(rad) { updateAttributes(); }

  double getRadius() const { return radius; }
  double getSurfaceArea() const { return surface_area; }
  double getVolume() const { return volume; }

  void setRadius(double rad) {
    radius = rad;
    updateAttributes();
  }
  

  void print() const {

        std::cout << "Sphere [" << label << "]: "
                  << "radius = " << radius
                  << ", surfaceArea = " << surface_area
                  << ", volume = " << volume
                  << std::endl;
    }

  
};


class Tile : public Rectangle {

 private:
  double mass;
  double lambda_coeff;

 public:

  Tile(const std::string& lbl, double b, double h, double m, double lambda) : Rectangle(lbl, b, h), mass(m), lambda_coeff(lambda){
    // Qui dentro non devi fare altro perché Rectangle chiama già updateAttributes()
  }

  double getMass() const { return mass; }
  double getLambda() const { return lambda_coeff; }
  
  double getDensity() const {                            //lazy method --> no storage
    return mass / area;
  
  }

  void dilate(double deltaT) {
    double factor = 1 + lambda_coeff * deltaT;
    base *= factor;
    height *= factor;

    updateAttributes();

  }

  void setMass(double m) { mass = m; }
  void setLambda(double lambda) { lambda_coeff = lambda; }


  void print() const {

        std::cout << "Tile [" << label << "]: "
                  << "base = " << base << ", height = " << height
                  << ", area = " << area << ", perimeter = " << perimeter
                  << ", mass = " << mass
                  << ", density = " << getDensity()
                  << std::endl;
    }
    

};



int main() {

   // Non posso istanziare Shape direttamente: e' astratta.
    // Shape s("test");  // ERRORE DI COMPILAZIONE 
 
    Rectangle rect("MyRectangle", 3.0, 4.0);
    Sphere sphere("MySphere", 2.5);
 
    rect.print();
    sphere.print();
 
    // Tile: mattonella di ceramica, esempio di valori realistici
    // base = 0.3 m, height = 0.3 m, mass = 1.2 kg
    // lambda tipico per la ceramica ~ 6e-6 1/K
    Tile tile("CeramicTile", 0.3, 0.3, 1.2, 6e-6);
 
    std::cout << "\n--- Before heating ---" << std::endl;
    tile.print();
 
    // Simulo un aumento di temperatura di 50 K
    tile.dilate(50.0);
 
    std::cout << "\n--- After dilation (deltaT = 50 K) ---" << std::endl;
    tile.print();
 
    // Dimostrazione di polimorfismo: uso puntatori a Shape per gestire
    // oggetti di tipo diverso in modo uniforme.
    std::vector<Shape*> shapes;
    shapes.push_back(&rect);
    shapes.push_back(&sphere);
    shapes.push_back(&tile);
 
    for (const auto& s : shapes) {
        s->print(); // chiama la print() corretta a runtime (virtual)
    }





  
}
