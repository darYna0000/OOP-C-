#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>

using namespace std;
// Визначення константи PI для зручності
const double PI = 3.14159265358979323846;

// --------------------------------------------------
// Абстрактний базовий клас: ГЕОМЕТРИЧНЕ ТІЛО
// --------------------------------------------------
class GeometricBody {
public:
    // Чисто віртуальні функції (pure virtual functions)
    // Вони роблять клас 'GeometricBody' абстрактним
    // і вимагають обов'язкової реалізації у всіх неконкретних похідних класах.
    virtual double calculateSurfaceArea() const = 0; // Площа поверхні
    virtual double calculateVolume() const = 0;      // Об'єм
    virtual void printInfo() const = 0;              // Виведення інформації про тіло
    
    // Віртуальний деструктор необхідний для коректного видалення
    // об'єктів похідних класів через вказівник на базовий клас.
    virtual ~GeometricBody() {}
};

// --------------------------------------------------
// Похідний клас: КУЛЯ
// --------------------------------------------------
class Sphere : public GeometricBody {
private:
    double radius; // Радіус
    
public:
    // Конструктор
    Sphere(double r) : radius(r) {
        if (radius <= 0) radius = 1.0; // Забезпечення коректних даних
    }

    // Перевизначення віртуальних функцій
    
    // Площа поверхні Кулі: 4 * PI * r^2
    double calculateSurfaceArea() const override {
        return 4.0 * PI * radius * radius;
    }

    // Об'єм Кулі: (4/3) * PI * r^3
    double calculateVolume() const override {
        return (4.0 / 3.0) * PI * std::pow(radius, 3);
    }

    // Виведення інформації
    void printInfo() const override {
        std::cout << "Тіло: Куля (сфера)" << std::endl;
        std::cout << "  Радіус (r): " << radius << std::endl;
    }
};

// --------------------------------------------------
// Похідний клас: ПІРАМІДА (Припустімо, ПІРАМІДА з прямокутною основою
// та висотою, що проектується у центр основи для спрощення)
// --------------------------------------------------
class Pyramid : public GeometricBody {
private:
    double base_length; // Довжина основи (a)
    double base_width;  // Ширина основи (b)
    double height;      // Висота (h)
    
public:
    // Конструктор
    Pyramid(double l, double w, double h) : base_length(l), base_width(w), height(h) {
        if (base_length <= 0 || base_width <= 0 || height <= 0) {
            base_length = base_width = height = 1.0; // Забезпечення коректних даних
        }
    }

    // Перевизначення віртуальних функцій
    
    // Об'єм Піраміди: (1/3) * (Площа Основи) * Висота
    // V = (1/3) * a * b * h (використовуємо формулу для прямокутної піраміди з розділу 5.4, завдання 8) [cite: 248]
    double calculateVolume() const override {
        return (1.0 / 3.0) * base_length * base_width * height;
    }

    // Площа поверхні Піраміди: Площа Основи + Площа Бічної Поверхні
    // S = a*b + Площа Бічної Поверхні (Складніша формула, але для демонстрації поліморфізму)
    double calculateSurfaceArea() const override {
        // Площа основи
        double base_area = base_length * base_width;
        
        // Обчислення апофеми для кожної пари трикутних граней
        // Апофема для граней з основою 'base_length': висота, опущена на 'base_length'
        double slant_height_l = std::sqrt(height * height + (base_width / 2.0) * (base_width / 2.0));
        // Апофема для граней з основою 'base_width': висота, опущена на 'base_width'
        double slant_height_w = std::sqrt(height * height + (base_length / 2.0) * (base_length / 2.0));
        
        // Площа бічної поверхні: 2 * (1/2 * base_length * slant_height_l) + 2 * (1/2 * base_width * slant_height_w)
        double lateral_area = base_length * slant_height_l + base_width * slant_height_w;
        
        return base_area + lateral_area;
    }
    
    // Виведення інформації
    void printInfo() const override {
        cout << "Тіло: Піраміда (з прямокутною основою)" << std::endl;
        cout << "  Довжина основи (a): " << base_length << std::endl;
        cout << "  Ширина основи (b): " << base_width << std::endl;
        cout << "  Висота (h): " << height << std::endl;
    }
};

// --------------------------------------------------
// Основна функція для демонстрації пізнього зв'язування
// --------------------------------------------------
void processBody(const GeometricBody* body_ptr) {
    // Встановлення точності виведення
    cout << std::fixed << std::setprecision(4);

    // Пізнє зв'язування: фактичний виклик методу (Sphere::calculateVolume() чи Pyramid::calculateVolume())
    // визначається під час виконання програми, залежно від реального типу об'єкта,
    // на який вказує 'body_ptr'.
    cout << "------------------------------------------" << std::endl;
    body_ptr->printInfo(); // Виклик віртуального методу
    
    cout << "  Площа поверхні: " << body_ptr->calculateSurfaceArea() << std::endl; // Виклик віртуального методу
    cout << "  Об'єм: " << body_ptr->calculateVolume() << std::endl;             // Виклик віртуального методу
}

int main() {
    // Створення об'єктів похідних класів
    Sphere s(5.0);
    Pyramid p(4.0, 6.0, 3.0);
    
    // Створення вектора вказівників на базовий клас (GeometricBody*)
    // Це класичний приклад поліморфічного контейнера.
    vector<GeometricBody*> bodies;
    bodies.push_back(&s);
    bodies.push_back(&p);
    bodies.push_back(new Sphere(1.5));
    bodies.push_back(new Pyramid(10.0, 10.0, 5.0));
    
    cout << "--- Демонстрація Пізнього Зв'язування (Динамічний Поліморфізм) ---" << std::endl;
    cout << "Вектор містить об'єкти різних типів, доступ до яких здійснюється" << std::endl;
    cout << "через єдиний інтерфейс базового класу GeometricBody*." << std::endl;
    
    // Ітерація по поліморфічному контейнеру
    for (const auto& body : bodies) {
        processBody(body);
    }
    cout << "------------------------------------------" << std::endl;
    
    // Прибирання динамічно виділеної пам'яті
    // Віртуальний деструктор забезпечує коректне видалення об'єктів похідних класів.
    delete bodies[2];
    delete bodies[3];

    return 0;
}