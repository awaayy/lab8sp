#include <iostream>
#include <vector>
#include <stdexcept>
#include <algorithm>

using namespace std;

// ��������� ��� �������� ������ � ����
struct Tour {
    string destination;
    int duration;
    double price;
    int groupSize;
};

// �����, ����������� ������ �����
class TourList {
private:
    vector<Tour> tours;

public:
    //���������� ���� � ������
    void addTour(const Tour& tour) {
        tours.push_back(tour);
        //���������� ������ �� ���� ����� ���������� ������ ��������
        sort(tours.begin(), tours.end(), [](const Tour& a, const Tour& b) {
            return a.price < b.price;
            });
    }

    //�������� ���� �� ������ �������
    void removeTour(int position) {
        if (position < 0 || position >= tours.size()) {
            throw out_of_range("������� ��� ���������");
        }
        tours.erase(tours.begin() + position);
    }

    //��������� �������� ���� �� ������ �������
    Tour getTour(int position) {
        if (position < 0 || position >= tours.size()) {
            throw out_of_range("������� ��� ���������");
        }
        return tours[position];
    }

    //�������� ������� ���� � ��������� ������������ �������
    vector<int> findToursByDestination(const string& destination) {
        vector<int> positions;
        for (size_t i = 0; i < tours.size(); ++i) {
            if (tours[i].destination == destination) {
                positions.push_back(i);
            }
        }
        return positions;
    }

    //��������� ������� �������� �����, ���� ������� �� ��������� ���������
    vector<string> getToursDescriptionsByMaxPrice(double maxPrice) {
        vector<string> descriptions;
        for (const auto& tour : tours) {
            if (tour.price <= maxPrice) {
                descriptions.push_back(tour.destination);
            }
        }
        return descriptions;
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    try {
        TourList tourList;

        //���������� �����
        tourList.addTour({ "�����", 5, 1000.0, 20 });
        tourList.addTour({ "������", 7, 1500.0, 15 });
        tourList.addTour({ "���", 3, 800.0, 25 });
        tourList.addTour({ "�������", 2, 700.0, 7 });

        //�������� ����
        tourList.removeTour(1);

        //������ ��������� ���� �� ������ �������
        Tour tour = tourList.getTour(0);
        cout << "����������� ����: " << tour.destination << endl;

        //������ ������ ���� �� �����������
        vector<int> positions = tourList.findToursByDestination("�����");
        cout << "���� � ������������ � ����� ������� �� ��������: ";
        for (int pos : positions) {
            cout << pos << " ";
        }
        cout << endl;

        //������ ��������� ����� �� ������������ ����
        vector<string> descriptions = tourList.getToursDescriptionsByMaxPrice(1000.0);
        cout << "���� � ����� <= 1000.0:" << endl;
        for (const auto& desc : descriptions) {
            cout << desc << ", ";
        }
        cout << endl;

    }
    catch (const exception& e) {
        cerr << "��������� ����������: " << e.what() << endl;
    }

    return 0;
}
