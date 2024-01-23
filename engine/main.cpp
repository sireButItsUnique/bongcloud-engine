#include "Board.hpp"

#define ll long long
using namespace std;

int main() {
	string input;
	Board *board = new Board();
	board->print(false);

	while (input != "quit") {
		cin >> input;
		board->movePiece(input);
		board->print(false);
	}
	return 0;
}