#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <algorithm>

using namespace std;

// ��������� ��� �������� ������ � ����
struct Tour {
    string destination;
    int duration;
    double price;
    int groupSize;
};

// ���� ������ �����
struct TourNode {
    Tour* data;
    TourNode* next;

    // �����������
    TourNode(Tour* _data, TourNode* _next) : data(_data), next(_next) {}

    // ���������� ��� ������������ ������
    ~TourNode() {
        delete data;
    }
};

// �����, ����������� ������ �����
class TourList {
private:
    TourNode* head; // ��������� �� ������ ������ �����

public:
    // ����������� �� ���������
    TourList() : head(nullptr) {}

    // ���������� ��� ������������ ������
    ~TourList() {
        while (head != nullptr) {
            TourNode* temp = head;
            head = head->next;
            delete temp;
        }
    }

    // ���������� ���� � ������
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

    // �������� ���� �� ������ �������
    void removeTour(int position) {
        if (position < 0 || head == nullptr) {
            throw out_of_range("������� ��� ���������");
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
                    throw out_of_range("������� ��� ���������");
                }
                current = current->next;
            }
            TourNode* temp = current->next;
            current->next = current->next->next;
            delete temp;
        }
    }

    // ��������� �������� ���� �� ������ �������
    Tour getTour(int position) {
        if (position < 0 || head == nullptr) {
            throw out_of_range("������� ��� ���������");
        }
        TourNode* current = head;
        for (int i = 0; i < position; ++i) {
            if (current->next == nullptr) {
                throw out_of_range("������� ��� ���������");
            }
            current = current->next;
        }
        return *(current->data);
    }

    // �������� ������� ���� � ��������� ������������ �������
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

    // ��������� ������� �������� �����, ���� ������� �� ��������� ���������
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

    // ������ ������ � ����
    void writeToFile(const string& filename) {
        ofstream outFile(filename);
        if (!outFile.is_open()) {
            throw runtime_error("������ �������� ����� ��� ������");
        }

        TourNode* current = head;
        while (current != nullptr) {
            outFile << current->data->destination << " " << current->data->duration << " "
                << current->data->price << " " << current->data->groupSize << endl;
            current = current->next;
        }

        outFile.close();
    }

    // ������ ������ �� �����
    void readFromFile(const string& filename) {
        clear();

        ifstream inFile(filename);
        if (!inFile.is_open()) {
            throw runtime_error("������ �������� ����� ��� ������");
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

    // ������� ������ �����
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
        cout << "������� ��� ����� ��� ���������� ������: ";
        cin >> filename;

        TourList tourList;

        char choice;
        do {
            Tour tour;
            cout << "������� ������ � ����:" << endl;
            cout << "�����������: ";
            cin >> tour.destination;
            cout << "����������������� (� ����): ";
            cin >> tour.duration;
            cout << "����: ";
            cin >> tour.price;
            cout << "������ ������: ";
            cin >> tour.groupSize;

            tourList.addTour(tour);

            cout << "������ �������� ��� ���? (y/n): ";
            cin >> choice;
        } while (choice == 'y' || choice == 'Y');

        // ������ ������ � ����
        tourList.writeToFile(filename);

        // ����� ����������� �����
        cout << "����������� ����:" << endl;
        for (int i = 0; ; ++i) {
            try {
                Tour tour = tourList.getTour(i);
                cout << "�����������: " << tour.destination << ", �����������������: " << tour.duration
                    << " ����, ����: " << tour.price << ", ������ ������: " << tour.groupSize << endl;
            }
            catch (const out_of_range&) {
                break;
            }
        }

        // �������� ���� �� ������ �������
        int removePosition;
        cout << "������� ����� ������� ���� ��� ��������: ";
        cin >> removePosition;
        try {
            tourList.removeTour(removePosition);
            cout << "��� ������� ������." << endl;
        }
        catch (const out_of_range& e) {
            cerr << "������ �������� ����: " << e.what() << endl;
        }

        // ������ ���������� ������ � ����
        tourList.writeToFile(filename);

        // ������ ������ �� �����
        tourList.readFromFile(filename);

        // ��������� ������� �������� �����, ���� ������� �� ��������� ���������
        double maxPrice;
        cout << "������� ������������ ���� ��� ���������� �����: ";
        cin >> maxPrice;
        vector<string> filteredTours = tourList.getToursDescriptionsByMaxPrice(maxPrice);
        cout << "���� � ����� �� " << maxPrice << " ������:" << endl;
        for (const auto& description : filteredTours) {
            cout << description << endl;
        }

    }
    catch (const exception& e) {
        cerr << "��������� ����������: " << e.what() << endl;
    }

    system("Pause");
    return 0;
}
