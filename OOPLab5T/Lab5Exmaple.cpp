#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <windows.h>
using namespace std;

// =====================================================================
//  Базовий клас Car
// =====================================================================
class Car {
protected:
    string brand;
    int    cylinders;
    double power;

public:
    Car() : brand(""), cylinders(0), power(0.0) {}

    Car(const string& brand, int cylinders, double power)
        : brand(brand), cylinders(cylinders), power(power) {
    }

    void setBrand(const string& newBrand) { brand = newBrand; }

    void setPower(double newPower) {
        if (newPower > 0)
            power = newPower;
        else
            cout << "Помилка: потужність має бути додатньою!" << endl;
    }

    string getBrand()     const { return brand; }
    int    getCylinders() const { return cylinders; }
    double getPower()     const { return power; }

    virtual void display() const {
        cout << "-------------------------------" << endl;
        cout << "  Тип:               Автомобіль" << endl;
        cout << "  Марка:             " << brand << endl;
        cout << "  Циліндри:          " << cylinders << endl;
        cout << "  Потужність (к.с.): " << power << endl;
    }

    // Зчитування з потоку (cin або ifstream)
    virtual void readFromStream(istream& in) {
        in >> brand >> cylinders >> power;
    }

    virtual ~Car() {}
};

// =====================================================================
//  Похідний клас Lorry
// =====================================================================
class Lorry : public Car {
private:
    double payload; // вантажопідйомність, тонни

public:
    Lorry() : Car(), payload(0.0) {}

    Lorry(const string& brand, int cylinders, double power, double payload)
        : Car(brand, cylinders, power), payload(payload) {
    }

    void reassignBrand(const string& newBrand) {
        cout << "  Марку змінено: " << brand << " -> " << newBrand << endl;
        setBrand(newBrand);
    }

    void setPayload(double newPayload) {
        if (newPayload > 0)
            payload = newPayload;
        else
            cout << "Помилка: вантажопідйомність має бути додатньою!" << endl;
    }

    double getPayload() const { return payload; }

    virtual void display() const {
        cout << "-------------------------------" << endl;
        cout << "  Тип:               Вантажівка" << endl;
        cout << "  Марка:             " << brand << endl;
        cout << "  Циліндри:          " << cylinders << endl;
        cout << "  Потужність (к.с.): " << power << endl;
        cout << "  Вантажопідйомн.:   " << payload << " т" << endl;
    }

    virtual void readFromStream(istream& in) {
        in >> brand >> cylinders >> power >> payload;
    }
};

// =====================================================================
//  Допоміжні функції виводу
// =====================================================================
void printHeader(const string& title) {
    cout << endl << "===============================" << endl;
    cout << "  " << title << endl;
    cout << "===============================" << endl;
}

// =====================================================================
//  1. Введення з КЛАВІАТУРИ
// =====================================================================
void inputFromKeyboard(Car& car, Lorry& lorry) {
    printHeader("ВВЕДЕННЯ З КЛАВІАТУРИ");

    cout << "--- Автомобіль ---" << endl;
    cout << "Введіть марку, кількість циліндрів, потужність:" << endl;
    cout << "> ";
    car.readFromStream(cin);

    cout << "--- Вантажівка ---" << endl;
    cout << "Введіть марку, к-ть циліндрів, потужність, вантажопідйомність (т):" << endl;
    cout << "> ";
    lorry.readFromStream(cin);

    cout << endl << "[Результат введення з клавіатури]" << endl;
    car.display();
    lorry.display();
}

// =====================================================================
//  2. Введення з ФАЙЛУ
// =====================================================================

// Створення тестового файлу з даними
void createSampleFile(const string& filename) {
    ofstream fout(filename);
    if (!fout) {
        cout << "Помилка створення файлу!" << endl;
        return;
    }
    // Формат рядка Car:   марка  циліндри  потужність
    // Формат рядка Lorry: марка  циліндри  потужність  вантажопідйомність
    fout << "Toyota   4  150.0" << endl; // Car
    fout << "Volvo    6  400.0  20.0" << endl; // Lorry
    fout.close();
    cout << "  Файл \"" << filename << "\" створено." << endl;
}

void inputFromFile(Car& car, Lorry& lorry, const string& filename) {
    printHeader("ВВЕДЕННЯ З ФАЙЛУ");

    createSampleFile(filename);

    ifstream fin(filename);
    if (!fin) {
        cout << "Помилка відкриття файлу \"" << filename << "\"!" << endl;
        return;
    }

    car.readFromStream(fin);
    lorry.readFromStream(fin);
    fin.close();

    cout << "[Результат зчитування з файлу]" << endl;
    car.display();
    lorry.display();
}

// =====================================================================
//  3. Введення за допомогою ДАТЧИКА ВИПАДКОВИХ ЧИСЕЛ
// =====================================================================
void inputFromRandom(Car& car, Lorry& lorry) {
    printHeader("ВВЕДЕННЯ ВИПАДКОВИХ ДАНИХ");

    // Набір марок для вибору
    vector<string> carBrands = { "Toyota", "Honda",  "BMW",    "Audi",   "Ford" };
    vector<string> lorryBrands = { "Volvo",  "MAN",    "Scania", "DAF",    "Iveco" };

    // --- Car ---
    string cBrand = carBrands[rand() % carBrands.size()];
    int    cCylinders = 4 + rand() % 5;              // 4..8
    double cPower = 80.0 + rand() % 221;         // 80..300

    car.setBrand(cBrand);
    car.setPower(cPower);
    // cylinders — задаємо через тимчасовий об'єкт і копіюємо
    Car tempCar(cBrand, cCylinders, cPower);
    car = tempCar;

    // --- Lorry ---
    string lBrand = lorryBrands[rand() % lorryBrands.size()];
    int    lCylinders = 6 + rand() % 3;              // 6..8
    double lPower = 250.0 + rand() % 351;        // 250..600
    double lPayload = 5.0 + rand() % 26;           // 5..30 т

    Lorry tempLorry(lBrand, lCylinders, lPower, lPayload);
    lorry = tempLorry;

    cout << "[Результат генерації випадкових даних]" << endl;
    car.display();
    lorry.display();
}

// =====================================================================
//  МЕНЮ
// =====================================================================
void showMenu() {
    cout << endl << "===============================" << endl;
    cout << "  ОБЕРІТЬ СПОСІБ ВВЕДЕННЯ:    " << endl;
    cout << "===============================" << endl;
    cout << "  1. З клавіатури" << endl;
    cout << "  2. З файлу" << endl;
    cout << "  3. Датчик випадкових чисел" << endl;
    cout << "  4. Всі три способи поспіль" << endl;
    cout << "  0. Вихід" << endl;
    cout << "-------------------------------" << endl;
    cout << "Ваш вибір: ";
}

// =====================================================================
//  MAIN
// =====================================================================
int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    setlocale(LC_ALL, "Ukrainian");
    srand(static_cast<unsigned int>(time(NULL)));

    Car   car;
    Lorry lorry;

    int choice = -1;
    while (choice != 0) {
        showMenu();
        cin >> choice;

        switch (choice) {
        case 1:
            inputFromKeyboard(car, lorry);
            break;
        case 2:
            inputFromFile(car, lorry, "cars.txt");
            break;
        case 3:
            inputFromRandom(car, lorry);
            break;
        case 4:
            inputFromFile(car, lorry, "cars.txt");
            inputFromRandom(car, lorry);
            inputFromKeyboard(car, lorry);
            break;
        case 0:
            cout << endl << "До побачення!" << endl;
            break;
        default:
            cout << "Невірний вибір, спробуйте ще раз." << endl;
        }
    }

    return 0;
}