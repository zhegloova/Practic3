#include <iostream>
#include <string>
using namespace std;

class AbonementTable {
private:

	struct Abonement {
		int ID = -1;
		string name;
		string adress;
	};
	int TABLE_SIZE; // длина таблицы
	int cur_size; // текущий размер таблицы
	int PRIME = 7; // вспомогательная переменная для вычисления хэша
	Abonement* array; //  таблица
	int h1(int key) {
		return key % TABLE_SIZE;
	}

	int h2(int key) {
		return (PRIME - (key % PRIME));
	}

	int dh(int index, int key) {
		return (h1(key) + index * h2(key)) % TABLE_SIZE;
	}

	// проверка занята ли данная ячейка таблицы
	bool isFill(int hash) {
		if (array[hash].ID == -1) {
			return false;
		}
		else
			return true;

	}

	bool isFull() {
		return (TABLE_SIZE == cur_size);
	}
	void resize() {
		// копируем данные текущего массива во временный массив
		Abonement* t = new Abonement[TABLE_SIZE * 2];
		for (int i = 0; i < TABLE_SIZE; i++) {
			t[i] = array[i];
		}
		// увлеичиваем длину
		TABLE_SIZE *= 2;
		cur_size = 0;
		// удаляем массив и инициализируем по новой
		delete[] array;
		array = new Abonement[TABLE_SIZE];
		for (int i = 0; i < TABLE_SIZE; i++) {
			Abonement item;
			array[i] = item;
		}
		// вносим старые данные в массив
		cout << endl << "Rehashing: " << endl << endl;
		for (int i = 0; i < TABLE_SIZE / 2; i++) {
			addItem(t[i].ID, t[i].name, t[i].adress);
		}
		cout << endl << "End of rehashing. " << endl << endl;
		delete[] t;
	}

public:
	AbonementTable(int size) {
		this->TABLE_SIZE = size;
		this->array = new Abonement[size];
		this->cur_size = 0;
		// заполняем таблицу пустыми элементами типа Student
		for (int i = 0; i < size; i++) {
			Abonement item;
			this->array[i] = item;
		}
	};

	~AbonementTable() {
		delete[] array;
	}

	bool addItem(int ID, string name, string adress) {
		// заполнена ли таблица
		if (isFull()) {
			//увеличиваем размер
			resize();
			cout << "Resize:  new size is " << TABLE_SIZE << endl;
		}
		Abonement item;
		item.name = name;
		item.adress = adress;
		item.ID = ID;

		// получаем первый хэш
		int index = h1(ID);

		// если возникла коллизия
		if (isFill(index)) {

			int i = 1;
			while (1) {
				// получаем новый хэш
				int newIndex = dh(i, ID);

				// если коллизий не возникло вставляем элемент
				if (!isFill(newIndex)) {
					this->array[newIndex] = item;
					cout << "Item added: " << newIndex << endl;
					break;
				}
				i++;
			}
		}
		// если коллизий не возникло
		else {
			cout << "Item added: " << index << endl;
			this->array[index] = item;
		}
		cur_size++;
		return true;
	}

	int search(int key) {
		int index = h1(key);
		// если элемент найден
		if (isFill(index) && this->array[index].ID == key) {
			cout << array[index].ID << " " << array[index].name << " " << array[index].adress << endl;
			return index;
			// если элемент в хэш-таблице найден но не с заданным ключом
		}
		else if (isFill(index) && this->array[index].ID != key) {
			int i = 1;
			while (i < 20) {
				int newIndex = dh(i, key);
				if (isFill(newIndex) && this->array[newIndex].ID == key) {
					cout << array[newIndex].ID << " " << array[newIndex].name << " " << array[newIndex].adress << endl;
					return newIndex;
				}
				i++;
			}
			cout << "Item not found" << endl;
			return -1;
			// данного ключа не существует в таблице
		}
		else {
			cout << "Item not found" << endl;
			return -1;
		}
	}

	bool deleteItem(int key) {
		Abonement item;
		int index = h1(key);
		// если это именно тот элемент который мы ищем
		if (isFill(index) && this->array[index].ID == key) {
			item.ID = 0;
			this->array[index] = item;
			cout << "Item is deleted" << endl;
			return true;
			// если элемент найден но не с заданным ключом
		}
		else if (isFill(index) && this->array[index].ID != key) {
			int i = 1;
			while (i < TABLE_SIZE) {
				int newIndex = dh(i, key);
				if (isFill(newIndex) && this->array[newIndex].ID == key) {
					this->array[newIndex] = item;
					return true;
				}
				i++;
			}
			cout << "Item not found" << endl;
			return false;
			// данного ключа не существует в таблице
		}
		else {
			cout << "Item not found" << endl;
			return false;
		}
	}

	void printTable() {
		if (this->TABLE_SIZE != 0) {
			for (int i = 0; i < this->TABLE_SIZE; i++) {
				if (array[i].ID != -1) {
					cout << i << " -> " << array[i].ID << " " << array[i].name << " " << array[i].adress << endl;
				}
			}
		}
	}

};

int main() {
	AbonementTable a(5);
	a.addItem(12345, "Ivanov Ivan Ivanovich", "Moscow, Mira, 1");
	a.addItem(45123, "Petrov Petr", "Perm, Lenina, 17");
	a.addItem(54145, "White Ann", "Moscow");
	a.addItem(98680, "Sidorova Maria Ivanovna", "Ufa, 1-st str, 23");
	a.addItem(55454, "Black Jhone", "UK, London");
	int func = -1;
	while (func != 0) {
		cout << endl << "Enter number of function or '0' for exit:" << endl;
		cout << "<1> Print table" << endl;
		cout << "<2> Add new reader" << endl;
		cout << "<3> Delete reader by ID" << endl;
		cout << "<4> Find reader by ID" << endl;
		cin >> func;
		switch (func)
		{
		case 1:
		{
			a.printTable();
			break;
		}
		case 2:
		{
			cout << "Enter ID" << endl;
			int ID;
			cin >> ID;
			cin.get();
			cout << "Enter name" << endl;
			string name;
			getline(cin, name);
			cout << "Enter adress" << endl;
			string adress;
			getline(cin, adress);
			a.addItem(ID, name, adress);
			break;
		}
		case 3:
		{
			cout << "Enter ID to delete reader" << endl;
			int ID;
			cin >> ID;
			a.deleteItem(ID);
			break;
		}
		case 4:
		{
			cout << "Enter id to search reader" << endl;
			int ID;
			cin >> ID;
			a.search(ID);
			break;
		}
		default:
			cout << "Reenter function" << endl;
			break;
		}
	}

	return 0;
}