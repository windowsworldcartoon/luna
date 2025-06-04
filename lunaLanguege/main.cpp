#include <iostream>
#include <string>


// luna terminal
class LunaTerminal {
	public:
	void run() {
		std::string command;
		std::cout << "Luna Terminal - Type 'exit' to quit." << std::endl;
		while (true) {
			std::cout << "> ";
			std::getline(std::cin, command);
			if (command == "exit") {
				break;
			}
			processCommand(command);
		}
	}

private:
	void processCommand(const std::string& command) {
		if (command == "hello") {
			std::cout << "Hello, Luna!" << std::endl;
		}
		else if (command == "help") {
			std::cout << "Available commands: hello, help, exit" << std::endl;
		} else if (command == "run") {


		} else {
			std::cout << "Unknown command: " << command << std::endl;
		}
	}
};
