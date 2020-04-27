#include <iostream>
#include <string>

void mainMenu();
void printCredits();

bool running;

int main() {

    running = true;

    while(running){
        mainMenu();
    }

    return EXIT_SUCCESS;

}

void mainMenu() {

	int choice = 0;
	
	std::cout << "*** Welcome to 🅰 🆉 🆄 🅻 ***" << std::endl;
	std::cout << "(1): Credits" << std::endl;
    std::cout << "(2): Exit without Saving" << std::endl;
	std::cin >> choice;
	
	switch(choice)
	{
	case 1:
		printCredits();
		break;
    case 2:
        running = false;
        std::cout << "Goodbye." << std::endl;
        break;
	default:
		std::cout << "Invalid choice." << std::endl;
		break;
	}

    return;

}

void printCredits() {

    std::cout << "🅰 🆉 🆄 🅻" << std::endl;
    std::cout << "Created by:" << std::endl
    << "s3687337 Joshua Monaghan-Landy" << std::endl
    << "s3787473 Joel Whitley" << std::endl
    << "s3837218 Dinesh Premanath Amarakone Urulugastenne Mudiyanselage" << std::endl 
    << "s3658845 Wen Koay" << std::endl;

    return;

}