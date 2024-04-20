#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <vector>

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
};

int main() {
    setlocale(LC_ALL, "Russian");
    try {
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

        // ��������� �������� ���� �� ������ �������
        int getPosition;
        cout << "������� ����� ������� ���� ��� ��������� ������: ";
        cin >> getPosition;
        try {
            Tour tour = tourList.getTour(getPosition);
            cout << "������ ���� �� ������� " << getPosition << ":" << endl;
            cout << "�����������: " << tour.destination << ", �����������������: " << tour.duration
                << " ����, ����: " << tour.price << ", ������ ������: " << tour.groupSize << endl;
        }
        catch (const out_of_range& e) {
            cerr << "������ ��������� ������ � ����: " << e.what() << endl;
        }

        // �������� ������� ���� � ��������� ������������ �������
        string destinationCheck;
        cout << "������� ����������� ��� �������� ������� ����: ";
        cin >> destinationCheck;
        if (tourList.findTourByDestination(destinationCheck)) {
            cout << "��� � ��������� ������������ ������." << endl;
        }
        else {
            cout << "��� � ��������� ������������ �� ������." << endl;
        }

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

    return 0;
}
