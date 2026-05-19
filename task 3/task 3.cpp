/*
 * Ієрархія класів: Person -> Student
 * Visual Studio 2019 | Кирилиця через wstring + wcout
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <iomanip>
#include <io.h>
#include <fcntl.h>
#include <windows.h>

 // Псевдонім для зручності
using wstr = std::wstring;

// ─────────────────────────────────────────────────────────────
// Базовий клас: Person
// ─────────────────────────────────────────────────────────────
class Person {
protected:
    wstr firstName;
    wstr lastName;
    int  age;

public:
    Person() : firstName(L"Невідомо"), lastName(L"Невідомо"), age(0) {}

    Person(const wstr& fn, const wstr& ln, int a)
        : firstName(fn), lastName(ln), age(a) {
    }

    // Конструктор копіювання
    Person(const Person& other)
        : firstName(other.firstName),
        lastName(other.lastName),
        age(other.age) {
    }

    // Оператор присвоювання
    Person& operator=(const Person& other) {
        if (this != &other) {
            firstName = other.firstName;
            lastName = other.lastName;
            age = other.age;
        }
        return *this;
    }

    virtual ~Person() {}

    wstr getFirstName() const { return firstName; }
    wstr getLastName()  const { return lastName; }
    int  getAge()       const { return age; }

    void setFirstName(const wstr& fn) { firstName = fn; }
    void setLastName(const wstr& ln) { lastName = ln; }
    void setAge(int a) { age = a; }

    // Вивід у потік (virtual)
    virtual std::wostream& print(std::wostream& os) const {
        os << L"[Person] "
            << L"Ім'я: " << firstName << L" " << lastName
            << L", Вік: " << age;
        return os;
    }

    // Введення з потоку (virtual)
    virtual std::wistream& read(std::wistream& is) {
        is >> firstName >> lastName >> age;
        return is;
    }
};

// Глобальні оператори через virtual-функції
std::wostream& operator<<(std::wostream& os, const Person& p) { return p.print(os); }
std::wistream& operator>>(std::wistream& is, Person& p) { return p.read(is); }


// ─────────────────────────────────────────────────────────────
// Похідний клас: Student
// ─────────────────────────────────────────────────────────────
class Student : public Person {
private:
    wstr   group;
    double gpa;
    int    course;

public:
    Student() : Person(), group(L"Невідомо"), gpa(0.0), course(1) {}

    Student(const wstr& fn, const wstr& ln, int a,
        const wstr& gr, double g, int c)
        : Person(fn, ln, a), group(gr), gpa(g), course(c) {
    }

    // Конструктор копіювання через базовий клас
    Student(const Student& other)
        : Person(other),
        group(other.group),
        gpa(other.gpa),
        course(other.course) {
    }

    // Оператор присвоювання через базовий клас
    Student& operator=(const Student& other) {
        if (this != &other) {
            Person::operator=(other);   // поля Person
            group = other.group;
            gpa = other.gpa;
            course = other.course;
        }
        return *this;
    }

    ~Student() override {}

    wstr   getGroup()  const { return group; }
    double getGpa()    const { return gpa; }
    int    getCourse() const { return course; }

    void setGroup(const wstr& gr) { group = gr; }
    void setGpa(double g) { gpa = g; }
    void setCourse(int c) { course = c; }

    // Перевизначений вивід
    std::wostream& print(std::wostream& os) const override {
        Person::print(os);
        os << L" | [Student] "
            << L"Група: " << group << L", "
            << L"Курс: " << course << L", "
            << L"Середній бал: "
            << std::fixed << std::setprecision(1) << gpa;
        return os;
    }

    // Перевизначене введення
    std::wistream& read(std::wistream& is) override {
        Person::read(is);
        is >> group >> gpa >> course;
        return is;
    }
};


// ─────────────────────────────────────────────────────────────
// Режим 1: Введення з клавіатури
// ─────────────────────────────────────────────────────────────
Student inputFromKeyboard() {
    std::wcout << L"\n=== Введення з клавіатури ===\n";
    wstr fn, ln, gr;
    int  age, course;
    double gpa;

    std::wcout << L"Ім'я: ";                   std::wcin >> fn;
    std::wcout << L"Прізвище: ";               std::wcin >> ln;
    std::wcout << L"Вік: ";                    std::wcin >> age;
    std::wcout << L"Група: ";                  std::wcin >> gr;
    std::wcout << L"Середній бал (0-100): ";   std::wcin >> gpa;
    std::wcout << L"Курс (1-6): ";             std::wcin >> course;

    return Student(fn, ln, age, gr, gpa, course);
}


// ─────────────────────────────────────────────────────────────
// Режим 2: Введення з файлу
// Формат рядка: Ім'я Прізвище Вік Група Бал Курс
// ─────────────────────────────────────────────────────────────
void createSampleFile(const std::string& filename) {
    std::ofstream fout(filename);
    if (!fout.is_open())
        throw std::runtime_error("Не вдалося створити файл");

    // Дані латиницею, щоб файл точно зчитався без проблем кодування
    fout << "# Format: FirstName LastName Age Group GPA Course\n";
    fout << "Vasyl Ivanenko 20 KN-21 85.5 2\n";
    fout << "Olena Petrenko 19 PZ-11 92.0 1\n";
    fout << "Mykola Sydorenko 22 IN-31 78.3 3\n";
    fout.close();
    std::wcout << L"Файл '" << filename.c_str() << L"' створено.\n";
}

std::vector<Student> inputFromFile(const std::string& filename) {
    std::ifstream fin(filename);
    if (!fin.is_open())
        throw std::runtime_error("Не вдалося відкрити файл: " + filename);

    std::vector<Student> students;
    std::string line;
    while (std::getline(fin, line)) {
        if (line.empty() || line[0] == '#') continue;
        // Конвертуємо вузький рядок у wide для потоку
        std::wistringstream wiss(wstr(line.begin(), line.end()));
        Student s;
        s.read(wiss);
        students.push_back(s);
    }
    fin.close();
    return students;
}


// ─────────────────────────────────────────────────────────────
// Режим 3: Генератор випадкових чисел
// ─────────────────────────────────────────────────────────────
Student inputFromRandom() {
    static const std::vector<wstr> firstNames = {
        L"Олег", L"Марія", L"Іван", L"Анна",
        L"Петро", L"Юлія", L"Андрій", L"Олена"
    };
    static const std::vector<wstr> lastNames = {
        L"Коваль", L"Мельник", L"Шевченко",
        L"Бондар", L"Кравченко", L"Лисенко"
    };
    static const std::vector<wstr> groups = {
        L"КН-21", L"КН-22", L"ІН-31",
        L"ПЗ-41", L"СІ-11", L"ТК-32"
    };

    wstr fn = firstNames[rand() % firstNames.size()];
    wstr ln = lastNames[rand() % lastNames.size()];
    int  age = 17 + rand() % 9;
    wstr gr = groups[rand() % groups.size()];
    double gpa = 50.0 + (rand() % 5001) / 100.0;
    int    course = 1 + rand() % 6;

    return Student(fn, ln, age, gr, gpa, course);
}


// ─────────────────────────────────────────────────────────────
// Демонстрація конструктора копіювання та оператора присвоювання
// ─────────────────────────────────────────────────────────────
void demonstrateCopyAndAssign(const Student& original) {
    std::wcout << L"\n--- Оригінал ---\n" << original << L"\n";

    // Конструктор копіювання
    Student copy(original);
    std::wcout << L"\n--- Копія (конструктор копіювання) ---\n" << copy << L"\n";

    // Оператор присвоювання
    Student assigned;
    assigned = original;
    std::wcout << L"\n--- Присвоєний об'єкт (operator=) ---\n" << assigned << L"\n";

    // Змінюємо копію — оригінал не повинен змінитися
    copy.setFirstName(L"ЗМІНЕНО");
    std::wcout << L"\n--- Після зміни копії, оригінал незмінний ---\n";
    std::wcout << L"Копія:    " << copy << L"\n";
    std::wcout << L"Оригінал: " << original << L"\n";
}


// ─────────────────────────────────────────────────────────────
// main
// ─────────────────────────────────────────────────────────────
int main() {
    // ── Перемикаємо stdout/stdin у режим UTF-16 (wide) ──
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);

    std::wcout << L"+----------------------------------------------+\n";
    std::wcout << L"|   Ієрархія класів: Person -> Student         |\n";
    std::wcout << L"+----------------------------------------------+\n";

    std::wcout << L"\nОберіть режим введення:\n"
        << L"  1 - Клавіатура\n"
        << L"  2 - Файл\n"
        << L"  3 - Генератор випадкових чисел\n"
        << L"  4 - Усі три режими (демо)\n"
        << L"Ваш вибір: ";

    int choice = 0;
    std::wcin >> choice;

    srand(static_cast<unsigned>(time(nullptr)));
    const std::string FILENAME = "students.txt";

    try {
        if (choice == 1) {
            // ── Клавіатура ──
            Student s = inputFromKeyboard();
            std::wcout << L"\n=== Результат ===\n" << s << L"\n";
            demonstrateCopyAndAssign(s);

        }
        else if (choice == 2) {
            // ── Файл ──
            createSampleFile(FILENAME);
            std::wcout << L"\n=== Зчитування з файлу ===\n";
            auto students = inputFromFile(FILENAME);
            for (size_t i = 0; i < students.size(); ++i)
                std::wcout << (i + 1) << L". " << students[i] << L"\n";
            if (!students.empty())
                demonstrateCopyAndAssign(students[0]);

        }
        else if (choice == 3) {
            // ── Генератор ──
            int n = 0;
            std::wcout << L"Скільки студентів згенерувати? ";
            std::wcin >> n;
            std::wcout << L"\n=== Згенеровані студенти ===\n";
            std::vector<Student> students;
            for (int i = 0; i < n; ++i) {
                students.push_back(inputFromRandom());
                std::wcout << (i + 1) << L". " << students.back() << L"\n";
            }
            if (!students.empty())
                demonstrateCopyAndAssign(students[0]);

        }
        else if (choice == 4) {
            // ── Усі три ──
            createSampleFile(FILENAME);
            std::wcout << L"\n=== [Файл] ===\n";
            for (auto& s : inputFromFile(FILENAME))
                std::wcout << s << L"\n";

            std::wcout << L"\n=== [Генератор] ===\n";
            for (int i = 0; i < 3; ++i)
                std::wcout << inputFromRandom() << L"\n";

            Student kbStudent = inputFromKeyboard();
            std::wcout << L"\n=== [Клавіатура] ===\n" << kbStudent << L"\n";
            demonstrateCopyAndAssign(kbStudent);

        }
        else {
            std::wcerr << L"Невідомий вибір.\n";
            return 1;
        }

    }
    catch (const std::exception& ex) {
        // narrow → wide для повідомлення про помилку
        std::string msg = ex.what();
        std::wcerr << L"Помилка: " << wstr(msg.begin(), msg.end()) << L"\n";
        return 1;
    }

    std::wcout << L"\n[Програму завершено]\n";
    _setmode(_fileno(stdin), _O_TEXT);   // повертаємо stdin перед system()
    system("pause");
    return 0;
}