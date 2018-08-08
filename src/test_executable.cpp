#include <Windows.h>
#include <cpp-utils/process/daemonize.h>
#include <thread>
#include <iostream>

using namespace std;

int main() {
	std::cout << "start" << std::endl;
	cpputils::daemonize();
	std::cout << "daemonized" << std::endl;
	while (true) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::cout << "beep" << std::endl;
		Beep(750, 300);
	}
}
