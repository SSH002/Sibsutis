#include <string>
#include <iostream>
#include "cryptography.hpp"

int main()
{
	srand(time(0));

	unique_ptr<server> serv(new server());
	const lint n = serv->send_n();
	const lint d = serv->send_d();

	uint64 count = 2 + rand() % 9;
	cout << "Количество избирателей: " << count << endl;

	vector<constituent> cons;

	for(uint64 i = 0; i < count; ++i) {
		cons.push_back(constituent(n, d));
	}

	for(uint64 j = 0; j < 2; ++j) {										//Количество попыток голосования
		cout << "\nГолосование " << j + 1 << endl << endl;

		for(uint64 i = 0; i < count; ++i) {
			lint h1 = cons[i].send_h1();

			if(!serv->check_registr(h1)) {
				cons[i].create_s(serv->create_bulletin(h1), n);

				if(serv->check_bulletin(cons[i].send_s(), cons[i].send_m())) {
					cout << "Бюллетень избирателя " << i + 1 << " действителен.\n";
				} else {
					cout << "Ошибка! Бюллетень избирателя " << i + 1 << " НЕ действителен.\n";
				}

			} else {
				cout << "Ошибка! Бюллетень избирателя " << i + 1 << " был отправлен ранее.\n";
			}
		}
	}

	serv->print_results();

	return 0;
}
