#include "cryptography.hpp"

constexpr uint8 CPP = 2;
typedef vector<vector<lint>> matrix;

const map<lint, string> pack = {
	{2, "2S"}, {3, "2H"}, {4, "2D"}, {5, "2C"},
	{6, "3S"}, {7, "3H"}, {8, "3D"}, {9, "3C"},
	{10, "4S"}, {11, "4H"}, {12, "4D"}, {13, "4C"},
	{14, "5S"}, {15, "5H"}, {16, "5D"}, {17, "5C"},
	{18, "6S"}, {19, "6H"}, {20, "6D"}, {21, "6C"},
	{22, "7S"}, {23, "7H"}, {24, "7D"}, {25, "7C"},
	{26, "8S"}, {27, "8H"}, {28, "8D"}, {29, "8C"},
	{30, "9S"}, {31, "9H"}, {32, "9D"}, {33, "9C"},
	{34, "10S"}, {35, "10H"}, {36, "10D"}, {37, "10C"},
	{38, "JS"}, {39, "JH"}, {40, "JD"}, {41, "JC"},
	{42, "QS"}, {43, "QH"}, {44, "QD"}, {45, "QC"},
	{46, "KS"}, {47, "KH"}, {48, "KD"}, {49, "KC"},
	{50, "AS"}, {51, "AH"}, {52, "AD"}, {53, "AC"}
};

void mental_pocker(const uint8 players)
{
	bool is_put[52] = {};

	matrix Cards;
	Cards.resize(players);

	unique_ptr<lint[]> C(new lint [players]);
	unique_ptr<lint[]> D(new lint [players]);
	lint p = gen_p();

	for(uint8 i = 0; i < players; ++i) {
		Cards.push_back(vector<lint>());

		C[i] = gen_coprime(p - 1);
		D[i] = mod_inv(C[i], p - 1);
	}

	matrix U;
	U.resize(players + 1);

	for(auto it = pack.begin(); it != pack.end(); ++it) {
		U[0].push_back(it->first);
	}

	for(uint8 i = 1; i <= players; ++i) {
		for(uint8 j = 0; j < 52; ++j) {
			U[i].push_back(mod_pow(U[i - 1][j], C[i - 1], p));
		}
		reverse_cards(U[i]);
	}

	for(uint8 i = 0, j = 0; i < players; ++i, j += 2) {
		for(uint8 k = j, l = 0; l < CPP; ++k, ++l) {
			Cards[i].push_back(U[players][k]);
		}
	}

	for(uint8 i = 0; i < players; ++i) {
		for(uint8 k = 0; k < CPP; ++k) {
			for(uint8 j = 0; j < players; ++j) {
				if(j != i) Cards[i][k] = mod_pow(Cards[i][k], D[j], p);
			}

			Cards[i][k] = mod_pow(Cards[i][k], D[i], p);
			is_put[(uint64)Cards[i][k] - 2] = true;
		}
	}

	print_cards(Cards, players, is_put);
}

void reverse_cards(vector<lint> &U)
{
	array<bool, 52> is_used = {};
	lint buffer;

	for(uint8 i = 0, j; i < 52; ++i) {
		do {
			j = rand() % 52;
		} while(is_used[j]);
		is_used[j] = true;

		buffer = U[i];
		U[i] = U[j];
		U[j] = buffer;
	}
}

void print_cards(const matrix Cards, const uint8 players, bool is_put[52])
{
	for(uint8 i = 0; i < players; ++i) {
		cout << "Игрок " << i + 1 << ": ";
		for(uint8 j = 0; j < CPP; ++j) {
			cout << "[" << pack.find(Cards[i][j])->second << "]";
		}
		cout << endl;
	}

	cout << "\nОставшиеся карты: \n";
	for(uint8 i = 0; i < 52; ++i) {
		if(!is_put[i]) cout << "[" << pack.find(i + 2)->second << "], ";
	}
	cout << endl;
}
