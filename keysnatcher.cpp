#include <iostream> // for cout (print to console)

int main() {
	system("stty -icanon"); // enable special charecter reading
	while (true) {
		std::cout << getchar();
	}
	return 0;
}
