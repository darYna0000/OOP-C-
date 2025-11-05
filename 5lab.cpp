#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>

using namespace std;
const double PI = 3.14;

class GeometricBody {
public:
    virtual double calculateSurfaceArea() const = 0; 
    virtual double calculateVolume() const = 0;      
    virtual void printInfo() const = 0;              
    
    virtual ~GeometricBody() {}
};


class Sphere : 
public GeometricBody {
private:
    double radius; 
    
public:
    Sphere(double r) : radius(r) {
        if (radius <= 0) radius = 1.0; 
    }
 
    double calculateSurfaceArea() const override {
        return 4.0 * PI * radius * radius;
    }

    double calculateVolume() const override {
        return (4.0 / 3.0) * PI * pow(radius, 3);
    }

    void printInfo() const override {
        cout << "Body: Sphere" <<endl;
        cout << "  Radius: " << radius <<endl;
    }
};

class Pyramid : 
public GeometricBody {
private:
    double base_length; 
    double base_width;  
    double height;      
    
public:
    Pyramid(double l, double w, double h) : base_length(l), base_width(w), height(h) {
        if (base_length <= 0 || base_width <= 0 || height <= 0) {
            base_length = base_width = height = 1.0; 
        }
    }

    // override virtualmethod
    double calculateVolume() const override {
        return (1.0 / 3.0) * base_length * base_width * height;
    }

    //S Sphere
    double calculateSurfaceArea() const override {
        double base_area = base_length * base_width;
        
        //Apothem
        double slant_height_l = sqrt(height * height + (base_width / 2.0) * (base_width / 2.0));
        double slant_height_w = sqrt(height * height + (base_length / 2.0) * (base_length / 2.0));
        
        // S side
        double lateral_area = base_length * slant_height_l + base_width * slant_height_w;
        
        return base_area + lateral_area;
    }
    
    void printInfo() const override {
        cout << "Body: Pyramid" <<endl;
        cout << "  Lenth (a): " << base_length <<endl;
        cout << "  Width (b): " << base_width <<endl;
        cout << "  Heigth (h): " << height <<endl;
    }
};

// later connection
void processBody(const GeometricBody* body_ptr) {
    cout << fixed << setprecision(2);

    cout << "------------------------------------------" << endl;
    body_ptr->printInfo(); // call virtual method
    
    cout << "  S side: " << body_ptr->calculateSurfaceArea() <<endl; 
    cout << "  V: " << body_ptr->calculateVolume() <<endl;             
}

int main() {
    Sphere s(5);
    Pyramid p(4.0, 6.0, 3.0);
    
    vector<GeometricBody*> bodies;
    bodies.push_back(&s);
    bodies.push_back(&p);
    bodies.push_back(new Sphere(1.5));
    bodies.push_back(new Pyramid(10.0, 10.0, 5.0));
    
    cout << "--- Demomstrate dynamic  ---" <<endl;
    cout << "A vector contains objects of different types, which are accessed" <<endl;
    cout << "through a single interface of the GeometricBody base class." <<endl;
    
    for (const auto& body : bodies) {
        processBody(body);
    }
    cout << "------------------------------------------" <<endl;
    
    delete bodies[2];
    delete bodies[3];

    return 0;
}
