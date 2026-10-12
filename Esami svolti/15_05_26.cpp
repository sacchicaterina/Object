#include <iostream>
#include <array>
#include <string>
#include <vector>
#include <cmath>
#include <stdexcept>


class PhysicalObject {
protected:
    double mass;
    std::string label;
public:
    PhysicalObject(double m, const std::string& lbl) : mass(m), label(lbl) {
        if (m <= 0) { throw std::invalid_argument("mass has to be positive"); }
    }
    virtual ~PhysicalObject() = default;
    double getMass() const { return mass; }
    std::string getLabel() const { return label; }
    void setMass(double m) {
        if (m <= 0) { throw std::invalid_argument("mass has to be positive"); }
        mass = m;
    }
    void setLabel(const std::string& lbl) { label = lbl; }
    virtual double calculateKineticEnergy() const = 0;
};

class TranslationalObject : public PhysicalObject {
private:
    std::array<double, 3> velocity;
    double calculateVel() const {
        double sum = 0.;
        for (int i = 0; i < 3; i++) { sum += velocity[i] * velocity[i]; }
        return std::sqrt(sum);
    }
public:
    TranslationalObject(double m, const std::string& lbl, const std::array<double,3>& v)
        : PhysicalObject(m, lbl), velocity(v) {}
    double getSpeed() const { return calculateVel(); }
    void setVelocity(std::array<double,3>& v) { velocity = v; }
    double calculateKineticEnergy() const {
        double vel = calculateVel();
        return 0.5 * mass * vel * vel;
    }
};

class RotatingDisk : public PhysicalObject {
protected:
    double radius, omega, axisSeparation;
    double calculateMomentOfInertia() const {
        return 0.5 * mass * radius * radius + mass * axisSeparation * axisSeparation;
    }
public:
    RotatingDisk(double m, const std::string& lbl, double r, double om, double sep)
        : PhysicalObject(m, lbl), radius(r), omega(om), axisSeparation(sep)
    {
        if (r <= 0) { throw std::invalid_argument("radius has to be positive"); }
        if (sep < 0) { throw std::invalid_argument("separation cannot be negative"); }
    }
    double getRadius() const { return radius; }
    double getOmega() const { return omega; }
    double getAxisSeparation() const { return axisSeparation; }
    double getMomentOfInertia() const { return calculateMomentOfInertia(); }
    void setRadius(double R) {
        if (R <= 0) { throw std::invalid_argument("radius has to be positive"); }
        radius = R;
    }
    void setOmega(double w) { omega = w; }
    void setAxisSeparation(double d) {
        if (d < 0) { throw std::invalid_argument("axis separation cannot be negative"); }
        axisSeparation = d;
    }
    double calculateKineticEnergy() const {
        double I = calculateMomentOfInertia();
        return 0.5 * I * omega * omega;
    }
};

class PureRollingObject : public RotatingDisk {
public:
    PureRollingObject(double m, const std::string& lbl, double r, double om, double sep)
        : RotatingDisk(m, lbl, r, om, sep) {}
    double calculateKineticEnergy() const {
        double roto_energy = RotatingDisk::calculateKineticEnergy();
        double v = omega * radius;
        double transl_energy = 0.5 * mass * v * v;
        return roto_energy + transl_energy;
    }
};

int main() {
    std::cout << "=== Physical objects kinetic energy calculator ===\n\n";

    int nObjects;
    std::cout << "How many objects do you want to create? ";
    std::cin >> nObjects;
    while (nObjects <= 0) {
        std::cout << "Please enter a positive number: ";
        std::cin >> nObjects;
    }

    std::vector<PhysicalObject*> objects;
    objects.reserve(nObjects);

    for (int i = 0; i < nObjects; i++) {
        std::cout << "\n--- Object " << (i + 1) << " of " << nObjects << " ---\n";

        int choice;
        std::cout << "Choose type (1=Translational, 2=RotatingDisk, 3=PureRolling): ";
        std::cin >> choice;
        while (choice < 1 || choice > 3) {
            std::cout << "Invalid choice, please enter 1, 2 or 3: ";
            std::cin >> choice;
        }

        if (choice == 1) {
            double m, v, theta, phi;
            std::string lbl;

            std::cout << "Mass [kg]: ";
            std::cin >> m;
            std::cout << "Label for this object: ";
            std::cin >> lbl;
            std::cout << "Speed [m/s]: ";
            std::cin >> v;
            std::cout << "Polar angle theta [degrees, from z-axis, 90 = xy-plane]: ";
            std::cin >> theta;
            std::cout << "Azimuthal angle phi [degrees, in xy-plane]: ";
            std::cin >> phi;

            double theta_rad = theta * M_PI / 180.0;
            double phi_rad = phi * M_PI / 180.0;

            double vx = v * std::sin(theta_rad) * std::cos(phi_rad);
            double vy = v * std::sin(theta_rad) * std::sin(phi_rad);
            double vz = v * std::cos(theta_rad);

            PhysicalObject* newObj = new TranslationalObject(m, lbl, {vx, vy, vz});
            objects.push_back(newObj);
        }
        else if (choice == 2) {
            double m, R, omega, d;
            std::string lbl;

            std::cout << "Mass [kg]: ";
            std::cin >> m;
            std::cout << "Label for this object: ";
            std::cin >> lbl;
            std::cout << "Disk radius R [m]: ";
            std::cin >> R;
            std::cout << "Angular velocity omega [rad/s]: ";
            std::cin >> omega;
            std::cout << "Axis separation d [m]: ";
            std::cin >> d;

            PhysicalObject* newObj = new RotatingDisk(m, lbl, R, omega, d);
            objects.push_back(newObj);
        }
        else if (choice == 3) {
            double m, R, omega, d;
            std::string lbl;

            std::cout << "Mass [kg]: ";
            std::cin >> m;
            std::cout << "Label for this object: ";
            std::cin >> lbl;
            std::cout << "Disk radius R [m]: ";
            std::cin >> R;
            std::cout << "Angular velocity omega [rad/s]: ";
            std::cin >> omega;
            std::cout << "Axis separation d [m] (0 for rolling about own axis): ";
            std::cin >> d;

            PhysicalObject* newObj = new PureRollingObject(m, lbl, R, omega, d);
            objects.push_back(newObj);
        }
    }

    std::cout << "\n=== Kinetic energies ===\n";
    for (const auto* obj : objects) {
        std::cout << "Object \"" << obj->getLabel() << "\" (mass = " << obj->getMass()
                  << " kg): K = " << obj->calculateKineticEnergy() << " J\n";
    }

    for (auto* obj : objects) {
        delete obj;
    }
    objects.clear();

    return 0;
}
