#include "functions.hpp"
#include <iostream>

int main() {
    Massiv array{};
    std::string TOMLarray = "";

    try {
        int operation_index = getOperation();

        if (operation_index == 1) {
            getStandartArray(array);
            std::string toml = standartToTOML(array);
            std::cout << "\n" << toml << std::endl;
        }

        else if (operation_index == 2) {
            getTOMLarray(TOMLarray);
            std::string classicArray = TOMLtoStandart(TOMLarray);
            std::cout << "\n" << classicArray << std::endl;
        }

        else if (operation_index == 3) {
            std::cout << "\n" << "Aborting the programm..." << std::endl;
        }
    }

    catch (const std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
    }

    catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }

    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
