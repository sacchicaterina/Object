#ifndef RIGHT_ANGLE_TRIANGLE_H
#define RIGHT_ANGLE_TRIANGLE_H

#include "Triangle.h"

class RightAngleTriangle : public Triangle {
private:
    // Il costruttore è PRIVATO. 
    // Nessuno può fare "RightAngleTriangle t(...)" dal main.
    // Accetta tre lati perché verranno calcolati dai metodi statici.
    RightAngleTriangle(const std::string& lbl, double a, double b, double c);
 public:
    // NAMED CONSTRUCTOR IDIOM
    // Questi metodi sono statici: si usano senza avere ancora l'oggetto.
    
    // 1. Crea il triangolo partendo dai due cateti
    static RightAngleTriangle createFromLegs(const std::string& lbl, double leg1, double leg2);

    // 2. Crea il triangolo partendo da ipotenusa e un cateto
    static RightAngleTriangle createFromHypotenuse(const std::string& lbl, double hyp, double leg);

    // Distruttore
    ~RightAngleTriangle() override;

    // Override del metodo print per specificare che è un triangolo rettangolo
    void print() const override;
};

#endif
