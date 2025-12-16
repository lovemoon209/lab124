#include <cstring>
#include <fstream>
#include <iostream>
using namespace std;

struct Note {
  char surname[32];
  char name[32];
  char phone[16];
  int birthday[3];
};

Note inputNote() {
  Note n;
  cout << "Введіть прізвище: ";
  cin >> n.surname;
  cout << "Введіть ім'я: ";
  cin >> n.name;
  cout << "Введіть номер телефону: ";
  cin >> n.phone;
  cout << "Введіть день народження (день місяць рік): ";
  cin >> n.birthday[0] >> n.birthday[1] >> n.birthday[2];
  return n;
}

void addNoteToFile(const char *filename) {
  ofstream fout(filename, ios::binary | ios::app);
  if (!fout) {
    cout << "Помилка відкриття файлу для запису.\n";
    return;
  }
  Note n = inputNote();
  fout.write((char *)&n, sizeof(Note));
  fout.close();
}

void printAllNotes(const char *filename) {
  ifstream fin(filename, ios::binary);
  if (!fin) {
    cout << "Файл не знайдено.\n";
    return;
  }
  Note n;
  cout << "Список записів:\n";
  while (fin.read((char *)&n, sizeof(Note))) {
    cout << n.surname << " " << n.name << ", тел.: " << n.phone
         << ", ДН: " << n.birthday[0] << "." << n.birthday[1] << "."
         << n.birthday[2] << endl;
  }
  fin.close();
}

void sortFileByPhone(const char *filename) {
  fstream file(filename, ios::binary | ios::in | ios::out);
  if (!file) {
    cout << "Файл не знайдено.\n";
    return;
  }
  file.seekg(0, ios::end);
  int count = file.tellg() / sizeof(Note);

  for (int i = 0; i < count - 1; i++) {
    for (int j = 0; j < count - i - 1; j++) {
      Note a, b;
      file.seekg(j * sizeof(Note), ios::beg);
      file.read((char *)&a, sizeof(Note));
      file.read((char *)&b, sizeof(Note));
      if (strcmp(a.phone, b.phone) > 0) {
        file.seekp(j * sizeof(Note), ios::beg);
        file.write((char *)&b, sizeof(Note));
        file.write((char *)&a, sizeof(Note));
      }
    }
  }
  file.close();
  cout << "Файл відсортовано за номерами телефонів.\n";
}

void findBySurname(const char *filename, const char *surname) {
  ifstream fin(filename, ios::binary);
  if (!fin) {
    cout << "Файл не знайдено.\n";
    return;
  }
  Note n;
  bool found = false;
  while (fin.read((char *)&n, sizeof(Note))) {
    if (strcmp(n.surname, surname) == 0) {
      cout << "Знайдено: " << n.surname << " " << n.name
           << ", тел.: " << n.phone << ", ДН: " << n.birthday[0] << "."
           << n.birthday[1] << "." << n.birthday[2] << endl;
      found = true;
      break;
    }
  }
  if (!found)
    cout << "Запис із прізвищем \"" << surname << "\" не знайдено.\n";
  fin.close();
}

void menu(const char *filename) {
  int choice;
  do {
    cout << "\nМеню:\n";
    cout << "1. Додати запис\n";
    cout << "2. Вивести всі записи\n";
    cout << "3. Відсортувати за телефоном\n";
    cout << "4. Знайти за прізвищем\n";
    cout << "0. Вихід\n";
    cout << "Ваш вибір: ";
    cin >> choice;

    if (cin.fail()) {
      cin.clear();
      cin.ignore(1000, '\n');
      cout << "Помилка вводу! Спробуйте ще раз.\n";
      continue;
    }

    switch (choice) {
    case 1:
      addNoteToFile(filename);
      break;
    case 2:
      printAllNotes(filename);
      break;
    case 3:
      sortFileByPhone(filename);
      break;
    case 4: {
      char surname[32];
      cout << "Введіть прізвище для пошуку: ";
      cin >> surname;
      findBySurname(filename, surname);
      break;
    }
    case 0:
      cout << "Вихід з програми.\n";
      break;
    default:
      cout << "Невірний вибір!\n";
    }
  } while (choice != 0);
}

int main() {
  char filename[64];
  cout << "Введіть ім'я файлу: ";
  cin >> filename;
  menu(filename);
  return 0;
}
