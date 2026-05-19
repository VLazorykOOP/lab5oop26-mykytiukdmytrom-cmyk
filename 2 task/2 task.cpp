#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <windows.h>
using namespace std;

// =====================================================================
//  Клас Button
// =====================================================================
class Button {
private:
    int width;
    int height;

public:
    Button() : width(0), height(0) {
        cout << "  [Button] Конструктор за замовчуванням" << endl;
    }

    Button(int w, int h) : width(w), height(h) {
        cout << "  [Button] Конструктор з параметрами ("
            << w << " x " << h << ")" << endl;
    }

    Button(const Button& other) : width(other.width), height(other.height) {
        cout << "  [Button] Конструктор копіювання" << endl;
    }

    ~Button() {
        cout << "  [Button] Деструктор ("
            << width << " x " << height << ")" << endl;
    }

    int  getWidth()  const { return width; }
    int  getHeight() const { return height; }
    void setWidth(int w) { if (w > 0) width = w; }
    void setHeight(int h) { if (h > 0) height = h; }

    Button& operator=(const Button& other) {
        if (this != &other) { width = other.width; height = other.height; }
        return *this;
    }

    void display() const {
        cout << "    Кнопка:  " << width << " x " << height << " пкс" << endl;
    }
};

// =====================================================================
//  Клас Window
// =====================================================================
class Window {
protected:
    int    x, y;
    Button button;

public:
    Window() : x(0), y(0), button() {
        cout << "  [Window] Конструктор за замовчуванням" << endl;
    }

    Window(int x, int y, int btnW, int btnH)
        : x(x), y(y), button(btnW, btnH) {
        cout << "  [Window] Конструктор з параметрами ("
            << x << ", " << y << ")" << endl;
    }

    Window(const Window& other)
        : x(other.x), y(other.y), button(other.button) {
        cout << "  [Window] Конструктор копіювання" << endl;
    }

    virtual ~Window() {
        cout << "  [Window] Деструктор ("
            << x << ", " << y << ")" << endl;
    }

    int     getX()      const { return x; }
    int     getY()      const { return y; }
    void    setX(int v) { x = v; }
    void    setY(int v) { y = v; }
    Button& getButton() { return button; }

    Window& operator=(const Window& other) {
        if (this != &other) { x = other.x; y = other.y; button = other.button; }
        return *this;
    }

    virtual void display() const {
        cout << "  --- Вікно ---" << endl;
        cout << "    Координати: (" << x << ", " << y << ")" << endl;
        button.display();
    }
};

// =====================================================================
//  Клас MenuWindow
// =====================================================================
class MenuWindow : public Window {
private:
    char* menuLabel;

    void allocLabel(const char* src) {
        if (src) {
            menuLabel = new char[strlen(src) + 1];
            strcpy_s(menuLabel, strlen(src) + 1, src);
        }
        else {
            menuLabel = new char[1];
            menuLabel[0] = '\0';
        }
    }

public:
    MenuWindow() : Window(), menuLabel(nullptr) {
        allocLabel("(без назви)");
        cout << "  [MenuWindow] Конструктор за замовчуванням" << endl;
    }

    MenuWindow(int x, int y, int btnW, int btnH, const char* label)
        : Window(x, y, btnW, btnH), menuLabel(nullptr) {
        allocLabel(label);
        cout << "  [MenuWindow] Конструктор з параметрами, мітка: \""
            << menuLabel << "\"" << endl;
    }

    MenuWindow(const MenuWindow& other)
        : Window(other), menuLabel(nullptr) {
        allocLabel(other.menuLabel);
        cout << "  [MenuWindow] Конструктор копіювання" << endl;
    }

    MenuWindow& operator=(const MenuWindow& other) {
        if (this != &other) {
            Window::operator=(other);
            delete[] menuLabel;
            allocLabel(other.menuLabel);
        }
        return *this;
    }

    virtual ~MenuWindow() {
        cout << "  [MenuWindow] Деструктор, мітка: \""
            << (menuLabel ? menuLabel : "") << "\"" << endl;
        delete[] menuLabel;
        menuLabel = nullptr;
    }

    const char* getMenuLabel() const { return menuLabel; }

    void setMenuLabel(const char* newLabel) {
        delete[] menuLabel;
        allocLabel(newLabel);
    }

    virtual void display() const {
        cout << "  --- Вікно із кнопкою меню ---" << endl;
        cout << "    Координати: (" << x << ", " << y << ")" << endl;
        cout << "    Мітка меню: \"" << (menuLabel ? menuLabel : "") << "\"" << endl;
        button.display();
    }
};

// =====================================================================
//  Допоміжні функції
// =====================================================================
void printSection(const string& title) {
    cout << endl << "===============================" << endl;
    cout << "  " << title << endl;
    cout << "===============================" << endl;
}

// =====================================================================
//  1. Введення з КЛАВІАТУРИ
// =====================================================================
void inputFromKeyboard(MenuWindow& mw) {
    printSection("ВВЕДЕННЯ З КЛАВІАТУРИ");

    int  x, y, btnW, btnH;
    char label[100];

    cout << "Введіть координати вікна (x y): ";
    cin >> x >> y;

    cout << "Введіть розмір кнопки (ширина висота): ";
    cin >> btnW >> btnH;

    cout << "Введіть мітку меню: ";
    cin >> label;

    MenuWindow temp(x, y, btnW, btnH, label);
    mw = temp;

    cout << endl << "[Результат введення з клавіатури]" << endl;
    mw.display();
}

// =====================================================================
//  2. Введення з ФАЙЛУ
// =====================================================================
void createSampleFile(const string& filename) {
    ofstream fout(filename);
    if (!fout) {
        cout << "Помилка створення файлу!" << endl;
        return;
    }
    // Формат: x  y  btnW  btnH  мітка
    fout << "150 200 120 40 Файл" << endl;
    fout << "300 400 100 35 Правка" << endl;
    fout << "500 200  90 30 Довідка" << endl;
    fout.close();
    cout << "  Файл \"" << filename << "\" створено." << endl;
}

void inputFromFile(const string& filename) {
    printSection("ВВЕДЕННЯ З ФАЙЛУ");

    createSampleFile(filename);

    ifstream fin(filename);
    if (!fin) {
        cout << "Помилка відкриття файлу!" << endl;
        return;
    }

    int  x, y, btnW, btnH;
    char label[100];
    int  count = 0;

    while (fin >> x >> y >> btnW >> btnH >> label) {
        MenuWindow mw(x, y, btnW, btnH, label);
        cout << endl << "[Запис #" << ++count << " з файлу]" << endl;
        mw.display();
    }

    fin.close();
}

// =====================================================================
//  3. Введення ВИПАДКОВИХ даних
// =====================================================================
void inputFromRandom(int count = 3) {
    printSection("ВВЕДЕННЯ ВИПАДКОВИХ ДАНИХ");

    const char* labels[] = { "Файл", "Правка", "Вигляд",
                               "Вставка", "Довідка", "Інструменти" };
    const int   labelCount = 6;

    for (int i = 0; i < count; i++) {
        int         x = 50 + rand() % 551;   //  50..600
        int         y = 50 + rand() % 351;   //  50..400
        int         btnW = 60 + rand() % 141;   //  60..200
        int         btnH = 25 + rand() % 26;    //  25..50
        const char* label = labels[rand() % labelCount];

        MenuWindow mw(x, y, btnW, btnH, label);
        cout << endl << "[Випадковий об'єкт #" << i + 1 << "]" << endl;
        mw.display();
    }
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

    MenuWindow mw;
    int choice = -1;

    while (choice != 0) {
        showMenu();
        cin >> choice;

        switch (choice) {
        case 1:
            inputFromKeyboard(mw);
            break;
        case 2:
            inputFromFile("windows.txt");
            break;
        case 3:
            inputFromRandom(3);
            break;
        case 4:
            inputFromFile("windows.txt");
            inputFromRandom(2);
            inputFromKeyboard(mw);
            break;
        case 0:
            cout << endl << "До побачення!" << endl;
            break;
        default:
            cout << "Невірний вибір!" << endl;
        }
    }

    return 0;
}