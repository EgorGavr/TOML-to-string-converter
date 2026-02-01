#include "functions.hpp"
#include <iostream>
#include <sstream>

int getOperation() {
    int operation_index;
    std::cout
        << "Choose an operation:" << std::endl
        << "1: Convert basic array into TOML format" << std::endl
        << "2: Convert an array from TOML format into basic array" << std::endl
        << "3: Exit" << std::endl;
    std::cin >> operation_index;
    if (!(operation_index)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        throw std::runtime_error("Not a number!");
    }
    std::cin.ignore();

    if (operation_index < 1 || operation_index > 3) {
        throw std::out_of_range("Invalid index!");
    }

    return operation_index;
}

void getStandartArray(Massiv& massiv) {
    std::string standartArray;
    std::string elements;

    std::cout
        << "\n" << "Enter the array in the format : " << "\n" << std::endl
        << "ArrayName = elem1, elem2, ... , elemN" << "\n" << std::endl;

    std::getline(std::cin, standartArray);
    if (standartArray.empty()) {
        throw std::runtime_error("Error with reading array!");
    }

    size_t len = standartArray.size();
    auto equal = standartArray.find_first_of("=");
    if (equal == std::string::npos) {
        throw std::invalid_argument("'=' is missing!");
    }

    massiv.name = standartArray.substr(0, equal - 1);
    if (massiv.name.empty()) {
        throw std::invalid_argument("Array name is empty");
    }

    elements = standartArray.substr(equal + 2, len - (equal + 2));
    if (elements.empty()) {
        throw std::invalid_argument("There is no elements after '='!");
    }

    elements += ",";
    size_t lenElem = elements.size();
    int element = 0;
    std::string temp = "";
    for (size_t i = 0; i < lenElem; i += 3) {
        temp = elements[i];
        while (elements[i + 1] != ',') {
            i++;
            temp += elements[i];
        }
        if (temp.empty()) {
            throw std::invalid_argument("There is an empty element!");
        }
        try {
            element = stoi(temp);
            massiv.elements.push_back(element);
        }
        catch (const std::exception&) {
            throw std::invalid_argument("Element is not an integer!");
        }
    }
    if (massiv.elements.empty()) {
        throw std::runtime_error("No valid elements parsed!");
    }
}


std::string standartToTOML(const Massiv& massiv) {
    std::string massivTOML = std::format("[{}]", massiv.name);
    size_t len = massiv.elements.size();
    for (size_t i = 0; i < len; ++i) {
        massivTOML += std::format("\n{} = {}", i + 1, massiv.elements[i]);
    }
    return massivTOML;
}


void getTOMLarray(std::string& TOMLarray) {
    std::string temp = "";

    std::cout
        << "\n" << "Enter the array in the format:" << "\n" << std::endl
        << "[ArrayName]" << std::endl
        << "1 = elem1" << std::endl
        << "2 = elem2" << std::endl
        << "......." << std::endl
        << "n = elemN" << std::endl
        << "end" << std::endl
        << "(Put \'end\' in the end of array)" << "\n" << std::endl;

    std::getline(std::cin, TOMLarray);
    if (TOMLarray.size() < 2 || TOMLarray.front() != '[' || TOMLarray.back() != ']') {
        throw std::invalid_argument("TOML must start with [Name]!");
    }

    std::getline(std::cin, temp);
    while (temp != "end") {
        size_t eq = temp.find('=');
        if (eq == std::string::npos) {
            throw std::invalid_argument("This string should contain '='!");
        }
        TOMLarray += "\n" + temp;
        std::getline(std::cin, temp);
    }
}


std::string TOMLtoStandart(const std::string& TOMLarray) {
    std::string massiv = "";
    int schet = 0;
    
    if (TOMLarray.empty()) {
        throw std::invalid_argument("TOML string is empty");
    }
    
    std::istringstream potok(TOMLarray);
    std::string stroka;

    if (!std::getline(potok, stroka)) {
        throw std::runtime_error("Failed to read TOML header");
    }
    
    massiv += stroka.substr(1, stroka.size() - 2);

    if (massiv.empty()) {
        throw std::invalid_argument("Array name is empty");
    }

    massiv += " = ";

    while (std::getline(potok, stroka)) {

        if (stroka == "end") break;

        if (stroka.empty()) {
            throw std::invalid_argument("Empty line inside TOML array");
        }

        auto ravno = std::find(stroka.begin(), stroka.end(), '=');
        auto elemStart = std::next(ravno) + 1;
        auto elemEnd = stroka.end();
        if (elemEnd == elemStart) {
            throw std::invalid_argument("There is an empty element!");
        }

        int element = 0;
        const char* first = &(*elemStart);
        const char* last = first + std::distance(elemStart, elemEnd);

        std::from_chars(first, last, element);

        if (schet != 0) {
            massiv += std::format(", {}", element);
            schet++;
        }
        else {
            massiv += std::format("{}", element);
            schet++;
        }
        
    }

    if (schet == 0) {
        throw std::runtime_error("No valid elements parsed!");
    }

    return massiv;

}



//ПЕРЕГРУЗКИ

std::string standartToTOML(const std::string& name, const std::vector<int>& arr) {
    if (name.empty()) {
        throw std::invalid_argument("Array name is empty");
    }

    std::string massivTOML = std::format("[{}]", name);
    size_t i;
    for (i = 0; i < arr.size(); i++) {
        massivTOML += std::format("\n{} = {}", i + 1, arr[i]);
    }
    return massivTOML;
}

const char* standartToTOML(const char* name, const int* arr, std::size_t alen) {

    if (name == nullptr) {
        throw std::invalid_argument("Name is empty");
    }
    if (arr == nullptr && alen != 0) {
        throw std::invalid_argument("Array is empty");
    }

    std::string massivName(name);

    std::vector<int> elements;
    std::size_t i;
    for (i = 0; i < alen; i++) {
        elements.push_back(arr[i]);
    }

    std::string result = standartToTOML(massivName, elements);

    static std::string answer; //Чтобы указатель остался после выхода из функции и в мусор не превратился
    answer = result;

    return answer.c_str();
}


const char* standartToTOML(const char* name, std::size_t slen, const int* arr, std::size_t alen) {
    if (name == nullptr && slen != 0) {
        throw std::invalid_argument("Name is empty (Null)");
    }
    if (arr == nullptr && alen != 0) {
        throw std::invalid_argument("Array is empty (Null)");
    }

    std::string arrayName(name, slen);
    if (arrayName.empty()) {
        throw std::invalid_argument("Name is empty");
    }

    std::vector<int> elements;
    std::size_t i;
    for (i = 0; i < alen; i++) {
        elements.push_back(arr[i]);
    }

    std::string result = standartToTOML(arrayName, elements);

    static std::string answer;
    answer = result;
    return answer.c_str();

}


const char* TOMLtoStandart(const char* TOMLarray) {
    if (TOMLarray == nullptr) {
        throw std::invalid_argument("TOML is empty (NULL)");
    }

    static std::string itog;
    itog.clear(); //Чтобы много раз работала

    itog = TOMLtoStandart(std::string(TOMLarray));

    return itog.c_str();
}


const char* TOMLtoStandart(const char* TOMLarray, std::size_t len) {
    if (TOMLarray == nullptr && len != 0) {
        throw std::invalid_argument("TOML is empty (NULL)");
    }

    std::string stroka(TOMLarray, len);

    static std::string itog;

    itog.clear();

    itog = TOMLtoStandart(stroka);

    return itog.c_str();
}