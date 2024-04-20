#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <vector>

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
};

int main() {
    setlocale(LC_ALL, "Russian");
    try {
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

        // Получение значения тура по номеру позиции
        int getPosition;
        cout << "Введите номер позиции тура для получения данных: ";
        cin >> getPosition;
        try {
            Tour tour = tourList.getTour(getPosition);
            cout << "Данные тура на позиции " << getPosition << ":" << endl;
            cout << "Направление: " << tour.destination << ", Продолжительность: " << tour.duration
                << " дней, Цена: " << tour.price << ", Размер группы: " << tour.groupSize << endl;
        }
        catch (const out_of_range& e) {
            cerr << "Ошибка получения данных о туре: " << e.what() << endl;
        }

        // Проверка наличия тура с указанным направлением поездки
        string destinationCheck;
        cout << "Введите направление для проверки наличия тура: ";
        cin >> destinationCheck;
        if (tourList.findTourByDestination(destinationCheck)) {
            cout << "Тур с указанным направлением найден." << endl;
        }
        else {
            cout << "Тур с указанным направлением не найден." << endl;
        }

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

    return 0;
}
