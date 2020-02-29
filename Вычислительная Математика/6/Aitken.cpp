#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
using namespace std;

int main()
{
    unsigned int i = 0, j, n = 1;
    const char *file = "Data.txt";
    char symb;
    char *buffer = new char[100];
    setlocale(0, "");
    ifstream f(file);

    f.getline(buffer, 100, '\n');
    f.close();
	
    //Вычисляем размер массивов
    while (buffer[i] != '\0') {
        if (buffer[i] == ' ') {
            ++n;
        }
        ++i;
    }

    unsigned int m;
    for (i = 0, m = 0; i < n; ++i) {
        m += n - i;
    }

    delete[] buffer;
    double *X = new double[n];
    double *Y = new double[m];
    double xx;

    //Считываем входные данные
    ifstream f1(file);
    while (!f1.eof()) {
        for (i = 0; i < n; ++i) {
            f1 >> X[i];
        }
        for (i = 0; i < n; ++i) {
            f1 >> Y[i];
        }
        f1 >> xx;
    } f1.close();
	
    for (i = 0; i < n; ++i) {
        cout << "X[" << i + 1 << "] = " << X[i] << ", Y[" << i + 1 << "] = " << Y[i] << endl;
    }

    unsigned int tmp = n;
	
    i = 0;
    for ( ; i < n - 1; ++i) {
        for (j = 0; j < n - i - 1; ++j, ++tmp) {
            Y[tmp] = (Y[tmp - n + i] * (xx - X[i + j + 1]) - Y[tmp - n + i + 1] * (xx - X[j])) / (X[j] - X[i + j + 1]);
            cout << "\nP[" << tmp + 1 << "] = " << Y[tmp];
        }
    }

    cout << "\n\nОтвет: " << Y[tmp - 1] << endl;

    system("pause");
    return 0;
}
