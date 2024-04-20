#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <algorithm>

using namespace std;

// Структура для хранения данных о туре
struct Tour {
    string destination;
    int duration;
    double price;
    int groupSize;
};

// Узел списка туров
struct TourNode {
    Tour* data;
    TourNode* next;

    // Конструктор
    TourNode(Tour* _data, TourNode* _next) : data(_data), next(_next) {}

    // Деструктор для освобождения памяти
    ~TourNode() {
        delete data;
    }
};

// Класс, реализующий список туров
class TourList {
private:
    TourNode* head; // Указатель на начало списка туров

public:
    // Конструктор по умолчанию
    TourList() : head(nullptr) {}

    // Деструктор для освобождения памяти
    ~TourList() {
        while (head != nullptr) {
            TourNode* temp = head;
            head = head->next;
            delete temp;
        }
    }

    // Добавление тура в список
    void addTour(const Tour& tour) {
        Tour* newTour = new Tour(tour);
        TourNode* newNode = new TourNode(newTour, nullptr);
        if (head == nullptr) {
            head = newNode;
        }
        else {
            TourNode* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newNode;
        }
    }

    // Удаление тура по номеру позиции
    void removeTour(int position) {
        if (position < 0 || head == nullptr) {
            throw out_of_range("Позиция вне диапазона");
        }
        if (position == 0) {
            TourNode* temp = head;
            head = head->next;
            delete temp;
        }
        else {
            TourNode* current = head;
            for (int i = 0; i < position - 1; ++i) {
                if (current->next == nullptr) {
                    throw out_of_range("Позиция вне диапазона");
                }
                current = current->next;
            }
            TourNode* temp = current->next;
            current->next = current->next->next;
            delete temp;
        }
    }

    // Получение значения тура по номеру позиции
    Tour getTour(int position) {
        if (position < 0 || head == nullptr) {
            throw out_of_range("Позиция вне диапазона");
        }
        TourNode* current = head;
        for (int i = 0; i < position; ++i) {
            if (current->next == nullptr) {
                throw out_of_range("Позиция вне диапазона");
            }
            current = current->next;
        }
        return *(current->data);
    }

    // Проверка наличия тура с указанным направлением поездки
    bool findTourByDestination(const string& destination) {
        TourNode* current = head;
        while (current != nullptr) {
            if (current->data->destination == destination) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    // Получение массива описаний туров, цена которых не превышает указанную
    vector<string> getToursDescriptionsByMaxPrice(double maxPrice) {
        vector<string> descriptions;
        TourNode* current = head;
        while (current != nullptr) {
            if (current->data->price <= maxPrice) {
                descriptions.push_back(current->data->destination);
            }
            current = current->next;
        }
        return descriptions;
    }

    // Запись данных в файл
    void writeToFile(const string& filename) {
        ofstream outFile(filename);
        if (!outFile.is_open()) {
            throw runtime_error("Ошибка открытия файла для записи");
        }

        TourNode* current = head;
        while (current != nullptr) {
            outFile << current->data->destination << " " << current->data->duration << " "
                << current->data->price << " " << current->data->groupSize << endl;
            current = current->next;
        }

        outFile.close();
    }

    // Чтение данных из файла
    void readFromFile(const string& filename) {
        clear();

        ifstream inFile(filename);
        if (!inFile.is_open()) {
            throw runtime_error("Ошибка открытия файла для чтения");
        }

        string destination;
        int duration;
        double price;
        int groupSize;
        while (inFile >> destination >> duration >> price >> groupSize) {
            Tour tour = { destination, duration, price, groupSize };
            addTour(tour);
        }

        inFile.close();
    }

    // Очистка списка туров
    void clear() {
        while (head != nullptr) {
            TourNode* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    try {
        string filename;
        cout << "Введите имя файла для сохранения данных: ";
        cin >> filename;

        TourList tourList;

        char choice;
        do {
            Tour tour;
            cout << "Введите данные о туре:" << endl;
            cout << "Направление: ";
            cin >> tour.destination;
            cout << "Продолжительность (в днях): ";
            cin >> tour.duration;
            cout << "Цена: ";
            cin >> tour.price;
            cout << "Размер группы: ";
            cin >> tour.groupSize;

            tourList.addTour(tour);

            cout << "Хотите добавить ещё тур? (y/n): ";
            cin >> choice;
        } while (choice == 'y' || choice == 'Y');

        // Запись данных в файл
        tourList.writeToFile(filename);

        // Вывод добавленных туров
        cout << "Добавленные туры:" << endl;
        for (int i = 0; ; ++i) {
            try {
                Tour tour = tourList.getTour(i);
                cout << "Направление: " << tour.destination << ", Продолжительность: " << tour.duration
                    << " дней, Цена: " << tour.price << ", Размер группы: " << tour.groupSize << endl;
            }
            catch (const out_of_range&) {
                break;
            }
        }

        // Удаление тура по номеру позиции
        int removePosition;
        cout << "Введите номер позиции тура для удаления: ";
        cin >> removePosition;
        try {
            tourList.removeTour(removePosition);
            cout << "Тур успешно удален." << endl;
        }
        catch (const out_of_range& e) {
            cerr << "Ошибка удаления тура: " << e.what() << endl;
        }

        // Запись измененных данных в файл
        tourList.writeToFile(filename);

        // Чтение данных из файла
        tourList.readFromFile(filename);

        // Получение массива описаний туров, цена которых не превышает указанную
        double maxPrice;
        cout << "Введите максимальную цену для фильтрации туров: ";
        cin >> maxPrice;
        vector<string> filteredTours = tourList.getToursDescriptionsByMaxPrice(maxPrice);
        cout << "Туры с ценой до " << maxPrice << " рублей:" << endl;
        for (const auto& description : filteredTours) {
            cout << description << endl;
        }

    }
    catch (const exception& e) {
        cerr << "Произошло исключение: " << e.what() << endl;
    }

    system("Pause");
    return 0;
}
